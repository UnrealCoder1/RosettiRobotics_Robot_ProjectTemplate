#pragma once

#include <string>
#include <vector>
#include <initializer_list>

class AbilityCaller;

class R_Settings;

class Ability {

    std::string ability_tag;

    std::vector<std::string> ActivationRequiredTags;

    std::vector<std::string> BlockAbilityActivationTags;

protected:

    void EndAbility(){
        EVENT_OnAbilityEnd();
    }

protected:

    void Add_ActivationRequiredTags(std::initializer_list<std::string> tags);

    void Add_BlockAbilityActivationTags(std::initializer_list<std::string> tags);

    bool EnsureActivation();

public:

    AbilityCaller* AbilityOwner = nullptr;

    R_Settings* AffectedSettingsSet;

public:

    Ability(const std::string& tag, R_Settings* AffectedSettingsSet) : ability_tag(tag), AffectedSettingsSet(AffectedSettingsSet) {}

    Ability(const char* tag, R_Settings* AffectedSettingsSet) : ability_tag(tag), AffectedSettingsSet(AffectedSettingsSet) {}

public:

    bool operator==(const Ability& other) {
        return this->ability_tag == other.ability_tag;
    }

public:

    std::string getAbilityTag() const noexcept {
        return ability_tag;
    }

    virtual void EVENT_ActivateAbility() = 0;// pure function

    virtual void EVENT_OnAbilityEnd() = 0; // pure function

};
