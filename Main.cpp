
#include "RobotBase.h"
#include "InitManager.h"
#include "Verifications.h"
#include "Reevaluation.h"

//Just testing some functionality

static float local_DeltaTime = 0.f;

class Worker {
public:
    void work() {
        Utility::FOR_LOOP_WITH_DELAY(0, 2, 1, 0.3s, [](int index) {
            std::cout << "Working" << index << "\n";
        });
    }
};

int main() 
{

    int* ptr = new int(10);

    int* ptr1 = Verifications::EnsureIsValid(ptr);

    int it = 10;

    Verifications::CheckIsValid(it);

    std::cout << ptr1 << "\n";

    ReEvaluation::T_GuardValue<float> GFloat = 10.f;

    std::cout << GFloat.toString() << "\n";

    GFloat = 100.f;

    std::cout << "Current value: " << GFloat.toString() << " | " << "Tracked value: " << *GFloat.GetTrackedValue() << "\n";

    GFloat = 32.f;
    GFloat.SetTrackedValue(47.f);

    std::cout << "Current value: " << GFloat.toString() << " | " << "Tracked value: " << *GFloat.GetTrackedValue() << "\n";

    GFloat.ReInitialize();

    std::cout << "Current value: " << GFloat.toString() << " | " << "Tracked value: " << *GFloat.GetTrackedValue() << "\n";

    ReEvaluation::T_GuardValue<float> GFloat1{ 2.f }, GFloat2{0.f};

    GFloat2 = GFloat + GFloat1;
    GFloat2 = GFloat * GFloat1;
    GFloat2 = GFloat - GFloat1;
    GFloat2 = GFloat / GFloat1;

    std::cout << GFloat2.toString() << "\n";

    ReEvaluation::T_HigherGuard<float> HigherG_Float(10, 3);

    for (int i = 1; i <= 7; i++)
    {

        std::cout << HigherG_Float.toString();

        if (i == 2)
            std::cout << "Idx of elem 10: " << HigherG_Float.findIndexOfElement(10) << "\n";


        if (i == 4) {
            std::cout << "Current value: " << *HigherG_Float.getCurrentValue() << " , Guarded value: " << *HigherG_Float.getGuardedValue() << " , Tracked index: " << *HigherG_Float.getTrackedIndex() << "\n";
        }

        if (i == 2) {
            std::cout << "Elem at index 0: " << *HigherG_Float.findElementAtIndex(0) << "\n";
        }


        HigherG_Float.printSavesList();

        HigherG_Float = i;

        if (i == 3) {
            HigherG_Float.setTrackedValue(0);
        }
    }

    std::cout << *HigherG_Float.getTrackedIndex();

	Vector2D<float> vec1(-5.f,5.f), vec2(2.f, 3.f);

	Vector2D<double> vec3(4.f, 6.f);

    vec3 = vec3 * 3.f;

    Vector2D angleVec = Vector2D<float>::angleToVector(55.f);

    std::optional<Vector2D<float>> vec_intersect = Vector2D<float>::getIntersectionPoint(vec1, Vector2D<float>::ZeroVector(), vec2, Vector2D<float>::ZeroVector());

    if(vec_intersect)
	    std::cout << vec_intersect->to_string() << "\n";

    static bool bReset = true;

    Worker worker;

    worker.work();

    TimeFramework::Delay(1.0s);
    {
        TimeFramework::Timer<TimeFramework::SCOPED> timer;

        std::cout << "angleToVector: " << Vector2D<float>::angleToVector(90.f).to_string() << "\n";

        std::cout << "cross: " << Vector2D<float>::cross(Vector2D<float>(1.3f, 2.5f), Vector2D<float>(1.f, 2.f)) << "\n";

        std::cout << "dot: " << Vector2D<float>::dot(Vector2D<float>(1.3f, 2.5f), Vector2D<float>(1.f, 2.f)) << "\n";

        std::cout << "getAngle_Degrees: " << Vector2D<float>::getAngle_Degrees(Vector2D<float>(0.f, 2.5f), Vector2D<float>(1.f, 2.f)) << "\n";

        std::cout << "getDistance: " << Vector2D<float>::getDistance(Vector2D<float>(1.3f, 2.5f), Vector2D<float>(1.f, 2.f)) << "\n";

        std::cout << "getAngle_Radians: " << Vector2D<float>::getAngle_Radians(Vector2D<float>(1.3f, 2.5f), Vector2D<float>(1.f, 2.f)) << "\n";

        std::cout << "getLength: " << Vector2D<float>::getLength(Vector2D<float>(1.3f, 2.5f)) << "\n";

        std::cout << "getPerpendicularDistanceToLine: " << Vector2D<float>::getPerpendicularDistanceToLine(Vector2D<float>(1.3f, 2.5f), Vector2D<float>(1.f, 2.f), Vector2D<float>()) << "\n";
    }

    TimeFramework::Timer<TimeFramework::LIMITED> trackedTimer;

    TimeFramework::Delay(3.0s);

    trackedTimer.StartTimer();

    Utility::FOR_LOOP_WITH_DELAY(0, 10, 1, 0.1s, [](int i) {
        std::cout << "Delayed FOR\n";
    });

    int integer = 0;
    Utility::WHILE_LOOP_WITH_DELAY([&]() {return integer < 10; }, 0.1s, [&]() mutable {
        std::cout << "Delayed WHILE\n";
        integer += 1;

        std::cout << integer << "\n";
    });

    trackedTimer.TerminateTimer();

    InitializationManager initManager;

    while (true)
    {
        local_DeltaTime += 0.01f;

        Utility::STATIC_SWITCH<std::function<void()>>({
            {
                []() {return false; },
                []() {std::cout << Vector2D<float>::mapRangeUnclamped(local_DeltaTime, 0.f, 5.f, 0.f, 1000.f) << "\n"; }
            },
            {
                []() {return false; },
                []() {std::cout << Vector2D<float>::mapRangeClamped(local_DeltaTime, 0.f, 5.f, 0.f, 1000.f) << "\n"; }
            },
            {
                []() {return false; },
                []()
                {
                    std::vector<std::function<void()>> fs = {
                        []() { std::this_thread::sleep_for(0.2s); std::cout << "First\n"; },
                        []() { std::this_thread::sleep_for(0.2s); std::cout << "Second\n"; },
                        []() { std::this_thread::sleep_for(0.2s); std::cout << "Third\n"; }
                    };

                    Utility::MULTICALL<std::function<void()>>({
                        []() { std::this_thread::sleep_for(0.2s); std::cout << "First\n"; },
                        []() { std::this_thread::sleep_for(0.2s); std::cout << "Second\n"; },
                        []() { std::this_thread::sleep_for(0.2s); std::cout << "Third\n"; }
                    });
                }
            },
            {
                []() {return false; },
                []()
                {
                    static float var(0.f);

                    TimeFramework::Delay(0.1s);

                    std::cout << Vector2D<float>::Num_InterpTo(var, 100.f, local_DeltaTime, 1.3f) << "\n";
                }
            },
            {
                []() {return false; },
                []()
                {
                    static Vector2D<float> var{1.3f, 23.f};

                    TimeFramework::Delay(0.1s);

                    std::cout << Vector2D<float>::Vec_InterpTo(var, Vector2D<float>(100.f), local_DeltaTime, 1.3f).to_string() << "\n";
                }
            },
            {
                []() {return true; },
                [&initManager]()
                {
                    RobotBase robot, robot1;

                    static bool cond1 = true, cond2 = true;

                    Utility::DO_ONCE([&]() {
                        initManager.Initialize_ParallelStart({
                            {&robot, cond1, 0.1s},
                            {&robot1, cond2, 0.1s}
                        });
                    });
                }
            }
        });
    }

}
