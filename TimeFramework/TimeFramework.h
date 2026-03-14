#pragma once

#include<chrono>
#include<functional>
#include "Vector2D.h"

//Everything works, besides the EndTimeline func, working on it, ignore thecommented code, trying to figure out if I can use it for something or not

#define MIN_TIME_STEP 0.03f

static float DeltaTime = 0.f;

using ChronoDuration = std::chrono::duration<float>;

class TimeFramework {

    //std::vector<std::function<void()>> Init_FunctionsToInsert;

private:

    void Tick(bool bEndCondition, ChronoDuration delay_step);

    bool bForcedEnd = false;

protected:

    virtual void EVENT_BeginInit();

    virtual void EVENT_Tick(float deltaTime);

    void StartTimeline(bool bEndCondition, ChronoDuration delay_step);

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
    inline static void Delay(std::chrono::duration<f_point> duration) {
        std::this_thread::sleep_for(duration);
    }


    static std::chrono::duration<float> GetDeltaTime();
};
