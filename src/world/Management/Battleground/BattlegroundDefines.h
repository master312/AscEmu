#include "CommonTypes.hpp"

const uint32 BG_SCORE_AB_BASES_ASSAULTED        = 0;
const uint32 BG_SCORE_AB_BASES_CAPTURED         = 1;
const uint32 BG_SCORE_AV_GRAVEYARDS_ASSAULTED   = 0;
const uint32 BG_SCORE_AV_GRAVEYARDS_DEFENDED    = 1;
const uint32 BG_SCORE_AV_TOWERS_ASSAULTED       = 2;
const uint32 BG_SCORE_AV_TOWERS_DEFENDED        = 3;
const uint32 BG_SCORE_AV_MINES_CAPTURES         = 4;
const uint32 BG_SCORE_EOTS_FLAGS_CAPTURED       = 0;
const uint32 BG_SCORE_WSG_FLAGS_CAPTURED        = 0;
const uint32 BG_SCORE_WSG_FLAGS_RETURNED        = 1;
const uint32 BG_SCORE_IOC_BASES_ASSAULTED       = 0;
const uint32 BG_SCORE_IOC_BASES_DEFENDED        = 1;

const uint32 SOUND_BATTLEGROUND_BEGIN   = 3439;
const uint32 SOUND_FLAG_RESPAWN         = 8232;
const uint32 SOUND_HORDE_SCORES         = 8213;
const uint32 SOUND_ALLIANCE_SCORES      = 8173;
const uint32 SOUND_ALLIANCE_CAPTURE     = 8174;
const uint32 SOUND_HORDE_CAPTURE        = 8212;
const uint32 SOUND_FLAG_RETURNED        = 8192;
const uint32 SOUND_HORDEWINS            = 8454;
const uint32 SOUND_ALLIANCEWINS         = 8455;
const uint32 SOUND_HORDE_BGALMOSTEND    = 8456;
const uint32 SOUND_ALLIANCE_BGALMOSTEND = 8457;

const uint32 BG_PREPARATION             = 44521;
const uint32 BG_REVIVE_PREPARATION      = 44535;
const uint32 RESURRECT_SPELL            = 21074; // Spirit Healer Res
const uint32 BG_DESERTER                = 26013;

#define MAX_LEVEL_GROUP 9
#define MINIMUM_PLAYERS_ON_EACH_SIDE_FOR_BG 1
#define MAXIMUM_BATTLEGROUNDS_PER_LEVEL_GROUP 50
#define LEVEL_GROUP_70 8

enum BattlegroundDbcIndex
{
    BGDBC_ALTERAC_VALLEY = 1,
    BGDBC_WARSONG_GULCH = 2,
    BGDBC_ARATHI_BASIN = 3,
    BGDBC_ARENA_NAGRAND = 4,
    BGDBC_ARENA_BLADES_EDGE = 5,
    BGDBC_ARENA_ALLMAPS = 6,
    BGDBC_EYE_OF_THE_STORM = 7,
    BGDBC_RUINS_OF_LORDAERON = 8,
    BGDBC_STRAND_OF_THE_ANCIENT = 9,
    BGDBC_DALARAN_SEWERS = 10,
    BGDBC_RING_OF_VALOR = 11,
    BGDBC_ISLE_OF_CONQUEST = 30,
    BGDBC_ROWS = 30,
};

enum BattleGroundTypes
{
    BATTLEGROUND_ALTERAC_VALLEY = 1,
    BATTLEGROUND_WARSONG_GULCH = 2,
    BATTLEGROUND_ARATHI_BASIN = 3,
    BATTLEGROUND_ARENA_2V2 = 4,
    BATTLEGROUND_ARENA_3V3 = 5,
    BATTLEGROUND_ARENA_5V5 = 6,
    BATTLEGROUND_EYE_OF_THE_STORM = 7,
    BATTLEGROUND_STRAND_OF_THE_ANCIENT = 9,
    BATTLEGROUND_ISLE_OF_CONQUEST = 30,
    BATTLEGROUND_RANDOM = 32,
    BATTLEGROUND_NUM_TYPES = 33   // Based on BattlemasterList.dbc, make the storage arrays big enough! On 3.1.3 the last one was 11 The Ring of Valor, so 12 was enough here, but on 3.2.0 there is 32 All Battlegrounds!
};

enum BattleGroundMasterTypes
{
    BGMASTER_CREATURE = 1,
    BGMASTER_OBJECT = 2,
    BGMASTER_ITEM = 3
};

enum BattleGroundStatus
{
    BGSTATUS_NOFLAGS = 0, // wtfbbq, why aren't there any flags?
    BGSTATUS_INQUEUE = 1, // Battleground has a queue, player is now in queue
    BGSTATUS_READY = 2,   // Battleground is ready to join
    BGSTATUS_TIME = 3     // Ex. Wintergrasp time remaining
};