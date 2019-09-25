#pragma once
#include "CommonTypes.hpp"
#include "BattlegroundDefines.h"

inline bool isArena(uint32 x)
{
    return (x >= BATTLEGROUND_ARENA_2V2 && x <= BATTLEGROUND_ARENA_5V5);
}

// // get level grouping for player
static inline uint32 GetLevelGrouping(uint32 level)
{
    return (level / 10) - 1;
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