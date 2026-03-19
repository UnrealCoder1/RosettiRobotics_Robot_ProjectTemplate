
#include "TimeFramework.h"
#include <thread>
#include <algorithm>

using namespace std::chrono_literals;

void TimeFramework::Tick(bool& bEndCondition, ChronoDuration delay_step) {

    while (bEndCondition)
    {
        if (bForcedEnd) break;
    
        Utility::DO_ONCE([=]() {EVENT_BeginInit(); });
    
        EVENT_Tick(DeltaTime.count());
    
        if (delay_step != 0.0s) {
            std::this_thread::sleep_for(delay_step);
        }
    
        DeltaTime += 0.1s;
    }
    
}

void TimeFramework::StartTimeline(bool& bEndCondition, ChronoDuration delay_step) {
    Tick(bEndCondition, delay_step);
}

void TimeFramework::EndTimeline()
{
    bForcedEnd = true;
}

ChronoDuration* TimeFramework::GetDeltaTime() {
    return &DeltaTime;
}

void TimeFramework::EVENT_BeginInit() {
    //Implement basic functionality
}

void TimeFramework::EVENT_Tick(float delta_time) {
    //Implement basic functionality
}


