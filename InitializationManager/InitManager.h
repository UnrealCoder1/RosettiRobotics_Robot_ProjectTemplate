#pragma once

#include "TimeFramework.h"

#include <future>

using FrameworkList = std::initializer_list<std::tuple<TimeFramework*, bool&, ChronoDuration>>; // time, end condition, delay step

using FrameworkElement = std::tuple<TimeFramework*, bool&, ChronoDuration>;

class InitializationManager {

public:

    void Initialize_ParallelStart(FrameworkList list);

};
