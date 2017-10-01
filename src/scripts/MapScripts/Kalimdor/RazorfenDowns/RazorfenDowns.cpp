/*
Copyright (c) 2014-2017 AscEmu Team <http://www.ascemu.org/>
This file is released under the MIT license. See README-MIT for more information.
*/

#include "Setup.h"
#include "RazorfenDowns.h"


class RazorfenDowns : public InstanceScript
{
public:

    RazorfenDowns(MapMgr* pMapMgr) : InstanceScript(pMapMgr)
    {
        generateBossDataState();
    }

    static InstanceScript* Create(MapMgr* pMapMgr) { return new RazorfenDowns(pMapMgr); }

    void OnCreatureDeath(Creature* pCreature, Unit* pUnit)
    {
        setData(pCreature->GetEntry(), Finished);
    }
};


void RazorfenDownsScripts(ScriptMgr* scriptMgr)
{
    scriptMgr->register_instance_script(129, &RazorfenDowns::Create);
}

