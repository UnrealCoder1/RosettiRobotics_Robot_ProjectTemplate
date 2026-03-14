
#include "RobotBase.h"

//Just testing some functionality

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

    TimeFramework::Delay(4.0s);

    RobotBase robot;

    robot.Run();

    TimeFramework::Delay(5.0s);

    robot.Terminate();

    Utility::FOR_LOOP_WITH_DELAY(0, 10, 1, 0.1s, [](int i) {
        std::cout << "Delayed FOR\n";
    });

    int integer = 0;
    Utility::WHILE_LOOP_WITH_DELAY([&]() {return integer < 10; }, 0.1s, [&]() mutable {
        std::cout << "Delayed WHILE\n";
        integer += 1;

        std::cout << integer << "\n";
    });

    while (true)
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

}
