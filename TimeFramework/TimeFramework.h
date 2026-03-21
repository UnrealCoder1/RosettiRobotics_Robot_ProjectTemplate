#pragma once

#include<chrono>
#include<functional>
#include<thread>
#include "Vector2D.h"
#include "MemTracker.h"

//Everything works, besides the EndTimeline func, working on it, ignore thecommented code, trying to figure out if I can use it for something or not

#define MIN_TIME_STEP 0.03f

using ChronoDuration = std::chrono::duration<float>;

using namespace std::chrono_literals;

static ChronoDuration DeltaTime = 0.0s;

class TimeFramework : public MemTracker{

    //std::vector<std::function<void()>> Init_FunctionsToInsert;

public:

    TimeFramework() {}

    TimeFramework(const TimeFramework& other) : MemTracker(other) {}

    TimeFramework(TimeFramework&& other) noexcept : MemTracker(std::move(other)) {}

private:

    bool bForcedEnd = false;

public:

    void StartTimeline(bool& bEndCondition, ChronoDuration delay_step);

protected:

    virtual void EVENT_BeginInit();

    virtual void EVENT_Tick(float deltaTime);

    void Tick(bool& bEndCondition, ChronoDuration delay_step);

    void EndTimeline();

    /*
    template<std::invocable Func>
    static void InsertInitFunctions(RobotBase* robot, std::initializer_list<Func> functions) {
        for (Func f : functions)
        {
            if (robot)
                *robot->Init_FunctionsToInsert.push_back(f);
            else DEBUG_BREAK();
        }
    }

    template<std::invocable Func>
    static void InsertInitFunctions(RobotBase* robot, std::vector<Func> functions) {
        for (Func f : functions)
        {
            if (robot)
                *robot->Init_FunctionsToInsert.push_back(f);
            else DEBUG_BREAK();
        }
    }
    */

public:

    template<std::floating_point f_point>
    inline static void Delay(const std::chrono::duration<f_point>& duration) {
        std::this_thread::sleep_for(duration);
    }

public:

    enum ETimerType : uint8_t {
        SCOPED,
        TRACKED,
        LIMITED,
        ANY
    };

    template<ETimerType Type>
    class Timer : public MemTracker{

    public:
        using HighRes_Clock = std::chrono::high_resolution_clock;

        Timer() {};

        ~Timer() {};

        Timer(const Timer& other) : MemTracker(other) {}

        Timer(Timer&& other) : MemTracker(std::move(other)) {}

        Timer() requires (Type == ETimerType::SCOPED || Type == ETimerType::ANY) {
            start_TimePoint = HighRes_Clock::now();
        }

        ~Timer() requires (Type == ETimerType::SCOPED || Type == ETimerType::ANY) {
            Stop();
        }

        void StartTimer() requires (Type == ETimerType::LIMITED || Type == ETimerType::ANY) {
            start_TimePoint = HighRes_Clock::now();
        }

        void TerminateTimer() requires (Type == ETimerType::LIMITED || Type == ETimerType::ANY) {
            Stop();
        }

        //Doesn't work properly:
        void SetupTimer(const ChronoDuration& trackedTime) requires (Type == ETimerType::TRACKED || Type == ETimerType::ANY) {

            static long long start;

            Utility::DO_ONCE([this]() mutable {
                start_TimePoint = HighRes_Clock::now();

                start = start_TimePoint.time_since_epoch().count();
            });

            auto now = HighRes_Clock::now();

            long long nowTime = now.time_since_epoch().count();

            auto elapsed = (nowTime - start) * 10.f;

            std::cout << "TIME:" << elapsed << "\n";

            if (elapsed >= trackedTime.count()) {
                Utility::DO_ONCE([&]() {std::cout << "TIME:" << elapsed << "\n"; });
                return;
            }

        }
        //========================================================================================================================

    private:

        void Stop()
        {
            end_TimePoint = HighRes_Clock::now();

            auto start = std::chrono::duration_cast<std::chrono::microseconds>(start_TimePoint.time_since_epoch()).count();

            auto end = std::chrono::duration_cast<std::chrono::microseconds>(end_TimePoint.time_since_epoch()).count();

            auto duration = end - start;
            double ms = duration * 0.001;

            std::cout << "Run duration: " << duration << "\n";
            std::cout << "Run duration  in milliseconds: " << ms << "\n";
        }

    private:

        std::chrono::time_point<HighRes_Clock> start_TimePoint, end_TimePoint;

    };

public:

    class WorldTime : public MemTracker{

    private:

        std::chrono::high_resolution_clock world_clock = std::chrono::high_resolution_clock();

        std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point, current_time_point;

    public:

        WorldTime() {
            start_time_point = world_clock.now();
        }

        WorldTime(const WorldTime& other) : MemTracker(other), start_time_point(other.start_time_point) {};

        WorldTime(WorldTime&& other) noexcept : MemTracker(std::move(other)), start_time_point(std::move(other.start_time_point)) {};

        static constexpr ChronoDuration* GetWorldDeltaTime() {
            return &DeltaTime;
        }

        static void UpdateWorldDeltaTime();

    };

};

static TimeFramework::WorldTime worldTime;

static inline std::vector<TimeFramework*> Time_Objects(2);
