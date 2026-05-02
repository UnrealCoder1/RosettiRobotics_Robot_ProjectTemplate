
#include "Ability.h"
#include "AbilityConcepts.h"

#include <iostream>

void Ability::Add_ActivationRequiredTags(std::initializer_list<std::string> tags) {
    for (std::string tag : tags)
    {
        this->ActivationRequiredTags.emplace_back(tag);
    }
}

void Ability::Add_BlockAbilityActivationTags(std::initializer_list<std::string> tags)
{
    for (std::string tag : tags)
    {
        this->BlockAbilityActivationTags.emplace_back(tag);
    }
}

bool Ability::EnsureActivation()
{
    for (std::string required_tag : ActivationRequiredTags)
    {
        if (!AbilityManager::HasTag(required_tag, AbilityOwner)) {

            return false;
        }
    }

    for (std::string block_tag : BlockAbilityActivationTags) {
        if (AbilityManager::HasTag(block_tag, AbilityOwner)) {
            return false;
        }
    }

    return true;
}
