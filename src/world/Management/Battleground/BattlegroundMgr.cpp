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
#include "Management/Battleground/BattlegroundPackets.h"
#include "Management/Arenas.h"
#include "Management/ArenaTeam.h"
#include "Server/MainServerDefines.h"
#include "Map/MapMgr.h"
#include "Map/WorldCreator.h"
#include "Chat/ChatHandler.hpp"
#include "Objects/ObjectMgr.h"
#include "Server/Packets/SmsgArenaError.h"
#include "Server/Packets/CmsgBattlemasterJoin.h"

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

void CBattlegroundManager::HandleBattlegroundListPacket(WorldSession* session, uint32 bgType, uint8 from)
{

    if (bgType >= BATTLEGROUND_NUM_TYPES) // Prevents hacking
        return;

    BattlegroundPackets::BattlefieldList packet;

    if (from == 0)
    {
        packet.BattlemasterGuid = from; // Send 0 instead of GUID when using the BG UI instead of Battlemaster
    }
    else
    {
        packet.BattlemasterGuid = session->GetPlayer()->getGuid();
    }
    
    packet.BattlemasterListID = from;
    packet.BattlegroundType = bgType;

    if (bgType == BATTLEGROUND_RANDOM)
    {
        uint32 arenaPointsForLosing;

        session->GetPlayer()->FillRandomBattlegroundReward(true, packet.RbgHonorPointsForWinning, packet.RbgArenaPointsForWinning);
        session->GetPlayer()->FillRandomBattlegroundReward(false, packet.RbgHonorPointsForLosing, arenaPointsForLosing);

        packet.HasRandomWinToday = session->GetPlayer()->HasWonRbgToday();
    }

    session->SendPacket(packet.Write());
}

void CBattlegroundManager::HandleBattlegroundJoin(WorldSession* session, WorldPacket& pck)
{
}

void CBattlegroundManager::EventQueueUpdate()
{
}

void CBattlegroundManager::RemovePlayerFromQueues(Player* plr)
{
}

void CBattlegroundManager::RemoveGroupFromQueues(Group* grp)
{
}

void CBattlegroundManager::DeleteBattleground(CBattleground* bg)
{
    //Your queue on battleground instance %u is no longer valid, the instance no longer exists.
}

void CBattlegroundManager::SendQueueStatus(Player* plr, BattleGroundStatus Status, uint32 Type, uint32 InstanceID, uint32 Time, uint32 MapId, uint8 RatedMatch)
{
}

void CBattlegroundManager::HandleArenaJoin(WorldSession* session, uint32 bgType, uint8 as_group, uint8 rated_match)
{
}
