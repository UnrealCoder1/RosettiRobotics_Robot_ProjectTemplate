
#include "InitManager.h"

#include <thread>

#define FRAMEWORK 0
#define CONDITION 1
#define STEP 2

void InitializationManager::Initialize_ParallelStart(FrameworkList list)
{
    std::vector<std::thread> threads;

    threads.reserve(2);

    for (FrameworkElement element : list)
    {
        threads.push_back(std::thread([&element]() {

            if (std::get<FRAMEWORK>(element) == nullptr) {

                std::get<FRAMEWORK>(element)->StartTimeline(

                    std::get<CONDITION>(element),
                    std::get<STEP>(element)

                );
            }

            })
        );
    }

    for (int i{}; i < threads.size(); i++) {
        threads[i].join();
    }
}
