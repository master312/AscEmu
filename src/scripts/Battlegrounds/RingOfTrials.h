/*
Copyright (c) 2014-2019 AscEmu Team <http://www.ascemu.org>
This file is released under the MIT license. See README-MIT for more information.
*/

#pragma once

#include "Management/Arenas.h"
#include "Management/Battleground/Battleground.h"

class RingOfTrials : public Arena
{
public:

    RingOfTrials(MapMgr* mgr, uint32 id, uint32 lgroup, uint32 t, uint32 players_per_side);
    ~RingOfTrials();

    static CBattleground* Create(MapMgr* m, uint32 i, uint32 l, uint32 t, uint32 players_per_side)
    {
        return new RingOfTrials(m, i, l, t, players_per_side);
    }

    void OnCreate() override;
    void HookOnShadowSight() override;
    LocationVector GetStartingCoords(uint32 Team) override;
    bool HookHandleRepop(Player* plr) override;
};
