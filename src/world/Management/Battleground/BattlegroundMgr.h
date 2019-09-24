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

inline bool isArena(uint32 x)
{
    return (x >= BATTLEGROUND_ARENA_2V2 && x <= BATTLEGROUND_ARENA_5V5);
}

// get level grouping for player
static inline uint32 GetLevelGrouping(uint32 level)
{
    if (level < 10)
        return 0;
    else if (level < 20)
        return 1;
    else if (level < 30)
        return 2;
    else if (level < 40)
        return 3;
    else if (level < 50)
        return 4;
    else if (level < 60)
        return 5;
    else if (level < 70)
        return 6;
    else if (level < 80)
        return 7;
    else
        return 8;
}

static inline uint32 GetFieldCount(uint32 BGType)
{
    switch (BGType)
    {
        case BATTLEGROUND_ALTERAC_VALLEY:
            return 5;
        case BATTLEGROUND_ARATHI_BASIN:
        case BATTLEGROUND_WARSONG_GULCH:
        case BATTLEGROUND_STRAND_OF_THE_ANCIENT:
        case BATTLEGROUND_ISLE_OF_CONQUEST:
            return 2;
        case BATTLEGROUND_EYE_OF_THE_STORM:
            return 1;
        default:
            return 0;
    }
}

class Arena;

typedef CBattleground* (*BattlegroundFactoryMethod)(MapMgr* mgr, uint32 iid, uint32 group, uint32 type);
typedef CBattleground* (*ArenaFactoryMethod)(MapMgr* mgr, uint32 iid, uint32 group, uint32 type, uint32 players_per_side);


class SERVER_DECL CBattlegroundManager : public Singleton<CBattlegroundManager>, public EventableObject
{
    // Battleground Instance Map
    std::map<uint32, CBattleground*> m_instances[BATTLEGROUND_NUM_TYPES];
    Mutex m_instanceLock;

    // Max Id
    uint32 m_maxBattlegroundId[BATTLEGROUND_NUM_TYPES];

    // Queue System
    // Instance Id -> list<Player guid> [ BattlegroundType ] (instance 0 - first available)
    std::list<uint32> m_queuedPlayers[BATTLEGROUND_NUM_TYPES][MAX_LEVEL_GROUP];

    // Instance Id -> list<Group id> [BattlegroundType][LevelGroup]
    std::list<uint32> m_queuedGroups[BATTLEGROUND_NUM_TYPES];

    Mutex m_queueLock;

    // Bg factory methods by Bg map Id
    std::map<uint32, BattlegroundFactoryMethod> bgFactories;

    // Arena factory methods
    std::vector<ArenaFactoryMethod> arenaFactories;

    // Bg map IDs by Bg type Id
    std::map<uint32, uint32> bgMaps;

    // Arena map IDs
    std::vector<uint32> arenaMaps;

    // All battlegrounds that are available in random BG queue
    std::vector<uint32> avalibleInRandom;

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


        void HandleBattlegroundListPacket(WorldSession* m_session, uint32 BattlegroundType, uint8 from = 0);
        void HandleArenaJoin(WorldSession* m_session, uint32 BattlegroundType, uint8 as_group, uint8 rated_match);

        void OnPlayerLogout(Player* plr);

        void EventQueueUpdate();
        void EventQueueUpdate(bool forceStart);

        void HandleGetBattlegroundQueueCommand(WorldSession* m_session);

        void HandleBattlegroundJoin(WorldSession* m_session, WorldPacket& pck);

        void RemovePlayerFromQueues(Player* plr);
        void RemoveGroupFromQueues(Group* grp);

        CBattleground* CreateInstance(uint32 Type, uint32 LevelGroup);

        bool CanCreateInstance(uint32 Type, uint32 LevelGroup);

        void DeleteBattleground(CBattleground* bg);

        void SendBattlefieldStatus(Player* plr, BattleGroundStatus Status, uint32 Type, uint32 InstanceID, uint32 Time, uint32 MapId, uint8 RatedMatch);

        uint32 GetArenaGroupQInfo(Group* group, int type, uint32* avgRating);

        int CreateArenaType(int type, Group* group1, Group* group2);

        void AddPlayerToBgTeam(CBattleground* bg, std::deque<uint32> *playerVec, uint32 i, uint32 j, int Team);

        void AddPlayerToBg(CBattleground* bg, std::deque<uint32> *playerVec, uint32 i, uint32 j);

        void AddGroupToArena(CBattleground* bg, Group* group, uint32 nteam);

        uint32 GetMinimumPlayers(uint32 dbcIndex);

        uint32 GetMaximumPlayers(uint32 dbcIndex);
};

#define BattlegroundManager CBattlegroundManager::getSingleton()

#endif // BATTLEGROUNDMGR_H
