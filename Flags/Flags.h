#pragma once

#include "RobotBase.h"
#include <memory>

//Still working on it! Have to talk about config method in order to finish this system

#define __TIME_FRAMEWORK__

enum EFlags : uint16_t
{
    TimeFrameork,
    Distances,
    Angles,
    MoveConstraints
};

class FlagsCollection {
public:
    std::vector<EFlags> flags;

    std::shared_ptr<RobotBase> robot;

#ifdef __TIME_FRAMEWORK__

#endif // __TIME_FRAMEWORK__


};
