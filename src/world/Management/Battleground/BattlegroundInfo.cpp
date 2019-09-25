#include "BattlegroundInfo.h"
#include "Server/World.h"

CBattlegroundInfo::CBattlegroundInfo()
{
}

CBattlegroundInfo::~CBattlegroundInfo()
{
}

bool CBattlegroundInfo::IsArena(uint32 x)
{
    return (x >= BATTLEGROUND_ARENA_2V2 && x <= BATTLEGROUND_ARENA_5V5);
}

uint32 CBattlegroundInfo::GetLevelGrouping(uint32 level)
{
    return (level / 10) - 1;
}

uint32 CBattlegroundInfo::GetFieldCount(uint32 BGType)
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

uint32 CBattlegroundInfo::GetMinimumPlayers(uint32 dbcIndex)
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

uint32 CBattlegroundInfo::GetMaximumPlayers(uint32 dbcIndex)
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

uint8 CBattlegroundInfo::GetBattlegroundCaption(BattleGroundTypes bgType)
{
    switch (bgType)
    {
        case BATTLEGROUND_ALTERAC_VALLEY:
            return 38;
        case BATTLEGROUND_WARSONG_GULCH:
            return 39;
        case BATTLEGROUND_ARATHI_BASIN:
            return 40;
        case BATTLEGROUND_ARENA_2V2:
            return 41;
        case BATTLEGROUND_ARENA_3V3:
            return 42;
        case BATTLEGROUND_ARENA_5V5:
            return 43;
        case BATTLEGROUND_EYE_OF_THE_STORM:
            return 44;
        case BATTLEGROUND_STRAND_OF_THE_ANCIENT:
            return 34;
        default:
            return 45;
    }
}