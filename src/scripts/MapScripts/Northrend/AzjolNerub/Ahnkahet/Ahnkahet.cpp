/*
Copyright (c) 2014-2017 AscEmu Team <http://www.ascemu.org/>
This file is released under the MIT license. See README-MIT for more information.
*/

#include "Setup.h"
#include "Ahnkahet.h"

class Ahnkahet : public InstanceScript
{
public:

    Ahnkahet(MapMgr* pMapMgr) : InstanceScript(pMapMgr)
    {
        generateBossDataState();
    }

    static InstanceScript* Create(MapMgr* pMapMgr) { return new Ahnkahet(pMapMgr); }

    void OnCreatureDeath(Creature* pCreature, Unit* pUnit)
    {
        setData(pCreature->GetEntry(), Finished);
    }
};


void AhnkahetScripts(ScriptMgr* scriptMgr)
{
    scriptMgr->register_instance_script(619, &Ahnkahet::Create);
}
