#include "RobotBase.h"

#define R_SETTINGS RobotConfig.Settings

using namespace std;

void RobotBase::EVENT_BeginInit()
{
    if (!get<USE_INIT>(R_SETTINGS)) return;

    TimeFramework::EVENT_BeginInit();

    std::cout << "Robot INIT!\n";
}

void RobotBase::EVENT_Tick(float deltaTime)
{
    TimeFramework::EVENT_Tick(deltaTime);

    Pos = Vector2D<decltype(Pos.X)>{
            std::clamp(
                Pos.X, get<MAPPED_SPACE>(R_SETTINGS).first.X,
                get<MAPPED_SPACE>(R_SETTINGS).first.Y
            ),

            std::clamp(
                Pos.Y, get<MAPPED_SPACE>(R_SETTINGS).second.X,
                get<MAPPED_SPACE>(R_SETTINGS).second.Y
            )
    };

    TimeFramework::Delay(0.1s);

    std::cout << "Robot pod: " << Pos.to_string() << " | Delta time : " << deltaTime << " | Robot Tick |\n";
}

//!!!!!CALL THIS INSIDE THE MAIN FILE, INSIDE "int main()"!!!!! // !!!AVOID POINTERS, STACK ALLOCATIONS ONLY!!!
void RobotBase::Run()
{
    StartTimeline(bStartBranchExecution, 0.0s);
}

void RobotBase::Terminate()
{
    EndTimeline();
    bStartBranchExecution = false;
    DeltaTime = 0.0s;
}

//--------------------------------------------------------------------------
