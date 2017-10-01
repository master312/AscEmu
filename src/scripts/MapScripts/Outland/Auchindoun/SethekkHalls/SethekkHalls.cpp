/*
Copyright (c) 2014-2017 AscEmu Team <http://www.ascemu.org/>
This file is released under the MIT license. See README-MIT for more information.
*/

#include "Setup.h"
#include "SethekkHalls.h"


class SethekkHalls : public InstanceScript
{
public:

    SethekkHalls(MapMgr* pMapMgr) : InstanceScript(pMapMgr)
    {
        generateBossDataState();
    }

    static InstanceScript* Create(MapMgr* pMapMgr) { return new SethekkHalls(pMapMgr); }

    void OnCreatureDeath(Creature* pCreature, Unit* pUnit)
    {
        setData(pCreature->GetEntry(), Finished);
    }
};


void SethekkHallsScripts(ScriptMgr* scriptMgr)
{
    scriptMgr->register_instance_script(556, &SethekkHalls::Create);
}

