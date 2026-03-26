#pragma once

#include<concepts>
#include<string>
#include<deque>
#include<numbers>
#include<sstream>
#include<algorithm>
#include"MemTracker.h"
#include"CollectionDefaults.h"

#define DEFAULT_NUMBER_OF_MEMORIES 3

class ReEvaluation {

public:

    template<typename T>
    class T_GuardValue {

    protected:
        T trackedValueCopy;

        T trackedValue;

    public:

        T_GuardValue() : trackedValue(NULL), trackedValueCopy(trackedValue) {}

        T_GuardValue(const T& value) {
            this->trackedValueCopy = value;
            this->trackedValue = value;
        }

        T_GuardValue(const T_GuardValue& other) : MemTracker(other) {
            this->trackedValue = other.trackedValue;

            this->trackedValueCopy = other.trackedValueCopy;
        }

        T_GuardValue(T_GuardValue&& other) noexcept : MemTracker(std::move(other)) {
            this->trackedValue = std::move(other.trackedValue);

            this->trackedValueCopy = std::move(other.trackedValueCopy);
        }

        constexpr T* GetTrackedValue() {
            return &trackedValueCopy;
        }

        constexpr void SetTrackedValue(const T& newTrackedValue) {
            this->trackedValueCopy = newTrackedValue;
        }

        constexpr void ReInitialize() {
            this->trackedValue = trackedValueCopy;
        }

        const T_GuardValue& operator=(const T& other) {
            this->trackedValue = other;

            return *this;
        }

        T_GuardValue operator+(const T_GuardValue& other) {
            return T_GuardValue{ this->trackedValue + other.trackedValue };
        }

        T_GuardValue operator+(const T& other) {
            return T_GuardValue{ this->trackedValue + other };
        }

        T_GuardValue operator-(const T_GuardValue& other) {
            return T_GuardValue{ this->trackedValue + other.trackedValue };
        }

        T_GuardValue operator-(const T& other) {
            return T_GuardValue{ this->trackedValue + other };
        }

        T_GuardValue operator*(const T_GuardValue& other) {
            return T_GuardValue{ this->trackedValue * other.trackedValue };
        }

        T_GuardValue operator*(const T& other) {
            return T_GuardValue{ this->trackedValue * other };
        }

        T_GuardValue operator/(const T_GuardValue& other) {
            return T_GuardValue{ this->trackedValue / other.trackedValue };
        }

        T_GuardValue operator/(const T& other) {
            return T_GuardValue{ this->trackedValue / other };
        }

        constexpr bool operator==(const T_GuardValue& other) const {
            return this->trackedValue == other.trackedValue;
        }

        constexpr bool operator==(const T& other) const {
            return this->trackedValue == other;
        }

        std::string toString() const {
            return std::to_string(trackedValue);
        }

        T_GuardValue& operator=(const T_GuardValue& other) {
            trackedValue = other.trackedValue;

            trackedValueCopy = other.trackedValueCopy;

            return *this;
        }
    };

public:

    template<typename T>
    class T_HigherGuard : public MemTracker, CollectionDefaults<T> {

    private:

        uint8_t maxNumberOfMemories;

        std::deque<T> memories;

        T currentValue;

        T guardedValue;

        size_t trackedIndex{};

    public:

        T_HigherGuard() {}

        T_HigherGuard(const T& value, const uint8_t& maxNumberOfMemories) {

            this->maxNumberOfMemories = maxNumberOfMemories;

            this->memories.push_front(value);

            this->guardedValue = memories[0];
            this->currentValue = value;

            trackedIndex = 0;
        }

        T_HigherGuard(const T& value) {
            this->maxNumberOfMemories = DEFAULT_NUMBER_OF_MEMORIES;

            this->memories.push_front(value);

            this->guardedValue = memories[0];
            this->currentValue = value;

            trackedIndex = 0;
        }

        T_HigherGuard(const T_HigherGuard& value, const uint8_t& maxNumberOfMemories) : MemTracker(value) {

            this->maxNumberOfMemories = maxNumberOfMemories;

            this->memories.push_front(value.currentValue);

            this->guardedValue = memories[0];
            this->currentValue = value.currentValue;

            trackedIndex = 0;
        }

        T_HigherGuard(const T_HigherGuard& value) : MemTracker(value) {
            this->maxNumberOfMemories = DEFAULT_NUMBER_OF_MEMORIES;

            this->memories.push_front(value.currentValue);

            this->guardedValue = memories[0];
            this->currentValue = value.currentValue;

            trackedIndex = 0;
        }

        T_HigherGuard(T_HigherGuard&& value, const uint8_t& maxNumberOfMemories) : MemTracker(std::move(value)) {

            this->maxNumberOfMemories = maxNumberOfMemories;

            this->memories.push_front(std::move(value.currentValue));

            this->guardedValue = memories[0];
            this->currentValue = std::move(value.currentValue);

            trackedIndex = 0;
        }

        T_HigherGuard(T_HigherGuard&& value) : MemTracker(std::move(value)) {

            this->maxNumberOfMemories = DEFAULT_NUMBER_OF_MEMORIES;

            this->memories.push_front(std::move(value.currentValue));

            this->guardedValue = memories[0];
            this->currentValue = std::move(value.currentValue);

            trackedIndex = 0;
        }

        const T_HigherGuard& operator=(const T_HigherGuard& other) {
            this->currentValue = other.currentValue;

            if (memories.size() == maxNumberOfMemories) {

                this->memories.pop_back();
            }

            this->memories.push_front(other.currentValue);

            if (containsElement(guardedValue)) {
                trackedIndex = findIndexOfElement(guardedValue);
            }
            else
            {
                trackedIndex = 0;

                this->guardedValue = memories[0];
            }

            return *this;
        }

        const T_HigherGuard& operator=(const T& other) {
            this->currentValue = other;

            if (memories.size() == maxNumberOfMemories) {

                this->memories.pop_back();
            }

            this->memories.push_front(other);

            if (containsElement(guardedValue)) {
                trackedIndex = findIndexOfElement(guardedValue);
            }
            else
            {
                trackedIndex = 0;

                this->guardedValue = memories[0];
            }

            return *this;
        }

        constexpr bool operator==(const T_HigherGuard& other) {
            return this->currentValue == other.currentValue;
        }

        constexpr bool operator==(const T& other) {
            return this->currentValue = other;
        }

        T_HigherGuard operator+(const T_HigherGuard& other) {
            return T_HigherGuard{ this->currentValue + other.currentValue };
        }

        T_HigherGuard operator+(const T& other) {
            return T_HigherGuard{ this->currentValue + other };
        }

        T_HigherGuard operator-(const T_HigherGuard& other) {
            return T_HigherGuard{ this->currentValue + other.currentValue };
        }

        T_HigherGuard operator-(const T& other) {
            return T_HigherGuard{ this->currentValue + other };
        }

        T_HigherGuard operator*(const T_HigherGuard& other) {
            return T_HigherGuard{ this->currentValue * other.currentValue };
        }

        T_HigherGuard operator*(const T& other) {
            return T_HigherGuard{ this->currentValue * other };
        }

        T_HigherGuard operator/(const T_HigherGuard& other) {
            return T_HigherGuard{ this->currentValue / other.currentValue };
        }

        T_HigherGuard operator/(const T& other) {
            return T_HigherGuard{ this->currentValue / other };
        }

        constexpr void setTrackedValue(const uint8_t& newTrackedValueIndex) {

            if (newTrackedValueIndex > (memories.size() - 1)) {

                std::cout << "!%! Invalid input index !%!\n";

                DEBUG_BREAK();
            }

            this->trackedIndex = newTrackedValueIndex;

            this->guardedValue = memories[newTrackedValueIndex];
        }

        size_t findIndexOfElement(const T& element) {

            return this->FindIndexOfElement(element, memories);
        }

        T* findElementAtIndex(const size_t& index) {

            return this->FindElementAtIndex(index, memories);
        }

        bool containsElement(const T& element) {
            return this->ContainsElement(element, memories);
        }

        constexpr size_t* getTrackedIndex() {
            return &trackedIndex;
        }

        constexpr T* getGuardedValue() {
            return &guardedValue;
        }

        constexpr T* getCurrentValue() {
            return &currentValue;
        }

        constexpr std::deque<T>* getSaveList() {
            return &memories;
        }

        constexpr T* getFrontElement() {
            return &memories[0];
        }

        constexpr T* getBackElement() {
            return &memories[memories.size() - 1];
        }

        constexpr void setFrontAsGuardedValue() {
            this->guardedValue = memories.front();

            this->trackedIndex = 0;
        }

        constexpr void setBackAsGuardedValue() {
            this->guardedValue = memories.back();

            this->trackedIndex = memories.size() - 1;
        }

        void printSavesList() {
            this->PrintCollection(memories);
        }

        void PrintGuard() {
            std::cout << "====FIXED GUARD PRINT====\n";
            std::cout << "Current value: " << currentValue << " | Tracked index: " << trackedIndex << " | Guarded value : " << guardedValue << "\n";
            std::cout << "----MEMORIES----\n";
            for (T var : memories)
            {
                std::cout << var << "\n";
            }
            std::cout << "----------------\n";
            std::cout << "=========================\n";
        }

        void ReInitialize() {
            this->currentValue = this->guardedValue;
        }

        std::string toString() {
            std::ostringstream oss;

            oss << "| Guarded value: " << guardedValue
                << " | Tracked Index: " << trackedIndex
                << " | Current value: " << currentValue
                << " |\n";

            return oss.str();
        }
    };

public:

    enum EFixedPositionType : uint8_t {
        FRONT,
        BACK
    };

    template<typename T>
    class T_FixedGuard : public CollectionDefaults<T>{ 

    private:

        T currentValue;

        T guardedValue;

        uint16_t containerMaxSize;

        size_t trackedIndex;

        std::deque<T> memories;

        EFixedPositionType PostionType = EFixedPositionType::FRONT;

    public:

        T_FixedGuard(const T& value, EFixedPositionType positionType, uint16_t max_size) : currentValue(value), guardedValue(value), PostionType(positionType), containerMaxSize(max_size) {
            memories.push_front(value);

            trackedIndex = 0;
        }

        T_FixedGuard(const T& value) : currentValue(value), guardedValue(value), trackedIndex(0), containerMaxSize(DEFAULT_NUMBER_OF_MEMORIES), PostionType(EFixedPositionType::FRONT) {
            memories.push_front(value);
        }

    public:

        T_FixedGuard operator=(const T_FixedGuard& other) {

            currentValue = other.currentValue;

            if (containerMaxSize == memories.size()) {

                memories.pop_back();

                memories.push_front(other.currentValue);

                if (containsElement(guardedValue))
                    trackedIndex = findIndexOfElem(guardedValue);
                else {

                    switch (PostionType)
                    {
                    case FRONT:
                        trackedIndex = 0;

                        guardedValue = memories[0];
                        break;

                    case BACK:
                        trackedIndex = memories.size() - 1;

                        guardedValue = memories[memories.size() - 1];
                    default:
                        break;
                    }

                }
            }
            else {
                memories.push_front(other.currentValue);
            }

            return *this;
        }

        T_FixedGuard operator=(const T& other) {

            currentValue = other;

            if (containerMaxSize == memories.size()) {

                memories.pop_back();

                memories.push_front(other);

                if (containsElement(guardedValue))
                    trackedIndex = findIndexOfElem(guardedValue);
                else {

                    switch (PostionType)
                    {
                    case FRONT:
                        trackedIndex = 0;

                        guardedValue = memories[0];
                        break;

                    case BACK:
                        trackedIndex = memories.size() - 1;

                        guardedValue = memories[memories.size() - 1];
                    default:
                        break;
                    }

                }
            }
            else {
                memories.push_front(other);
            }

            return *this;
        }

        constexpr bool operator==(const T_FixedGuard& other) {
            return this->currentValue == other.guardedValue;
        }

        constexpr bool operator==(const T& other) {
            return this->currentValue == other;
        }

        T_FixedGuard operator+(const T_FixedGuard& other) {
            return T_FixedGuard{ this->currentValue + other.currentValue };
        }

        T_FixedGuard operator+(const T& other) {
            return T_FixedGuard{ this->currentValue + other };
        }

        T_FixedGuard operator-(const T_FixedGuard& other) {
            return T_FixedGuard{ this->currentValue + other.currentValue };
        }

        T_FixedGuard operator-(const T& other) {
            return T_FixedGuard{ this->currentValue + other };
        }

        T_FixedGuard operator*(const T_FixedGuard& other) {
            return T_FixedGuard{ this->currentValue * other.currentValue };
        }

        T_FixedGuard operator*(const T& other) {
            return T_FixedGuard{ this->currentValue * other };
        }

        T_FixedGuard operator/(const T_FixedGuard& other) {
            return T_FixedGuard{ this->currentValue / other.currentValue };
        }

        T_FixedGuard operator/(const T& other) {
            return T_FixedGuard{ this->currentValue / other };
        }

        T* findElemAtIndex(const size_t& index) {

            return this->FindElementAtIndex(index, memories);
    }

        bool containsElement(const T& elem) {

            return this->ContainsElement(elem, memories);
        }

        size_t findIndexOfElem(const T& elem) {

            return this->FindIndexOfElement(elem, memories);

        }

        constexpr size_t* getTrackedIndex() {
            return &trackedIndex;
        }

        constexpr T* getGuardedValue() {
            return &guardedValue;
        }

        constexpr T* getCurrentValue() {
            return &currentValue;
        }

        constexpr std::deque<T>* getSaveList() {
            return &memories;
        }

        constexpr T* getFrontElement() {
            return &memories[0];
        }

        constexpr T* getBackElement() {
            return &memories[memories.size() - 1];
        }

        void ReInitialize() {
            this->currentValue = this->guardedValue;
        }

        void PrintGuard() {
            std::cout << "====FIXED GUARD PRINT====\n";
            std::cout << "Current value: " << currentValue << " | Tracked index: " << trackedIndex << " | Guarded value : " << guardedValue << "\n";
            std::cout << "----MEMORIES----\n";
            for(T var : memories)
            {
                std::cout << var << "\n";
            }
            std::cout << "----------------\n";
            std::cout << "=========================\n";
        }

        void printSavesList() {
            this->PrintCollection(memories);
        }

        std::string toString() {
            std::ostringstream oss;

            oss << "| Guarded value: " << guardedValue
                << " | Tracked Index: " << trackedIndex
                << " | Current value: " << currentValue
                << " |\n";

            return oss.str();
        }
    };

public:

    template<typename T>
    class T_ConstLateIntializeable {

        T value;

        bool bInitialized = false;

    private:

        bool checkIfInitilaized() {
            if (bInitialized) {
                throw std::runtime_error("!%! LateInit called MORE THAN ONCE !&!");

                return true;
            }

            return false;
        }

    public:

        T_ConstLateIntializeable(const T& value) : value(value) {};

        T_ConstLateIntializeable() {}

        void LateInit(T&& other) {
            checkIfInitilaized();

            bInitialized = true;

            this->value = other;
        }

        const T getValue() const {
            return value;
        }

        bool operator==(const T& other) {
            this->value == other;
        }

        bool operator==(const T_ConstLateIntializeable& other) {
            this->value == other.getValue();
        }

        T_ConstLateIntializeable operator=(const T& other) {

            checkIfInitilaized();

            this->value = other;

            return T_ConstLateIntializeable{ other };
        }

        T_ConstLateIntializeable operator=(const T_ConstLateIntializeable& other) {
            checkIfInitilaized();

            this->value = other.getValue();

            return T_ConstLateIntializeable{ other.getValue() };
        }
    };

};
