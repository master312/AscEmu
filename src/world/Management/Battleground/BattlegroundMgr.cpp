/*
 * AscEmu Framework based on ArcEmu MMORPG Server
 * Copyright (c) 2014-2019 AscEmu Team <http://www.ascemu.org>
 * Copyright (C) 2008-2012 ArcEmu Team <http://www.ArcEmu.org/>
 * Copyright (C) 2005-2007 Ascent Team
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "StdAfx.h"
#include "Management/Battleground/Battleground.h"
#include "Management/Battleground/BattlegroundMgr.h"
#include "Management/Arenas.h"
#include "Management/ArenaTeam.h"
#include "Server/MainServerDefines.h"
#include "Map/MapMgr.h"
#include "Map/WorldCreator.h"
#include "Chat/ChatHandler.hpp"
#include "Objects/ObjectMgr.h"
#include "Server/Packets/SmsgArenaError.h"
#include "Server/Packets/CmsgBattlemasterJoin.h"

// uint8 GetBattlegroundCaption(BattleGroundTypes bgType)
// {
//     switch (bgType)
//     {
//         case BATTLEGROUND_ALTERAC_VALLEY:
//             return 38;
//         case BATTLEGROUND_WARSONG_GULCH:
//             return 39;
//         case BATTLEGROUND_ARATHI_BASIN:
//             return 40;
//         case BATTLEGROUND_ARENA_2V2:
//             return 41;
//         case BATTLEGROUND_ARENA_3V3:
//             return 42;
//         case BATTLEGROUND_ARENA_5V5:
//             return 43;
//         case BATTLEGROUND_EYE_OF_THE_STORM:
//             return 44;
//         case BATTLEGROUND_STRAND_OF_THE_ANCIENT:
//             return 34;
//         default:
//             return 45;
//     }
// }


using namespace AscEmu::Packets;

initialiseSingleton(CBattlegroundManager);

CBattlegroundManager::CBattlegroundManager() : EventableObject()
{
    // Yes we will be running from WorldRunnable
    m_holder = sEventMgr.GetEventHolder(WORLD_INSTANCE);

    sEventMgr.AddEvent(this, &CBattlegroundManager::EventQueueUpdate, EVENT_BATTLEGROUND_QUEUE_UPDATE, 15000, 0, 0);
}

CBattlegroundManager::~CBattlegroundManager()
{
}

void CBattlegroundManager::RegisterBgFactory(uint32 map, BattlegroundFactoryMethod method)
{
}

void CBattlegroundManager::RegisterArenaFactory(uint32 map, ArenaFactoryMethod method)
{
}

void CBattlegroundManager::RegisterMapForBgType(uint32 type, uint32 map)
{
}

void CBattlegroundManager::HandleBattlegroundListPacket(WorldSession* m_session, uint32 BattlegroundType, uint8 from)
{
}

void CBattlegroundManager::HandleBattlegroundJoin(WorldSession* m_session, WorldPacket& pck)
{
}

void CBattlegroundManager::EventQueueUpdate()
{
    this->EventQueueUpdate(false);
}

void CBattlegroundManager::RemovePlayerFromQueues(Player* plr)
{
}

void CBattlegroundManager::RemoveGroupFromQueues(Group* grp)
{
}

/// Returns the minimum number of players (Only valid for battlegrounds)
uint32 CBattlegroundManager::GetMinimumPlayers(uint32 dbcIndex)
{
    switch (dbcIndex)
    {
        case BATTLEGROUND_ALTERAC_VALLEY:
            return worldConfig.bg.minPlayerCountAlteracValley;
        case BATTLEGROUND_WARSONG_GULCH:
            return worldConfig.bg.minPlayerCountWarsongGulch;
        case BATTLEGROUND_ARATHI_BASIN:
            return worldConfig.bg.minPlayerCountArathiBasin;
        case BATTLEGROUND_EYE_OF_THE_STORM:
            return worldConfig.bg.minPlayerCountEyeOfTheStorm;
        case BATTLEGROUND_ARENA_2V2:
            return worldConfig.arena.minPlayerCount2V2;
        case BATTLEGROUND_ARENA_3V3:
            return worldConfig.arena.minPlayerCount3V3;
        case BATTLEGROUND_ARENA_5V5:
            return worldConfig.arena.minPlayerCount5V5;
        case BATTLEGROUND_STRAND_OF_THE_ANCIENT:
            return worldConfig.bg.minPlayerCountStrandOfTheAncients;
        case BATTLEGROUND_ISLE_OF_CONQUEST:
            return worldConfig.bg.minPlayerCountIsleOfConquest;
        default:
            return 1;
    }
}

/// Returns the maximum number of players (Only valid for battlegrounds)
uint32 CBattlegroundManager::GetMaximumPlayers(uint32 dbcIndex)
{
    switch (dbcIndex)
    {
        case BATTLEGROUND_ALTERAC_VALLEY:
            return worldConfig.bg.maxPlayerCountAlteracValley;
        case BATTLEGROUND_WARSONG_GULCH:
            return worldConfig.bg.maxPlayerCountWarsongGulch;
        case BATTLEGROUND_ARATHI_BASIN:
            return worldConfig.bg.maxPlayerCountArathiBasin;
        case BATTLEGROUND_EYE_OF_THE_STORM:
            return worldConfig.bg.maxPlayerCountEyeOfTheStorm;
        case BATTLEGROUND_ARENA_2V2:
            return worldConfig.arena.maxPlayerCount2V2;
        case BATTLEGROUND_ARENA_3V3:
            return worldConfig.arena.maxPlayerCount3V3;
        case BATTLEGROUND_ARENA_5V5:
            return worldConfig.arena.maxPlayerCount5V5;
        case BATTLEGROUND_STRAND_OF_THE_ANCIENT:
            return worldConfig.bg.maxPlayerCountStrandOfTheAncients;
        case BATTLEGROUND_ISLE_OF_CONQUEST:
            return worldConfig.bg.maxPlayerCountIsleOfConquest;
        default:
            return 1;
    }
}

void CBattlegroundManager::DeleteBattleground(CBattleground* bg)
{
    //Your queue on battleground instance %u is no longer valid, the instance no longer exists.
}

void CBattlegroundManager::SendQueueStatus(Player* plr, BattleGroundStatus Status, uint32 Type, uint32 InstanceID, uint32 Time, uint32 MapId, uint8 RatedMatch)
{
}

void CBattlegroundManager::HandleArenaJoin(WorldSession* m_session, uint32 BattlegroundType, uint8 as_group, uint8 rated_match)
{
}
