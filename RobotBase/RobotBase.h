#pragma once

#include "RobotSettings.h"
#include "TimeFramework.h"

#define USE_TYPE 0

#define SCOPED_START

template<std::floating_point f_point>
using Translation = Vector2D<f_point>;

class RobotBase : public TimeFramework {

#if defined SCOPED_START
public:

    RobotBase() {
        StartTimeline(bStartBranchExecution, 0.0s);
    }
#endif

private:

#if USE_TYPE == 0
    Translation<double> Pos;
#endif

#if USE_TYPE == 1
    Translation<float> Pos;
#endif

public:

    R_Settings RobotConfig;

    bool bStartBranchExecution{ true };

public:

    template<std::floating_point f_point>
    void PauseMachineExecution(const std::chrono::duration<f_point>& duration)
    {
        bStartBranchExecution = false;

        using clock = std::chrono::steady_clock;

        auto start = clock::now();
        auto now = clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);

        if (elapsed.count() >= duration.count()) {
            bStartBranchExecution = true;
        }
    }

#if not defined SCOPED_START
    void Run();
#endif();

    void Terminate();

    using TranslationType = Translation<decltype(Pos.X)>;

    constexpr TranslationType* getRobotPos() {
        return &Pos;
    }

    void setRobotPos(const TranslationType& newPos) {
        this->Pos = newPos;
    }

    void setRobotPos(TranslationType* newPos) {
        this->Pos = *newPos;
    }

protected:

    virtual void EVENT_BeginInit() override;

    virtual void EVENT_Tick(float deltaTime) override;

};
