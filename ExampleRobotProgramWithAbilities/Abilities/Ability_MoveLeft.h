#pragma once

#include "Ability.h"
#include "AbilityConcepts.h"
#include "RobotSettings.h"

class Ability_MoveLeft : public Ability {

public:

    explicit Ability_MoveLeft(const std::string tag, R_Settings* Settings) : Ability(tag, Settings) {

        Add_ActivationRequiredTags({ "TestRequirements" });

        Add_BlockAbilityActivationTags({ "Block" });
    }

    virtual void EVENT_ActivateAbility() override;

    virtual void EVENT_OnAbilityEnd() override;

protected:

    AbilityEffect<EAbilityEffectType::ADDER, float> Effect{"Power Increase", std::get<ROBOT_POWER>(AffectedSettingsSet->Settings), 10.f};

};
