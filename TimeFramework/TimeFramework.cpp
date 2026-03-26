
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
    }
    
    
}

void TimeFramework::StartTimeline(bool& bEndCondition, ChronoDuration delay_step) {
    Tick(bEndCondition, delay_step);
}

void TimeFramework::EndTimeline()
{
    bForcedEnd = true;
}

void TimeFramework::EVENT_BeginInit() {
    //Implement basic functionality
}

void TimeFramework::EVENT_Tick(float delta_time) {

    worldTime.UpdateWorldDeltaTime();
}

void TimeFramework::WorldTime::UpdateWorldDeltaTime()
{
    worldTime.current_time_point = worldTime.world_clock.now();

    auto start = std::chrono::duration_cast<std::chrono::milliseconds>(worldTime.start_time_point.time_since_epoch()).count();

    auto end = std::chrono::duration_cast<std::chrono::milliseconds>(worldTime.current_time_point.time_since_epoch()).count();

    auto duration = (end - start) / 1000.f;

    std::cout << "Elapsed: " << duration << " !!/!! \n";

    DeltaTime = std::chrono::duration<float>(duration);
}
