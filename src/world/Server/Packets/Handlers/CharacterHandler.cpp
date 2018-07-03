/*
Copyright (c) 2014-2018 AscEmu Team <http://www.ascemu.org>
This file is released under the MIT license. See README-MIT for more information.
*/

#include "StdAfx.h"
#include "Server/WorldSession.h"
#include "Server/Packets/CmsgSetFactionAtWar.h"
#include "Server/Packets/CmsgSetFactionInactive.h"
#include "Units/Players/Player.h"
#include "Server/Packets/CmsgCharDelete.h"
#include "Server/Packets/SmsgCharDelete.h"
#include "Server/Packets/CmsgCharFactionChange.h"
#include "Server/Packets/SmsgCharFactionChange.h"
#include "Server/Packets/SmsgCharacterLoginFailed.h"
#include "Server/Packets/CmsgPlayerLogin.h"
#include "Server/Packets/CmsgCharRename.h"
#include "Server/Packets/SmsgCharRename.h"
#include "Objects/ObjectMgr.h"
#include "Server/MainServerDefines.h"
#include "Units/Players/PlayerClasses.hpp"
#include "Storage/MySQLDataStore.hpp"
#include "Server/Packets/SmsgCharCreate.h"
#include "Server/Packets/CmsgCharCreate.h"
#include "Server/Packets/CmsgCharCustomize.h"
#include "Server/Packets/SmsgCharCustomize.h"
#include "Server/LogonCommClient/LogonCommHandler.h"
#if VERSION_STRING == Cata
#include "GameCata/Management/GuildMgr.h"
#endif

using namespace AscEmu::Packets;

void WorldSession::handleSetFactionAtWarOpcode(WorldPacket& recvPacket)
{
    CmsgSetFactionAtWar recv_packet;
    if (!recv_packet.deserialise(recvPacket))
        return;

    GetPlayer()->SetAtWar(recv_packet.id, recv_packet.state == 1);
}

void WorldSession::handleSetFactionInactiveOpcode(WorldPacket& recvPacket)
{
    CmsgSetFactionInactive recv_packet;
    if (!recv_packet.deserialise(recvPacket))
        return;

    GetPlayer()->SetFactionInactive(recv_packet.id, recv_packet.state == 1);
}

void WorldSession::handleCharDeleteOpcode(WorldPacket& recvPacket)
{
    CmsgCharDelete recv_packet;
    if (!recv_packet.deserialise(recvPacket))
        return;

    const uint8_t deleteResult = deleteCharacter(recv_packet.guid);
    SendPacket(SmsgCharDelete(deleteResult).serialise().get());
}

#if VERSION_STRING > TBC
// \todo port player to a main city of his new faction
void WorldSession::handleCharFactionOrRaceChange(WorldPacket& recvPacket)
{
    CmsgCharFactionChange recv_packet;
    if (!recv_packet.deserialise(recvPacket))
        return;

    const auto playerInfoPacket = objmgr.GetPlayerInfo(recv_packet.guid.getGuidLow());
    if (playerInfoPacket == nullptr)
    {
        SendPacket(SmsgCharFactionChange(E_CHAR_CREATE_ERROR).serialise().get());
        return;
    }

    const uint32_t used_loginFlag = ((recvPacket.GetOpcode() == CMSG_CHAR_RACE_CHANGE) ? LOGIN_CUSTOMIZE_RACE : LOGIN_CUSTOMIZE_FACTION);
    uint32_t newflags = 0;

    const auto loginFlagsQuery = CharacterDatabase.Query("SELECT login_flags FROM characters WHERE guid = %u", recv_packet.guid.getGuidLow());
    if (loginFlagsQuery)
    {
        uint16_t loginFlags = loginFlagsQuery->Fetch()[0].GetUInt16();
        if (!(loginFlags & used_loginFlag))
        {
            SendPacket(SmsgCharFactionChange(E_CHAR_CREATE_ERROR).serialise().get());
            return;
        }
        newflags = loginFlags - used_loginFlag;
    }

    if (!sMySQLStore.getPlayerCreateInfo(recv_packet.charCreate._race, playerInfoPacket->cl))
    {
        SendPacket(SmsgCharFactionChange(E_CHAR_CREATE_ERROR).serialise().get());
        return;
    }

    const auto loginErrorCode = VerifyName(recv_packet.charCreate.name.c_str(), recv_packet.charCreate.name.length());
    if (loginErrorCode != E_CHAR_NAME_SUCCESS)
    {
        SendPacket(SmsgCharFactionChange(loginErrorCode).serialise().get());
        return;
    }

    if (!HasGMPermissions())
    {
        const auto bannedNamesQuery = CharacterDatabase.Query("SELECT COUNT(*) FROM `banned_names` WHERE name = '%s'",
            CharacterDatabase.EscapeString(recv_packet.charCreate.name).c_str());
        if (bannedNamesQuery)
        {
            if (bannedNamesQuery->Fetch()[0].GetUInt32() > 0)
            {
                SendPacket(SmsgCharFactionChange(E_CHAR_NAME_RESERVED).serialise().get());
                return;
            }
        }
    }

    const auto playerInfo = objmgr.GetPlayerInfoByName(recv_packet.charCreate.name.c_str());
    if (playerInfo != nullptr && playerInfo->guid != recv_packet.guid.getGuidLow())
    {
        SendPacket(SmsgCharFactionChange(E_CHAR_CREATE_NAME_IN_USE).serialise().get());
        return;
    }

    Player::CharChange_Looks(recv_packet.guid, recv_packet.charCreate.gender, recv_packet.charCreate.skin,
        recv_packet.charCreate.face, recv_packet.charCreate.hairStyle, recv_packet.charCreate.hairColor, recv_packet.charCreate.facialHair);

    std::string newname = recv_packet.charCreate.name;
    Util::CapitalizeString(newname);

    objmgr.RenamePlayerInfo(playerInfoPacket, playerInfoPacket->name, newname.c_str());

    CharacterDatabase.Execute("UPDATE `characters` set name = '%s', login_flags = %u, race = %u WHERE guid = %u",
        newname.c_str(), newflags, static_cast<uint32_t>(recv_packet.charCreate._race), recv_packet.guid.getGuidLow());

    SendPacket(SmsgCharFactionChange(0, recv_packet.guid, recv_packet.charCreate).serialise().get());
}
#endif

void WorldSession::handlePlayerLoginOpcode(WorldPacket& recvPacket)
{
    CmsgPlayerLogin recv_packet;
    if (!recv_packet.deserialise(recvPacket))
        return;

    LOG_DEBUG("Received CMSG_PLAYER_LOGIN %u (guidLow)", recv_packet.guid.getGuidLow());

    if (objmgr.GetPlayer(recv_packet.guid.getGuidLow()) != nullptr || m_loggingInPlayer || _player)
    {
        SendPacket(SmsgCharacterLoginFailed(E_CHAR_LOGIN_DUPLICATE_CHARACTER).serialise().get());
        return;
    }

    const auto query = new AsyncQuery(new SQLClassCallbackP0<WorldSession>(this, &WorldSession::loadPlayerFromDBProc));
    query->AddQuery("SELECT guid,class FROM characters WHERE guid = %u AND login_flags = %u", recv_packet.guid.getGuidLow(), static_cast<uint32_t>(LOGIN_NO_FLAG));
    CharacterDatabase.QueueAsyncQuery(query);
}

void WorldSession::handleCharRenameOpcode(WorldPacket& recvPacket)
{
    CmsgCharRename recv_packet;
    if (!recv_packet.deserialise(recvPacket))
        return;

    const auto playerInfo = objmgr.GetPlayerInfo(recv_packet.guid.getGuidLow());
    if (playerInfo == nullptr)
        return;

    QueryResult* result = CharacterDatabase.Query("SELECT login_flags FROM characters WHERE guid = %u AND acct = %u", recv_packet.guid.getGuidLow(), _accountId);
    if (result == nullptr)
        return;

    const auto loginErrorCode = VerifyName(recv_packet.name.c_str(), recv_packet.name.length());
    if (loginErrorCode != E_CHAR_NAME_SUCCESS)
    {
        SendPacket(SmsgCharRename(recv_packet.size, loginErrorCode, recv_packet.guid, recv_packet.name).serialise().get());
        return;
    }

    QueryResult* result2 = CharacterDatabase.Query("SELECT COUNT(*) FROM banned_names WHERE name = '%s'", CharacterDatabase.EscapeString(recv_packet.name).c_str());
    if (result2)
    {
        if (result2->Fetch()[0].GetUInt32() > 0)
        {
            SendPacket(SmsgCharRename(recv_packet.size, E_CHAR_NAME_PROFANE, recv_packet.guid, recv_packet.name).serialise().get());
            return;
        }
    }

    if (objmgr.GetPlayerInfoByName(recv_packet.name.c_str()) != nullptr)
    {
        SendPacket(SmsgCharRename(recv_packet.size, E_CHAR_CREATE_NAME_IN_USE, recv_packet.guid, recv_packet.name).serialise().get());
        return;
    }

    std::string newName = recv_packet.name;
    Util::CapitalizeString(newName);
    objmgr.RenamePlayerInfo(playerInfo, playerInfo->name, newName.c_str());

    sPlrLog.writefromsession(this, "renamed character %s, %u (guid), to %s.", playerInfo->name, playerInfo->guid, newName.c_str());

    free(playerInfo->name);

    playerInfo->name = strdup(newName.c_str());

    CharacterDatabase.WaitExecute("UPDATE characters SET name = '%s' WHERE guid = %u", newName.c_str(), recv_packet.guid.getGuidLow());
    CharacterDatabase.WaitExecute("UPDATE characters SET login_flags = %u WHERE guid = %u", static_cast<uint32_t>(LOGIN_NO_FLAG), recv_packet.guid.getGuidLow());

    SendPacket(SmsgCharRename(recv_packet.size, E_RESPONSE_SUCCESS, recv_packet.guid, newName).serialise().get());
}

void WorldSession::loadPlayerFromDBProc(QueryResultVector& results)
{
    if (results.empty())
    {
        SendPacket(SmsgCharacterLoginFailed(E_CHAR_LOGIN_NO_CHARACTER).serialise().get());
        return;
    }

    QueryResult* result = results[0].result;
    if (result == nullptr)
    {
        SendPacket(SmsgCharacterLoginFailed(E_CHAR_LOGIN_NO_CHARACTER).serialise().get());
        return;
    }

    Field* fields = result->Fetch();

    const uint64_t playerGuid = fields[0].GetUInt64();
    const uint8_t _class = fields[1].GetUInt8();

    Player* player = nullptr;
    switch (_class)
    {
        case WARRIOR:
            player = new Warrior(static_cast<uint32_t>(playerGuid));
            break;
        case PALADIN:
            player = new Paladin(static_cast<uint32_t>(playerGuid));
            break;
        case HUNTER:
            player = new Hunter(static_cast<uint32_t>(playerGuid));
            break;
        case ROGUE:
            player = new Rogue(static_cast<uint32_t>(playerGuid));
            break;
        case PRIEST:
            player = new Priest(static_cast<uint32_t>(playerGuid));
            break;
#if VERSION_STRING > TBC
        case DEATHKNIGHT:
            player = new DeathKnight(static_cast<uint32_t>(playerGuid));
            break;
#endif
        case SHAMAN:
            player = new Shaman(static_cast<uint32_t>(playerGuid));
            break;
        case MAGE:
            player = new Mage(static_cast<uint32_t>(playerGuid));
            break;
        case WARLOCK:
            player = new Warlock(static_cast<uint32_t>(playerGuid));
            break;
        case DRUID:
            player = new Druid(static_cast<uint32_t>(playerGuid));
            break;
    }

    if (player == nullptr)
    {
        LOG_ERROR("Unknown class %u!", _class);
        SendPacket(SmsgCharacterLoginFailed(E_CHAR_LOGIN_NO_CHARACTER).serialise().get());
        return;
    }

    player->SetSession(this);

    m_bIsWLevelSet = false;

    LOG_DEBUG("Async loading player %u", static_cast<uint32_t>(playerGuid));
    m_loggingInPlayer = player;
    player->LoadFromDB(static_cast<uint32_t>(playerGuid));
}

uint8_t WorldSession::deleteCharacter(WoWGuid guid)
{
    const auto playerInfo = objmgr.GetPlayerInfo(guid.getGuidLow());
    if (playerInfo != nullptr && playerInfo->m_loggedInPlayer == nullptr)
    {
        QueryResult* result = CharacterDatabase.Query("SELECT name FROM characters WHERE guid = %u AND acct = %u", guid.getGuidLow(), _accountId);
        if (!result)
            return E_CHAR_DELETE_FAILED;

        std::string name = result->Fetch()[0].GetString();
        delete result;

#if VERSION_STRING != Cata
        if (playerInfo->guild)
        {
            if (playerInfo->guild->GetGuildLeader() == playerInfo->guid)
                return E_CHAR_DELETE_FAILED_GUILD_LEADER;

            playerInfo->guild->RemoveGuildMember(playerInfo, nullptr);
        }
#else
        if (playerInfo->m_guild)
        {
            const auto guild = sGuildMgr.getGuildById(playerInfo->m_guild);
            if (guild != nullptr && guild->getLeaderGUID() == playerInfo->guid)
                return E_CHAR_DELETE_FAILED_GUILD_LEADER;

            if (guild != nullptr)
                guild->handleRemoveMember(this, playerInfo->guid);
        }
#endif

        for (uint8_t i = 0; i < NUM_CHARTER_TYPES; ++i)
        {
            const auto charter = objmgr.GetCharterByGuid(guid, static_cast<CharterTypes>(i));
            if (charter != nullptr)
                charter->RemoveSignature(guid.getGuidLow());
        }


        for (uint8_t i = 0; i < NUM_ARENA_TEAM_TYPES; ++i)
        {
            const auto arenaTeam = objmgr.GetArenaTeamByGuid(guid.getGuidLow(), i);
            if (arenaTeam != nullptr && arenaTeam->m_leader == guid.getGuidLow())
                return E_CHAR_DELETE_FAILED_ARENA_CAPTAIN;

            if (arenaTeam != nullptr)
                arenaTeam->RemoveMember(playerInfo);
        }

        sPlrLog.writefromsession(this, "deleted character %s %u (guidLow))", name.c_str(), guid.getGuidLow());

        CharacterDatabase.WaitExecute("DELETE FROM characters WHERE guid = %u", guid.getGuidLow());

        const auto corpse = objmgr.GetCorpseByOwner(guid.getGuidLow());
        if (corpse)
            CharacterDatabase.Execute("DELETE FROM corpses WHERE guid = %u", corpse->getGuidLow());

        CharacterDatabase.Execute("DELETE FROM playeritems WHERE ownerguid=%u", guid.getGuidLow());
        CharacterDatabase.Execute("DELETE FROM gm_tickets WHERE playerguid = %u", guid.getGuidLow());
        CharacterDatabase.Execute("DELETE FROM playerpets WHERE ownerguid = %u", guid.getGuidLow());
        CharacterDatabase.Execute("DELETE FROM playerpetspells WHERE ownerguid = %u", guid.getGuidLow());
        CharacterDatabase.Execute("DELETE FROM tutorials WHERE playerId = %u", guid.getGuidLow());
        CharacterDatabase.Execute("DELETE FROM questlog WHERE player_guid = %u", guid.getGuidLow());
        CharacterDatabase.Execute("DELETE FROM playercooldowns WHERE player_guid = %u", guid.getGuidLow());
        CharacterDatabase.Execute("DELETE FROM mailbox WHERE player_guid = %u", guid.getGuidLow());
        CharacterDatabase.Execute("DELETE FROM social_friends WHERE character_guid = %u OR friend_guid = %u", guid.getGuidLow(), guid.getGuidLow());
        CharacterDatabase.Execute("DELETE FROM social_ignores WHERE character_guid = %u OR ignore_guid = %u", guid.getGuidLow(), guid.getGuidLow());
        CharacterDatabase.Execute("DELETE FROM character_achievement WHERE guid = %u AND achievement NOT IN (457, 467, 466, 465, 464, 463, 462, 461, 460, 459, 458, 1404, 1405, 1406, 1407, 1408, 1409, 1410, 1411, 1412, 1413, 1415, 1414, 1416, 1417, 1418, 1419, 1420, 1421, 1422, 1423, 1424, 1425, 1426, 1427, 1463, 1400, 456, 1402)", guid.getGuidLow());
        CharacterDatabase.Execute("DELETE FROM character_achievement_progress WHERE guid = %u", guid.getGuidLow());
        CharacterDatabase.Execute("DELETE FROM playerspells WHERE GUID = %u", guid.getGuidLow());
        CharacterDatabase.Execute("DELETE FROM playerdeletedspells WHERE GUID = %u", guid.getGuidLow());
        CharacterDatabase.Execute("DELETE FROM playerreputations WHERE guid = %u", guid.getGuidLow());
        CharacterDatabase.Execute("DELETE FROM playerskills WHERE GUID = %u", guid.getGuidLow());

        objmgr.DeletePlayerInfo(guid.getGuidLow());
        return E_CHAR_DELETE_SUCCESS;
    }
    return E_CHAR_DELETE_FAILED;
}

void WorldSession::handleCharCreateOpcode(WorldPacket& recvPacket)
{
    CmsgCharCreate recv_packet;
    if (!recv_packet.deserialise(recvPacket))
        return;

    const auto loginErrorCode = VerifyName(recv_packet.createStruct.name.c_str(), recv_packet.createStruct.name.length());
    if (loginErrorCode != E_CHAR_NAME_SUCCESS)
    {
        SendPacket(SmsgCharCreate(loginErrorCode).serialise().get());
        return;
    }

    const auto isAllowed = sMySQLStore.isCharacterNameAllowed(recv_packet.createStruct.name);
    if (!isAllowed)
    {
        SendPacket(SmsgCharCreate(E_CHAR_NAME_PROFANE).serialise().get());
        return;
    }

    if (objmgr.GetPlayerInfoByName(recv_packet.createStruct.name.c_str()) != nullptr)
    {
        SendPacket(SmsgCharCreate(E_CHAR_CREATE_NAME_IN_USE).serialise().get());
        return;
    }

    const auto isValid = sHookInterface.OnNewCharacter(recv_packet.createStruct._race, recv_packet.createStruct._class, this, recv_packet.createStruct.name.c_str());
    if (!isValid)
    {
        SendPacket(SmsgCharCreate(E_CHAR_CREATE_ERROR).serialise().get());
        return;
    }

    const auto bannedNamesQuery = CharacterDatabase.Query("SELECT COUNT(*) FROM banned_names WHERE name = '%s'", CharacterDatabase.EscapeString(recv_packet.createStruct.name).c_str());
    if (bannedNamesQuery)
    {
        if (bannedNamesQuery->Fetch()[0].GetUInt32() > 0)
        {
            SendPacket(SmsgCharCreate(E_CHAR_NAME_PROFANE).serialise().get());
            return;
        }
    }

#if VERSION_STRING > TBC
    if (worldConfig.player.deathKnightLimit && has_dk && recv_packet.createStruct._class == DEATHKNIGHT)
    {
        SendPacket(SmsgCharCreate(E_CHAR_CREATE_UNIQUE_CLASS_LIMIT).serialise().get());
        return;
    }
#endif

    const auto charactersQuery = CharacterDatabase.Query("SELECT COUNT(*) FROM characters WHERE acct = %u", GetAccountId());
    if (charactersQuery)
    {
        if (charactersQuery->Fetch()[0].GetUInt32() >= 10)
        {
            SendPacket(SmsgCharCreate(E_CHAR_CREATE_SERVER_LIMIT).serialise().get());
            return;
        }
    }

    const auto newPlayer = objmgr.CreatePlayer(recv_packet.createStruct._class);
    newPlayer->SetSession(this);

    if (!newPlayer->Create(recv_packet.createStruct))
    {
        newPlayer->ok_to_remove = true;
        delete newPlayer;

        SendPacket(SmsgCharCreate(E_CHAR_CREATE_FAILED).serialise().get());
        return;
    }

    const auto realmType = sLogonCommHandler.getRealmType();
    if (!HasGMPermissions() && realmType == REALMTYPE_PVP && _side >= 0 && !worldConfig.player.isCrossoverCharsCreationEnabled)
    {
        if ((newPlayer->IsTeamAlliance() && _side == 1) || (newPlayer->IsTeamHorde() && _side == 0))
        {
            newPlayer->ok_to_remove = true;
            delete newPlayer;

            SendPacket(SmsgCharCreate(E_CHAR_CREATE_PVP_TEAMS_VIOLATION).serialise().get());
            return;
        }
    }

#if VERSION_STRING > TBC
    if (worldConfig.player.deathKnightPreReq && !has_level_55_char && recv_packet.createStruct._class == DEATHKNIGHT)
    {
        newPlayer->ok_to_remove = true;
        delete newPlayer;

        SendPacket(SmsgCharCreate(E_CHAR_CREATE_LEVEL_REQUIREMENT).serialise().get());
        return;
    }
#endif

    newPlayer->UnSetBanned();
    newPlayer->addSpell(22027);

    if (newPlayer->getClass() == WARLOCK)
    {
        newPlayer->AddSummonSpell(416, 3110);
        newPlayer->AddSummonSpell(417, 19505);
        newPlayer->AddSummonSpell(1860, 3716);
        newPlayer->AddSummonSpell(1863, 7814);
    }

    newPlayer->SaveToDB(true);

    const auto playerInfo = new PlayerInfo;
    playerInfo->guid = newPlayer->getGuidLow();
    playerInfo->name = strdup(newPlayer->getName().c_str());
    playerInfo->cl = newPlayer->getClass();
    playerInfo->race = newPlayer->getRace();
    playerInfo->gender = newPlayer->getGender();
    playerInfo->acct = GetAccountId();
    playerInfo->m_Group = nullptr;
    playerInfo->subGroup = 0;
    playerInfo->m_loggedInPlayer = nullptr;
    playerInfo->team = newPlayer->GetTeam();
#if VERSION_STRING != Cata
    playerInfo->guild = nullptr;
    playerInfo->guildRank = nullptr;
    playerInfo->guildMember = nullptr;
#else
    playerInfo->m_guild = 0;
    playerInfo->guildRank = GUILD_RANK_NONE;
#endif
    playerInfo->lastOnline = UNIXTIME;

    objmgr.AddPlayerInfo(playerInfo);

    newPlayer->ok_to_remove = true;
    delete newPlayer;

    SendPacket(SmsgCharCreate(E_CHAR_CREATE_SUCCESS).serialise().get());

    sLogonCommHandler.updateAccountCount(GetAccountId(), 1);
}

#if VERSION_STRING > TBC
void WorldSession::handleCharCustomizeLooksOpcode(WorldPacket& recvPacket)
{
    CmsgCharCustomize recv_packet;
    if (!recv_packet.deserialise(recvPacket))
        return;

    const auto loginErrorCode = VerifyName(recv_packet.createStruct.name.c_str(), recv_packet.createStruct.name.length());
    if (loginErrorCode != E_CHAR_NAME_SUCCESS)
    {
        SendPacket(SmsgCharCustomize(E_CHAR_NAME_NO_NAME).serialise().get());
        return;
    }

    QueryResult* result = CharacterDatabase.Query("SELECT COUNT(*) FROM `banned_names` WHERE name = '%s'", CharacterDatabase.EscapeString(recv_packet.createStruct.name).c_str());
    if (result)
    {
        if (result->Fetch()[0].GetUInt32() > 0)
        {
            SendPacket(SmsgCharCustomize(E_CHAR_NAME_PROFANE).serialise().get());
            return;
        }
    }

    const auto playerInfo = objmgr.GetPlayerInfoByName(recv_packet.createStruct.name.c_str());
    if (playerInfo != nullptr && playerInfo->guid != recv_packet.guid.getGuidLow())
    {
        SendPacket(SmsgCharCustomize(E_CHAR_CREATE_NAME_IN_USE).serialise().get());
        return;
    }

    Util::CapitalizeString(recv_packet.createStruct.name);

    CharacterDatabase.WaitExecute("UPDATE `characters` set name = '%s' WHERE guid = %u", recv_packet.createStruct.name.c_str(), recv_packet.guid.getGuidLow());
    CharacterDatabase.WaitExecute("UPDATE `characters` SET login_flags = %u WHERE guid = %u", static_cast<uint32_t>(LOGIN_NO_FLAG), recv_packet.guid.getGuidLow());

    Player::CharChange_Looks(recv_packet.guid, recv_packet.createStruct.gender, recv_packet.createStruct.skin, recv_packet.createStruct.face,
        recv_packet.createStruct.hairStyle, recv_packet.createStruct.hairColor, recv_packet.createStruct.facialHair);

    SendPacket(SmsgCharCustomize(E_RESPONSE_SUCCESS, recv_packet.guid, recv_packet.createStruct).serialise().get());
}
#endif
