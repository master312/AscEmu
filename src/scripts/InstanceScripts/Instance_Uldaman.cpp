/*
 * ArcScripts for ArcEmu MMORPG Server
 * Copyright (C) 2008-2012 ArcEmu Team <http://www.ArcEmu.org/>
 * Copyright (C) 2008-2015 Sun++ Team <http://www.sunplusplus.info/>
 * Copyright (C) 2005-2007 Ascent Team
 * Copyright (C) 2007-2015 Moon++ Team <http://www.moonplusplus.info/>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Setup.h"
#include "Instance_Uldaman.h"


class Archaedas : public CreatureAIScript
{
        ADD_CREATURE_FACTORY_FUNCTION(Archaedas);
        SP_AI_Spell spell;
        bool m_spellcheck;

        Archaedas(Creature* pCreature) : CreatureAIScript(pCreature)
        {
            spell.info = sSpellCustomizations.GetSpellInfo(6524);
            spell.cooldown = 10;
            spell.targettype = TARGET_ATTACKING;
            spell.instant = true;
            spell.perctrigger = RandomFloat(20.0f);
            spell.attackstoptimer = 3000;
            m_spellcheck = true;
        }

        void OnCombatStart(Unit* mTarget) override
        {
            CastTime();
            RegisterAIUpdateEvent(getCreature()->GetBaseAttackTime(MELEE));
        }

        void CastTime()
        {
            spell.casttime = spell.cooldown;
        }

        void OnCombatStop(Unit* mTarget) override
        {
            CastTime();
            setAIAgent(AGENT_NULL);
            getCreature()->GetAIInterface()->setAiState(AI_STATE_IDLE);
            RemoveAIUpdateEvent();
        }

        void OnDied(Unit* mKiller) override
        {
            CastTime();
        }

        void AIUpdate() override
        {
            float val = RandomFloat(100.0f);
            SpellCast(val);
        }

        void SpellCast(float val)
        {
            if (getCreature()->GetCurrentSpell() == NULL && getCreature()->GetAIInterface()->getNextTarget())
            {
                float comulativeperc = 0;
                Unit* target = NULL;
                spell.casttime--;

                if (m_spellcheck)
                {
                    spell.casttime = spell.cooldown;
                    target = getCreature()->GetAIInterface()->getNextTarget();
                    getCreature()->CastSpell(target, spell.info, spell.instant);

                    if (spell.speech != "")
                    {
                        getCreature()->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spell.speech.c_str());
                        getCreature()->PlaySoundToSet(spell.soundid);
                    }

                    m_spellcheck = false;
                    return;
                }

                if ((val > comulativeperc && val <= (comulativeperc + spell.perctrigger)) || !spell.casttime)
                {
                    getCreature()->setAttackTimer(spell.attackstoptimer, false);
                    m_spellcheck = true;
                }
                comulativeperc += spell.perctrigger;
            }
        }

};


class Revelosh : public CreatureAIScript
{
        ADD_CREATURE_FACTORY_FUNCTION(Revelosh);
        SP_AI_Spell spells[2];
        bool m_spellcheck[2];

        Revelosh(Creature* pCreature) : CreatureAIScript(pCreature)
        {
            nrspells = 2;
            for (uint8 i = 0; i < nrspells; i++)
            {
                m_spellcheck[i] = false;
            }

            spells[0].info = sSpellCustomizations.GetSpellInfo(10392);
            spells[0].cooldown = 10;
            spells[0].targettype = TARGET_ATTACKING;
            spells[0].instant = true;
            spells[0].perctrigger = RandomFloat(20.0f);
            spells[0].attackstoptimer = 3000;
            m_spellcheck[0] = true;

            spells[1].info = sSpellCustomizations.GetSpellInfo(2860);
            spells[1].cooldown = 10;
            spells[1].targettype = TARGET_ATTACKING;
            spells[1].instant = true;
            spells[1].perctrigger = RandomFloat(20.0f);
            spells[1].attackstoptimer = 3000;
            m_spellcheck[1] = true;
        }

        void OnCombatStart(Unit* mTarget) override
        {
            CastTime();
            RegisterAIUpdateEvent(getCreature()->GetBaseAttackTime(MELEE));
        }

        void CastTime()
        {
            for (uint8 i = 0; i < nrspells; i++)
                spells[i].casttime = spells[i].cooldown;
        }

        void OnCombatStop(Unit* mTarget) override
        {
            CastTime();
            setAIAgent(AGENT_NULL);
            getCreature()->GetAIInterface()->setAiState(AI_STATE_IDLE);
            RemoveAIUpdateEvent();
        }

        void OnDied(Unit* mKiller) override
        {
            CastTime();
        }

        void AIUpdate() override
        {
            float val = RandomFloat(100.0f);
            SpellCast(val);
        }

        void SpellCast(float val)
        {
            if (getCreature()->GetCurrentSpell() == NULL && getCreature()->GetAIInterface()->getNextTarget())
            {
                float comulativeperc = 0;
                Unit* target = NULL;
                for (uint8 i = 0; i < nrspells; i++)
                {
                    spells[i].casttime--;

                    if (m_spellcheck[i])
                    {
                        spells[i].casttime = spells[i].cooldown;
                        target = getCreature()->GetAIInterface()->getNextTarget();
                        switch (spells[i].targettype)
                        {
                            case TARGET_SELF:
                            case TARGET_VARIOUS:
                                getCreature()->CastSpell(getCreature(), spells[i].info, spells[i].instant);
                                break;
                            case TARGET_ATTACKING:
                                getCreature()->CastSpell(target, spells[i].info, spells[i].instant);
                                break;
                            case TARGET_DESTINATION:
                                getCreature()->CastSpellAoF(target->GetPosition(), spells[i].info, spells[i].instant);
                                break;
                        }

                        if (spells[i].speech != "")
                        {
                            getCreature()->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
                            getCreature()->PlaySoundToSet(spells[i].soundid);
                        }

                        m_spellcheck[i] = false;
                        return;
                    }

                    if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
                    {
                        getCreature()->setAttackTimer(spells[i].attackstoptimer, false);
                        m_spellcheck[i] = true;
                    }
                    comulativeperc += spells[i].perctrigger;
                }
            }
        }

    protected:

        uint8 nrspells;
};


class Grimlok : public CreatureAIScript
{
        ADD_CREATURE_FACTORY_FUNCTION(Grimlok);
        SP_AI_Spell spells[3];
        bool m_spellcheck[3];

        Grimlok(Creature* pCreature) : CreatureAIScript(pCreature)
        {
            nrspells = 3;
            for (uint8 i = 0; i < nrspells; i++)
            {
                m_spellcheck[i] = false;
            }

            spells[0].info = sSpellCustomizations.GetSpellInfo(8292);
            spells[0].cooldown = 10;
            spells[0].targettype = TARGET_ATTACKING;
            spells[0].instant = true;
            spells[0].perctrigger = RandomFloat(20.0f);
            spells[0].attackstoptimer = 3000;
            m_spellcheck[0] = true;

            spells[1].info = sSpellCustomizations.GetSpellInfo(10392);
            spells[1].cooldown = 10;
            spells[1].targettype = TARGET_ATTACKING;
            spells[1].instant = true;
            spells[1].perctrigger = RandomFloat(20.0f);
            spells[1].attackstoptimer = 3000;
            m_spellcheck[1] = true;

            spells[2].info = sSpellCustomizations.GetSpellInfo(8066);
            spells[2].cooldown = 10;
            spells[2].targettype = TARGET_ATTACKING;
            spells[2].instant = true;
            spells[2].perctrigger = RandomFloat(20.0f);
            spells[2].attackstoptimer = 3000;
            m_spellcheck[2] = true;
        }

        void OnCombatStart(Unit* mTarget) override
        {
            CastTime();
            RegisterAIUpdateEvent(getCreature()->GetBaseAttackTime(MELEE));
        }

        void CastTime()
        {
            for (uint8 i = 0; i < nrspells; i++)
                spells[i].casttime = spells[i].cooldown;
        }

        void OnCombatStop(Unit* mTarget) override
        {
            CastTime();
            setAIAgent(AGENT_NULL);
            getCreature()->GetAIInterface()->setAiState(AI_STATE_IDLE);
            RemoveAIUpdateEvent();
        }

        void OnDied(Unit* mKiller) override
        {
            CastTime();
        }

        void AIUpdate() override
        {
            float val = RandomFloat(100.0f);
            SpellCast(val);
        }

        void SpellCast(float val)
        {
            if (getCreature()->GetCurrentSpell() == NULL && getCreature()->GetAIInterface()->getNextTarget())
            {
                float comulativeperc = 0;
                Unit* target = NULL;
                for (uint8 i = 0; i < nrspells; i++)
                {
                    spells[i].casttime--;

                    if (m_spellcheck[i])
                    {
                        spells[i].casttime = spells[i].cooldown;
                        target = getCreature()->GetAIInterface()->getNextTarget();
                        switch (spells[i].targettype)
                        {
                            case TARGET_SELF:
                            case TARGET_VARIOUS:
                                getCreature()->CastSpell(getCreature(), spells[i].info, spells[i].instant);
                                break;
                            case TARGET_ATTACKING:
                                getCreature()->CastSpell(target, spells[i].info, spells[i].instant);
                                break;
                            case TARGET_DESTINATION:
                                getCreature()->CastSpellAoF(target->GetPosition(), spells[i].info, spells[i].instant);
                                break;
                        }

                        if (spells[i].speech != "")
                        {
                            getCreature()->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
                            getCreature()->PlaySoundToSet(spells[i].soundid);
                        }

                        m_spellcheck[i] = false;
                        return;
                    }

                    if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
                    {
                        getCreature()->setAttackTimer(spells[i].attackstoptimer, false);
                        m_spellcheck[i] = true;
                    }
                    comulativeperc += spells[i].perctrigger;
                }
            }
        }

    protected:

        uint8 nrspells;
};


class Baelog : public CreatureAIScript
{
        ADD_CREATURE_FACTORY_FUNCTION(Baelog);
        SP_AI_Spell spells[2];
        bool m_spellcheck[2];

        Baelog(Creature* pCreature) : CreatureAIScript(pCreature)
        {
            nrspells = 2;
            for (uint8 i = 0; i < nrspells; i++)
            {
                m_spellcheck[i] = false;
            }

            spells[0].info = sSpellCustomizations.GetSpellInfo(15613);
            spells[0].cooldown = 10;
            spells[0].targettype = TARGET_ATTACKING;
            spells[0].instant = true;
            spells[0].perctrigger = RandomFloat(20.0f);
            spells[0].attackstoptimer = 3000;
            m_spellcheck[0] = true;

            spells[1].info = sSpellCustomizations.GetSpellInfo(15655);
            spells[1].cooldown = 10;
            spells[1].targettype = TARGET_ATTACKING;
            spells[1].instant = true;
            spells[1].perctrigger = RandomFloat(20.0f);
            spells[1].attackstoptimer = 3000;
            m_spellcheck[1] = true;
        }

        void OnCombatStart(Unit* mTarget) override
        {
            CastTime();
            RegisterAIUpdateEvent(getCreature()->GetBaseAttackTime(MELEE));
        }

        void CastTime()
        {
            for (uint8 i = 0; i < nrspells; i++)
                spells[i].casttime = spells[i].cooldown;
        }

        void OnCombatStop(Unit* mTarget) override
        {
            CastTime();
            setAIAgent(AGENT_NULL);
            getCreature()->GetAIInterface()->setAiState(AI_STATE_IDLE);
            RemoveAIUpdateEvent();
        }

        void OnDied(Unit* mKiller) override
        {
            CastTime();
        }

        void AIUpdate() override
        {
            float val = RandomFloat(100.0f);
            SpellCast(val);
        }

        void SpellCast(float val)
        {
            if (getCreature()->GetCurrentSpell() == NULL && getCreature()->GetAIInterface()->getNextTarget())
            {
                float comulativeperc = 0;
                Unit* target = NULL;
                for (uint8 i = 0; i < nrspells; i++)
                {
                    spells[i].casttime--;

                    if (m_spellcheck[i])
                    {
                        spells[i].casttime = spells[i].cooldown;
                        target = getCreature()->GetAIInterface()->getNextTarget();
                        switch (spells[i].targettype)
                        {
                            case TARGET_SELF:
                            case TARGET_VARIOUS:
                                getCreature()->CastSpell(getCreature(), spells[i].info, spells[i].instant);
                                break;
                            case TARGET_ATTACKING:
                                getCreature()->CastSpell(target, spells[i].info, spells[i].instant);
                                break;
                            case TARGET_DESTINATION:
                                getCreature()->CastSpellAoF(target->GetPosition(), spells[i].info, spells[i].instant);
                                break;
                        }

                        if (spells[i].speech != "")
                        {
                            getCreature()->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
                            getCreature()->PlaySoundToSet(spells[i].soundid);
                        }

                        m_spellcheck[i] = false;
                        return;
                    }

                    if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
                    {
                        getCreature()->setAttackTimer(spells[i].attackstoptimer, false);
                        m_spellcheck[i] = true;
                    }
                    comulativeperc += spells[i].perctrigger;
                }
            }
        }

    protected:

        uint8 nrspells;
};


class GalgannFirehammer : public CreatureAIScript
{
        ADD_CREATURE_FACTORY_FUNCTION(GalgannFirehammer);
        SP_AI_Spell spells[3];
        bool m_spellcheck[3];

        GalgannFirehammer(Creature* pCreature) : CreatureAIScript(pCreature)
        {
            nrspells = 3;
            for (uint8 i = 0; i < nrspells; i++)
            {
                m_spellcheck[i] = false;
            }

            spells[0].info = sSpellCustomizations.GetSpellInfo(10448);
            spells[0].cooldown = 10;
            spells[0].targettype = TARGET_ATTACKING;
            spells[0].instant = true;
            spells[0].perctrigger = RandomFloat(20.0f);
            spells[0].attackstoptimer = 3000;
            m_spellcheck[0] = true;

            spells[1].info = sSpellCustomizations.GetSpellInfo(18958);
            spells[1].cooldown = 10;
            spells[1].targettype = TARGET_ATTACKING;
            spells[1].instant = true;
            spells[1].perctrigger = RandomFloat(20.0f);
            spells[1].attackstoptimer = 3000;
            m_spellcheck[1] = true;

            spells[2].info = sSpellCustomizations.GetSpellInfo(12470);
            spells[2].cooldown = 10;
            spells[2].targettype = TARGET_ATTACKING;
            spells[2].instant = true;
            spells[2].perctrigger = RandomFloat(20.0f);
            spells[2].attackstoptimer = 3000;
            m_spellcheck[2] = true;
        }

        void OnCombatStart(Unit* mTarget) override
        {
            CastTime();
            RegisterAIUpdateEvent(getCreature()->GetBaseAttackTime(MELEE));
        }

        void CastTime()
        {
            for (uint8 i = 0; i < nrspells; i++)
                spells[i].casttime = spells[i].cooldown;
        }

        void OnCombatStop(Unit* mTarget) override
        {
            CastTime();
            setAIAgent(AGENT_NULL);
            getCreature()->GetAIInterface()->setAiState(AI_STATE_IDLE);
            RemoveAIUpdateEvent();
        }

        void OnDied(Unit* mKiller) override
        {
            CastTime();
        }

        void AIUpdate() override
        {
            float val = RandomFloat(100.0f);
            SpellCast(val);
        }

        void SpellCast(float val)
        {
            if (getCreature()->GetCurrentSpell() == NULL && getCreature()->GetAIInterface()->getNextTarget())
            {
                float comulativeperc = 0;
                Unit* target = NULL;
                for (uint8 i = 0; i < nrspells; i++)
                {
                    spells[i].casttime--;

                    if (m_spellcheck[i])
                    {
                        spells[i].casttime = spells[i].cooldown;
                        target = getCreature()->GetAIInterface()->getNextTarget();
                        switch (spells[i].targettype)
                        {
                            case TARGET_SELF:
                            case TARGET_VARIOUS:
                                getCreature()->CastSpell(getCreature(), spells[i].info, spells[i].instant);
                                break;
                            case TARGET_ATTACKING:
                                getCreature()->CastSpell(target, spells[i].info, spells[i].instant);
                                break;
                            case TARGET_DESTINATION:
                                getCreature()->CastSpellAoF(target->GetPosition(), spells[i].info, spells[i].instant);
                                break;
                        }

                        if (spells[i].speech != "")
                        {
                            getCreature()->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
                            getCreature()->PlaySoundToSet(spells[i].soundid);
                        }

                        m_spellcheck[i] = false;
                        return;
                    }

                    if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
                    {
                        getCreature()->setAttackTimer(spells[i].attackstoptimer, false);
                        m_spellcheck[i] = true;
                    }
                    comulativeperc += spells[i].perctrigger;
                }
            }
        }

    protected:

        uint8 nrspells;
};


class Ironaya : public CreatureAIScript
{
        ADD_CREATURE_FACTORY_FUNCTION(Ironaya);
        SP_AI_Spell spells[2];
        bool m_spellcheck[2];

        Ironaya(Creature* pCreature) : CreatureAIScript(pCreature)
        {
            nrspells = 2;
            for (uint8 i = 0; i < nrspells; i++)
            {
                m_spellcheck[i] = false;
            }

            spells[0].info = sSpellCustomizations.GetSpellInfo(16169);
            spells[0].cooldown = 10;
            spells[0].targettype = TARGET_ATTACKING;
            spells[0].instant = true;
            spells[0].perctrigger = RandomFloat(20.0f);
            spells[0].attackstoptimer = 3000;
            m_spellcheck[0] = true;

            spells[1].info = sSpellCustomizations.GetSpellInfo(24375);
            spells[1].cooldown = 10;
            spells[1].targettype = TARGET_ATTACKING;
            spells[1].instant = true;
            spells[1].perctrigger = RandomFloat(20.0f);
            spells[1].attackstoptimer = 3000;
            m_spellcheck[1] = true;
        }

        void OnCombatStart(Unit* mTarget) override
        {
            CastTime();
            RegisterAIUpdateEvent(getCreature()->GetBaseAttackTime(MELEE));
        }

        void CastTime()
        {
            for (uint8 i = 0; i < nrspells; i++)
                spells[i].casttime = spells[i].cooldown;
        }

        void OnCombatStop(Unit* mTarget) override
        {
            CastTime();
            setAIAgent(AGENT_NULL);
            getCreature()->GetAIInterface()->setAiState(AI_STATE_IDLE);
            RemoveAIUpdateEvent();
        }

        void OnDied(Unit* mKiller) override
        {
            CastTime();
        }

        void AIUpdate() override
        {
            float val = RandomFloat(100.0f);
            SpellCast(val);
        }

        void SpellCast(float val)
        {
            if (getCreature()->GetCurrentSpell() == NULL && getCreature()->GetAIInterface()->getNextTarget())
            {
                float comulativeperc = 0;
                Unit* target = NULL;
                for (uint8 i = 0; i < nrspells; i++)
                {
                    spells[i].casttime--;

                    if (m_spellcheck[i])
                    {
                        spells[i].casttime = spells[i].cooldown;
                        target = getCreature()->GetAIInterface()->getNextTarget();
                        switch (spells[i].targettype)
                        {
                            case TARGET_SELF:
                            case TARGET_VARIOUS:
                                getCreature()->CastSpell(getCreature(), spells[i].info, spells[i].instant);
                                break;
                            case TARGET_ATTACKING:
                                getCreature()->CastSpell(target, spells[i].info, spells[i].instant);
                                break;
                            case TARGET_DESTINATION:
                                getCreature()->CastSpellAoF(target->GetPosition(), spells[i].info, spells[i].instant);
                                break;
                        }

                        if (spells[i].speech != "")
                        {
                            getCreature()->SendChatMessage(CHAT_MSG_MONSTER_YELL, LANG_UNIVERSAL, spells[i].speech.c_str());
                            getCreature()->PlaySoundToSet(spells[i].soundid);
                        }

                        m_spellcheck[i] = false;
                        return;
                    }

                    if ((val > comulativeperc && val <= (comulativeperc + spells[i].perctrigger)) || !spells[i].casttime)
                    {
                        getCreature()->setAttackTimer(spells[i].attackstoptimer, false);
                        m_spellcheck[i] = true;
                    }
                    comulativeperc += spells[i].perctrigger;
                }
            }
        }

    protected:

        uint8 nrspells;
};

void SetupUldaman(ScriptMgr* mgr)
{
    //Creatures
    mgr->register_creature_script(CN_ARCHAEDAS, &Archaedas::Create);
    mgr->register_creature_script(CN_REVELOSH, &Revelosh::Create);
    mgr->register_creature_script(CN_GRIMLOK, &Grimlok::Create);
    mgr->register_creature_script(CN_BAELOG, &Baelog::Create);
    mgr->register_creature_script(CN_GALGAN_FIREHAMMER, &GalgannFirehammer::Create);
    mgr->register_creature_script(CN_IRONAYA, &Ironaya::Create);
}
