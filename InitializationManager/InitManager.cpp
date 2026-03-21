
#include "InitManager.h"
#include "Verifications.h"
#include <mutex>

#define FRAMEWORK 0
#define CONDITION 1
#define STEP 2

void InitializationManager::Initialize_ParallelStart(FrameworkList list)
{
    threads.reserve(list.size());

    for (FrameworkElement element : list)
    {
        threads.push_back(std::thread([&element]() {

            if (std::get<FRAMEWORK>(element) != nullptr) {

                std::get<FRAMEWORK>(element)->StartTimeline(

                    std::get<CONDITION>(element),
                    std::get<STEP>(element)

                );
            }

            })
        );
    }

    for (auto& t : threads) {
        if (t.joinable()) t.join();  // wait for threads
    }
}
