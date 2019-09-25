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
 *
 */

#ifndef BATTLEGROUNDMGR_H
#define BATTLEGROUNDMGR_H

#include "BattlegroundDefines.h"
#include "BattlegroundScore.h"
#include "BattlegroundInfo.h"
#include "WorldPacket.h"
#include "Server/EventableObject.h"

#define ANTI_CHEAT

class CBattleground;
class MapMgr;
class Player;
class Map;
class Group;
/// AV - Corpse
class Corpse;
class Arena;

typedef CBattleground* (*BattlegroundFactoryMethod)(MapMgr* mgr, uint32 iid, uint32 group, uint32 type);
typedef CBattleground* (*ArenaFactoryMethod)(MapMgr* mgr, uint32 iid, uint32 group, uint32 type, uint32 players_per_side);


class SERVER_DECL CBattlegroundManager : public Singleton<CBattlegroundManager>, public EventableObject
{
    public:

        CBattlegroundManager();
        ~CBattlegroundManager();

        //////////////////////////////////////////////////////////////////////////////////////////
        // void RegisterBgFactory(uint32 map, BattlegroundFactoryMethod method)
        // \note   Registers the specified Battleground class factory method for
        //         the specified Battleground type.
        //         When trying to register a duplicate, the duplicate will be ignored.
        //
        // \param  uint32 map                          -  The map of the Battleground
        // \param  BattlegroundFactoryMethod method    -  The Battleground factory method
        //
        // \return none
        //
        //////////////////////////////////////////////////////////////////////////////////////////
        void RegisterBgFactory(uint32 map, BattlegroundFactoryMethod method);


        //////////////////////////////////////////////////////////////////////////////////////////
        // void RegisterArenaFactory(uint32 map, ArenaFactoryMethod method)
        // \note   Registers the specified Arena class factory method for
        //         the specified Battleground type.
        //         When trying to register a duplicate, the duplicate will be ignored.
        //
        // \param  uint32 map                  -  Map id
        // \param  ArenaFactoryMethod method   -  The Arena factory method
        //
        // \return none
        //
        //////////////////////////////////////////////////////////////////////////////////////////
        void RegisterArenaFactory(uint32 map, ArenaFactoryMethod method);


        //////////////////////////////////////////////////////////////////////////////////////////
        // void RegisterMapForBgType(uint32 type, uint32 map)
        // \note   Registers a Map Id for the specified Battleground type.
        //         When trying to register a duplicate, the duplicate will be ignored.
        //
        // \param  uint32 type  -  The Battleground type
        // \param  uint32 map   -  The map Id
        //
        // \return none
        //
        //////////////////////////////////////////////////////////////////////////////////////////
        void RegisterMapForBgType(uint32 type, uint32 map);

        //////////////////////////////////////////////////////////////////////////////////////////
        // void HandleBattlegroundListPacket(WorldSession* session, uint32 bgType, uint8 from)
        // \note   Player opened battleground menu. Return info about current battleground.
        //
        // TODO: Params
        //////////////////////////////////////////////////////////////////////////////////////////
        void HandleBattlegroundListPacket(WorldSession* session, uint32 bgType, uint8 from = 0);

        //////////////////////////////////////////////////////////////////////////////////////////
        // void HandleArenaJoin(WorldSession* session, uint32 bgType, uint8 as_group, uint8 rated_match)
        // \note   Player requests to join arena queue.
        //
        // TODO: Params
        //////////////////////////////////////////////////////////////////////////////////////////
        void HandleArenaJoin(WorldSession* session, uint32 bgType, uint8 as_group, uint8 rated_match);

        //////////////////////////////////////////////////////////////////////////////////////////
        // void HandleBattlegroundJoin(WorldSession* session, WorldPacket& pck)
        // \note   Player requests to join battleground queue.
        //
        // TODO: Params
        //////////////////////////////////////////////////////////////////////////////////////////
        void HandleBattlegroundJoin(WorldSession* session, WorldPacket& pck);

        /// Periodic update
        void EventQueueUpdate();

        /// Removes player from any queues
        void RemovePlayerFromQueues(Player* plr);

        /// Called when group is Disbanded or left queue
        void RemoveGroupFromQueues(Group* grp);

        //// Delete battlegroun from queue? TODO: research this
        void DeleteBattleground(CBattleground* bg);

        /// Sends battleground/arena queue status to player ("ready","timeInQue","UpdateRunningTimeOfCurrentBG")
        void SendQueueStatus(Player* plr, BattleGroundStatus Status, uint32 Type, uint32 InstanceID, uint32 Time, uint32 MapId, uint8 RatedMatch);
};

#define BattlegroundManager CBattlegroundManager::getSingleton()

#endif // BATTLEGROUNDMGR_H
