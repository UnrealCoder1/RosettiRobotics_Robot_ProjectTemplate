#include "Ability_MoveLeft.h"

#include "ExampleRobot.h"

#include <iostream>

void Ability_MoveLeft::EVENT_ActivateAbility()
{

    if(!EnsureActivation()) return;

    std::cout << "Move left ability was called\n";

    ExampleRobot* robot = static_cast<ExampleRobot*>(AbilityOwner);

    robot->setRobotPos(RobotBase::TranslationType((*robot->getRobotPos()->getX()) - 10.f, 233.f));

    std::cout << "New robot position: " << robot->getRobotPos()->to_string()  << "\n";

    AbilityEffectsManager::CommitEffect(&Effect);

    std::cout << "Robot Power: " << *std::get<ROBOT_POWER>(robot->RobotConfig.Settings)->get() << "\n";

    EndAbility();
}

void Ability_MoveLeft::EVENT_OnAbilityEnd()
{

    std::cout << "Move left ability ended\n";
}
