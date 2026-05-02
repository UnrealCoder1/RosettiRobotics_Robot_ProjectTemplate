#pragma once

#include "RobotBase.h"
#include "AbilityConcepts.h"
#include "AbilityCaller.h"
#include "Ability_MoveLeft.h"

class ExampleRobot : public RobotBase, public AbilityCaller {

private:

    Ability_MoveLeft ability{ "Movement.Move_Left", &RobotConfig };

public:

    virtual void EVENT_BeginInit() override;

    virtual void EVENT_Tick(float deltaTime) override;

};
