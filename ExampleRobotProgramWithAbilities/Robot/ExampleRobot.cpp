#include "ExampleRobot.h"
#include <string>

void ExampleRobot::EVENT_BeginInit()
{
    RobotBase::EVENT_BeginInit();

    this->active_abilities_tags.push_back("TestRequirements"); // <- needed for the ability to get executed

    //this->active_abilities_tags.push_back("Block");

    setRobotPos(RobotBase::TranslationType(

        std::get<MAPPED_SPACE>(R_SETTINGS).first.Y / 2.f,
        std::get<MAPPED_SPACE>(R_SETTINGS).second.Y / 2.f

    ));

    std::cout << getRobotPos()->to_string() << "\n";

    AbilityManager::GiveAbility(this, &ability);

    AbilityManager::TryActivateAbility_ByTag("Movement.Move_Left", this);

    AbilityManager::TryActivateAbility_ByObject(&ability, this);

    AbilityManager::TryActivateAbility_ByClass<Ability_MoveLeft>(this);

    TimeFramework::Delay(3.s);
}

void ExampleRobot::EVENT_Tick(float deltaTime)
{
    RobotBase::EVENT_Tick(deltaTime);

    std::cout << deltaTime << "\n";

    AbilityManager::TryActivateAbility_ByTag("Movement.Move_Left", this);

    AbilityManager::TryActivateAbility_ByObject(&ability, this);

    AbilityManager::TryActivateAbility_ByClass<Ability_MoveLeft>(this);

    TimeFramework::Delay(0.2s);
}
