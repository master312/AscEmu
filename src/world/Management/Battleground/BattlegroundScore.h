#pragma once
#include "CommonTypes.hpp"
#include <iostream>

struct BattlegroundScore
{
    uint32 KillingBlows;
    uint32 HonorableKills;
    uint32 Deaths;
    uint32 BonusHonor;
    uint32 DamageDone;
    uint32 HealingDone;
    uint32 MiscData[5];

    BattlegroundScore()
    {
        KillingBlows = 0;
        HonorableKills = 0;
        Deaths = 0;
        BonusHonor = 0;
        DamageDone = 0;
        HealingDone = 0;
        std::fill(&MiscData[0], &MiscData[5], 0);
    }
};