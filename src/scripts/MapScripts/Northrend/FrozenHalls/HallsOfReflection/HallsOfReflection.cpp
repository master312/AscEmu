/*
Copyright (c) 2014-2017 AscEmu Team <http://www.ascemu.org/>
This file is released under the MIT license. See README-MIT for more information.
*/

#include "Setup.h"
#include "HallsOfReflection.h"

class HallsOfReflection : public InstanceScript
{
public:

    HallsOfReflection(MapMgr* pMapMgr) : InstanceScript(pMapMgr)
    {
        generateBossDataState();
    }

    static InstanceScript* Create(MapMgr* pMapMgr) { return new HallsOfReflection(pMapMgr); }

    void OnCreatureDeath(Creature* pCreature, Unit* pUnit)
    {
        setData(pCreature->GetEntry(), Finished);
    }
};


void HallsOfReflectionScripts(ScriptMgr* scriptMgr)
{
#ifdef UseNewMapScriptsProject
    scriptMgr->register_instance_script(668, &HallsOfReflection::Create);
#endif
}
