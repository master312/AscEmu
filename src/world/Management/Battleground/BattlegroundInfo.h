#pragma once
#include "CommonTypes.hpp"
#include "BattlegroundDefines.h"

class SERVER_DECL CBattlegroundInfo 
{
    public:

        CBattlegroundInfo();
        ~CBattlegroundInfo();

        static bool IsArena(uint32 x);

        /// get level grouping for player
        static uint32 GetLevelGrouping(uint32 level);

        static uint32 GetFieldCount(uint32 BGType);

        /// Returns the minimum number of players (Only valid for battlegrounds)
        static uint32 GetMinimumPlayers(uint32 dbcIndex);

        /// Returns the maximum number of players (Only valid for battlegrounds)
        static uint32 GetMaximumPlayers(uint32 dbcIndex);
        
        static uint8 GetBattlegroundCaption(BattleGroundTypes bgType);
};