#pragma once

#include<chrono>
#include<functional>
#include "Vector2D.h"

//Everything works, besides the EndTimeline func, working on it, ignore thecommented code, trying to figure out if I can use it for something or not

#define MIN_TIME_STEP 0.03f

using ChronoDuration = std::chrono::duration<float>;

static ChronoDuration DeltaTime = 0.0s;

class TimeFramework {

    //std::vector<std::function<void()>> Init_FunctionsToInsert;

private:

    void Tick(bool& bEndCondition, ChronoDuration delay_step);

    bool bForcedEnd = false;

protected:

    virtual void EVENT_BeginInit();

    virtual void EVENT_Tick(float deltaTime);

    void StartTimeline(bool& bEndCondition, ChronoDuration delay_step);

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

    ChronoDuration* GetDeltaTime();

public:

    enum ETimerType : uint8_t {
        SCOPED,
        TRACKED,
        LIMITED,
        ANY
    };

    template<ETimerType Type>
    class Timer {

    public:
        using HighRes_Clock = std::chrono::high_resolution_clock;

        Timer() {};

        ~Timer() {};

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
};
