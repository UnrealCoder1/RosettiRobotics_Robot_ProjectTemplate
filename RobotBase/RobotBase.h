#pragma once

#include "RobotSettings.h"
#include "TimeFramework.h"

#include <stop_token>

#include <cmath>

#define USE_TYPE 0

#define R_SETTINGS RobotConfig.Settings

template<std::floating_point f_point>
using Translation = Vector2D<f_point>;

class RobotBase : public TimeFramework {

private:

    std::stop_source m_stop_source;

    std::stop_token m_st = m_stop_source.get_token();

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

public:

    template<std::floating_point f_point>
    void PauseMachineExecution(const std::chrono::duration<f_point>& duration)
    {
        using clock = std::chrono::steady_clock;

        auto start = clock::now();
        auto now = clock::now();

        auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start);
    }

    void Run();

    void Terminate();

    using TranslationType = Translation<decltype(Pos.X)>;

    constexpr TranslationType* getRobotPos() {
        return &Pos;
    }

    void setRobotPos(const TranslationType& newPos) {
        this->Pos = Vector2D<decltype(Pos.X)>{
            std::clamp(
                newPos.X, get<MAPPED_SPACE>(R_SETTINGS).first.X,
                get<MAPPED_SPACE>(R_SETTINGS).first.Y
            ),
            
            std::clamp(
                newPos.Y, get<MAPPED_SPACE>(R_SETTINGS).second.X,
                get<MAPPED_SPACE>(R_SETTINGS).second.Y
            )
        };
    }

    void setRobotPos(TranslationType* newPos) {
        this->Pos = Vector2D<decltype(Pos.X)>{
        std::clamp(
            newPos->X, get<MAPPED_SPACE>(R_SETTINGS).first.X,
            get<MAPPED_SPACE>(R_SETTINGS).first.Y
        ),

        std::clamp(
            newPos->Y, get<MAPPED_SPACE>(R_SETTINGS).second.X,
            get<MAPPED_SPACE>(R_SETTINGS).second.Y
        )
        };
    }

protected:

    virtual void EVENT_BeginInit() override;

    virtual void EVENT_Tick(float deltaTime) override;

};
