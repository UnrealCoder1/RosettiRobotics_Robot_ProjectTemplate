#pragma once

#include "Ability.h"
#include <vector>

class AbilityCaller {

public:

    std::vector<Ability*> accessible_abilities;

    std::vector<std::string> active_abilities_tags;

};
