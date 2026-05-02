#include "RobotBase.h"

#include <thread>

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

    std::cout << "Robot pod: " << Pos.to_string() << " | Delta time : " << deltaTime << " | Robot Tick |\n";
}

//!!!!!CALL THIS INSIDE THE MAIN FILE, INSIDE "int main()"!!!!! // !!!AVOID POINTERS, STACK ALLOCATIONS ONLY!!!
void RobotBase::Run()
{
    StartTimeline(&m_st, 0.0s);
}

void RobotBase::Terminate()
{
    EndTimeline(&m_stop_source);

    JoinTimeThread();

    std::cout << "========================================== Robot Ended Exec ========================================================\n";
}

//--------------------------------------------------------------------------
