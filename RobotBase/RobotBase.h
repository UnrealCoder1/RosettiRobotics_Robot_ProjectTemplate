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

    RobotBase() {}

    RobotBase(const RobotBase& other) : TimeFramework(other) {};

    RobotBase(RobotBase&& other) noexcept : TimeFramework(std::move(other)) {};

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

protected:

    virtual void EVENT_BeginInit() override;

    virtual void EVENT_Tick(float deltaTime) override;

};
