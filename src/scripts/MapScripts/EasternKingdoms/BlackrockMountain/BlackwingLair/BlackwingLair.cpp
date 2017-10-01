/*
Copyright (c) 2014-2017 AscEmu Team <http://www.ascemu.org/>
This file is released under the MIT license. See README-MIT for more information.
*/

#include "Setup.h"
#include "BlackwingLair.h"


class BlackwingLair : public InstanceScript
{
public:

    BlackwingLair(MapMgr* pMapMgr) : InstanceScript(pMapMgr)
    {
        generateBossDataState();
    }

    static InstanceScript* Create(MapMgr* pMapMgr) { return new BlackwingLair(pMapMgr); }

    void OnCreatureDeath(Creature* pCreature, Unit* pUnit)
    {
        setData(pCreature->GetEntry(), Finished);
    }
};


void BlackwingLairScripts(ScriptMgr* scriptMgr)
{
    scriptMgr->register_instance_script(469, &BlackwingLair::Create);
}

