#pragma once

#include "RobotSettings.h"
#include "TimeFramework.h"

#define USE_TYPE 0

template<std::floating_point f_point>
using Translation = Vector2D<f_point>;

class RobotBase : public TimeFramework {

private:

#if USE_TYPE == 0
    Translation<double> Pos;
#endif

#if USE_TYPE == 1
    Translation<float> Pos;
#endif

public:

    R_Settings RobotConfig;

    bool bStopBranchExecution{ false };

public:

    template<std::floating_point f_point>
    void PauseMachineExecution(volatile std::chrono::duration<f_point>* duration)
    {
        bStopBranchExecution = true;

        using clock = std::chrono::steady_clock;

        auto start = clock::now();
        auto now = clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);

        if (elapsed.count() >= duration) {
            bStopBranchExecution = false;
        }
    }

    void Run();

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

    void setRobotPos(TranslationType newPos) {
        this->Pos = newPos;
    }

protected:

    virtual void EVENT_BeginInit() override;

    virtual void EVENT_Tick(float deltaTime) override;

};
