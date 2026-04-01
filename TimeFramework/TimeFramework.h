#pragma once

#include<chrono>
#include<functional>
#include<thread>
#include<type_traits>
#include<string>

#include "Vector2D.h"
#include "MemTracker.h"
#include "CollectionDefaults.h"

//Everything works, besides the EndTimeline func, working on it

#define FIND_VAR_NAME(x) #x

#define MIN_TIME_STEP 0.03f

using ChronoDuration = std::chrono::duration<float>;

using namespace std::chrono_literals;

static float DeltaTime = 0.0f;

static inline int ID_Counter;

class TimeFramework; // <- Forwarding declaration

static inline std::vector<TimeFramework> Time_Objects;

class TimeFramework : public MemTracker{

public:

    TimeFramework() {
        ID = ID_Counter;

        ID_Counter += 1;

        name = ("Timeline NAME_NONE_" + std::to_string(this->ID));
    }

    TimeFramework(const TimeFramework& other, const std::string& name) : MemTracker(other), name(name) {
        ID = ID_Counter;

        ID_Counter += 1;
    }

    TimeFramework(const TimeFramework& other) : MemTracker(other){
        ID = ID_Counter;

        ID_Counter += 1;

        name = "Timeline NAME_NONE_" + std::to_string(this->ID);
    }

private:

    std::string name;

    int ID;

    bool bForcedEnd = false;

public:

    friend std::ostream& operator<<(std::ostream& os, const TimeFramework& tf) {
        os << "Timeline ID: " << tf.ID << " | Timeline NAME: " << tf.name; // Use tf.ID to access the instance data
        return os;
    }

    bool operator==(const TimeFramework& other) const {
        return this->name == other.name;
    }

    bool operator>(const TimeFramework& other) const {
        return this->ID > other.ID;
    }

    bool operator>(const int& other) const {
        return this->ID > other;
    }

    bool operator<(const TimeFramework& other) const {
        return this->ID < other.ID;
    }

    bool operator<(const int& other) const {
        return this->ID < other;
    }

    bool operator>=(const TimeFramework& other) const {
        return this->ID >= other.ID;
    }

    bool operator>=(const int& other) const {
        return this->ID >= other;
    }

    bool operator<=(const TimeFramework& other) const {
        return this->ID <= other.ID;
    }

    bool operator<=(const int& other) const {
        return this->ID <= other;
    }

public:

    void StartTimeline(bool& bEndCondition, ChronoDuration delay_step);

protected:

    virtual void EVENT_BeginInit();

    virtual void EVENT_Tick(float deltaTime);

    void Tick(bool& bEndCondition, ChronoDuration delay_step);

    void EndTimeline();

public:

    template<std::floating_point f_point>
    inline static void Delay(const std::chrono::duration<f_point>& duration) {
        std::this_thread::sleep_for(duration);
    }

    int* getID() {
        return &ID;
    }

public:

    enum ETimerType : uint8_t {
        SCOPED,
        TRACKED,
        LIMITED,
        TASK_TRACKER,
        ANY
    };

    enum class EObservationMode : uint8_t{
        CONTINOUS_CODE_IMPLEMENTATION,
        PARALLEL_CODE_IMPLEMENTATION
    //    EXPLICITLY_PARALLEL  <- optional, thinking of further development
    };

    template<ETimerType Type>
    class Timer : public MemTracker{

    public:
        using HighRes_Clock = std::chrono::high_resolution_clock;

        Timer() {};

        ~Timer() {};

        Timer(const Timer& other) : MemTracker(other) {}

        Timer(Timer&& other) : MemTracker(std::move(other)) {}

        template<typename Func> requires (std::is_invocable_v<Func>)
        Timer(Func&& func) requires (Type == ETimerType::TASK_TRACKER) {
            StartFunctionTrack(func);
        }

        Timer() requires (Type == ETimerType::SCOPED || Type == ETimerType::ANY) {
            start_TimePoint = HighRes_Clock::now();
        }

        ~Timer() requires (Type == ETimerType::SCOPED || Type == ETimerType::ANY) {
            Stop();
        }

        void StartTimer() requires (Type == ETimerType::LIMITED || Type == ETimerType::ANY) {
            start_TimePoint = HighRes_Clock::now();
        }

        void TerminateTimer() requires (Type == ETimerType::LIMITED || Type == ETimerType::ANY) {
            Stop();
        }

        void SetupTimer(const ChronoDuration& trackedTime, EObservationMode observationMode ) requires (Type == ETimerType::TRACKED || Type == ETimerType::ANY) {

            static long long start;

            Utility::DO_ONCE([&]() mutable {
                start_TimePoint = HighRes_Clock::now();

                start = std::chrono::duration_cast<std::chrono::microseconds>(start_TimePoint.time_since_epoch()).count();
            });

            static long long elapsed_time, now;

            switch (observationMode)
            {
            case TimeFramework::EObservationMode::CONTINOUS_CODE_IMPLEMENTATION :

                while (elapsed_time < (trackedTime.count() * 1000.f))
                {
                    now = std::chrono::duration_cast<std::chrono::microseconds>(HighRes_Clock::now().time_since_epoch()).count();

                    elapsed_time = (now - start) / 1000;
                }

                end_TimePoint = HighRes_Clock::now();

                std::cout << "Timer ended: " << elapsed_time << " | Target time: " << trackedTime.count() * 1000.f << "\n";

                break;

            case TimeFramework::EObservationMode::PARALLEL_CODE_IMPLEMENTATION :

                now = std::chrono::duration_cast<std::chrono::microseconds>(HighRes_Clock::now().time_since_epoch()).count();

                elapsed_time = (now - start) / 1000;

                if (elapsed_time >= (trackedTime.count() * 1000.f)) {

                    end_TimePoint = HighRes_Clock::now();

                    std::cout << "Timer ended: " << elapsed_time << " | Target time: " << trackedTime.count() * 1000.f << "\n";
                }

                break;
            default:
                break;
            }

        }

        template<typename Func> requires (std::is_invocable_v<Func>)
        auto StartFunctionTrack(Func&& func) requires (Type == ETimerType::TASK_TRACKER){
            start_TimePoint = HighRes_Clock::now();

            auto start = std::chrono::duration_cast<std::chrono::milliseconds>(start_TimePoint.time_since_epoch());

            func();

            end_TimePoint = HighRes_Clock::now();

            auto end = std::chrono::duration_cast<std::chrono::milliseconds>(end_TimePoint.time_since_epoch());

            auto elapsed = end - start;

            std::cout << "Time PASSED: " << elapsed << "\n";

            return elapsed;
        }

    private:

        void Stop()
        {
            end_TimePoint = HighRes_Clock::now();

            auto start = std::chrono::duration_cast<std::chrono::microseconds>(start_TimePoint.time_since_epoch()).count();

            auto end = std::chrono::duration_cast<std::chrono::microseconds>(end_TimePoint.time_since_epoch()).count();

            auto duration = end - start;
            double ms = duration * 0.001;

            std::cout << "Run duration: " << duration << "\n";
            std::cout << "Run duration  in milliseconds: " << ms << "\n";
        }

    private:

        std::chrono::time_point<HighRes_Clock> start_TimePoint, end_TimePoint;

    };

public:

    class WorldTime : public MemTracker{

    private:

        std::chrono::high_resolution_clock world_clock = std::chrono::high_resolution_clock();

        std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point, current_time_point;

    public:

        WorldTime() {
            start_time_point = world_clock.now();
        }

        WorldTime(const WorldTime& other) : MemTracker(other), start_time_point(other.start_time_point) {};

        static constexpr float* GetWorldDeltaTime() {
            return &DeltaTime;
        }

        static void UpdateWorldDeltaTime(float& var);

    };

};

class ObjectsManager : public CollectionDefaults<TimeFramework> {

public:

    ObjectsManager() {};

    static bool containsElement(const TimeFramework& element) {
        return CollectionDefaults<TimeFramework>::ContainsElement(element, Time_Objects);
    }

    static TimeFramework* findElementAtIndex(const size_t& index) {
        return CollectionDefaults<TimeFramework>::FindElementAtIndex(index, Time_Objects);
    }

    static size_t findIndexOfElement(const TimeFramework& element) {
        return CollectionDefaults<TimeFramework>::FindIndexOfElement(element, Time_Objects);
    }

    /*
    static void sort_ascending() {
        CollectionDefaults<TimeFramework>::Sort_Ascending(Time_Objects);
    }

    static void sort_descending() {
        CollectionDefaults<TimeFramework>::Sort_Descending(Time_Objects);
    }

    template<Func CompFunction>
    static void sort(CompFunction&& comparisonFunction) {
        CollectionDefaults<TimeFramework>::Sort(Time_Objects, comparisonFunction);
    }
    */

    static void printCollection() {

        std::cout << "==== OBJECTS MANAGER PRINT ====\n";

        CollectionDefaults<TimeFramework>::PrintCollection(Time_Objects);
    }

};

static inline TimeFramework::WorldTime worldTime;

