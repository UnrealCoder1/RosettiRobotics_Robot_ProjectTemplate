#pragma once

#include<concepts>
#include<string>
#include<deque>
#include<numbers>
#include<sstream>
#include<algorithm>

#define DEFAULT_NUMBER_OF_MEMORIES 3

class ReEvaluation {

public:

    template<typename T>
    class T_GuardValue {

    protected:
        T trackedValueCopy;

        T trackedValue;

    public:

        T_GuardValue(const T& value) {
            this->trackedValueCopy = value;
            this->trackedValue = value;
        }

        ~T_GuardValue() {
            this->trackedValue = trackedValueCopy;
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

        const T_GuardValue& operator+(const T_GuardValue& other) {
            return T_GuardValue{ this->trackedValue + other.trackedValue };
        }

        const T_GuardValue& operator+(const T& other) {
            return T_GuardValue{ this->trackedValue + T };
        }

        const T_GuardValue& operator-(const T_GuardValue& other) {
            return T_GuardValue{ this->trackedValue + other.trackedValue };
        }

        const T_GuardValue& operator-(const T& other) {
            return T_GuardValue{ this->trackedValue + T };
        }

        const T_GuardValue& operator*(const T_GuardValue& other) {
            return T_GuardValue{ this->trackedValue * other.trackedValue };
        }

        const T_GuardValue& operator*(const T& other) {
            return T_GuardValue{ this->trackedValue * T };
        }

        const T_GuardValue& operator/(const T_GuardValue& other) {
            return T_GuardValue{ this->trackedValue / other.trackedValue };
        }

        const T_GuardValue& operator/(const T& other) {
            return T_GuardValue{ this->trackedValue / T };
        }

        constexpr bool operator==(const T_GuardValue& other) const {
            return this->trackedValue == other->trackedValue;
        }

        constexpr bool operator==(const T& other) const {
            return this->trackedValue == other;
        }

        std::string toString() const {
            return std::to_string(trackedValue);
        }
    };

public:

    template<typename T>
    class T_HigherGuard {

    private:

        uint8_t maxNumberOfMemories;

        std::deque<T> memories;

        T currentValue;

        T guardedValue;

        uint8_t trackedIndex{};

    public:

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

        const T_HigherGuard& operator=(const T_HigherGuard& other) {
            this->currentValue = other.currentValue;

            if (memories.size() == maxNumberOfMemories) {

                this->memories.pop_back();  
            }

            this->memories.push_front(other.currentValue);

            if (findIndexOfElement(guardedValue) != nullptr) {
                trackedIndex = *findIndexOfElement(guardedValue);
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

            if (findIndexOfElement(guardedValue) != nullptr) {
                trackedIndex = *findIndexOfElement(guardedValue);
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

        const T_HigherGuard& operator+(const T_HigherGuard& other) {
            return T_HigherGuard{ this->currentValue + other.currentValue };
        }

        const T_HigherGuard& operator+(const T& other) {
            return T_HigherGuard{ this->currentValue + other };
        }

        const T_HigherGuard& operator-(const T_HigherGuard& other) {
            return T_HigherGuard{ this->currentValue + other.currentValue };
        }

        const T_HigherGuard& operator-(const T& other) {
            return T_HigherGuard{ this->currentValue + other };
        }

        const T_HigherGuard& operator*(const T_HigherGuard& other) {
            return T_HigherGuard{ this->currentValue * other.currentValue };
        }

        const T_HigherGuard& operator*(const T& other) {
            return T_HigherGuard{ this->currentValue * other };
        }

        const T_HigherGuard& operator/(const T_HigherGuard& other) {
            return T_HigherGuard{ this->currentValue / other.currentValue };
        }

        const T_HigherGuard& operator/(const T& other) {
            return T_HigherGuard{ this->currentValue / other };
        }

        T* setTrackedValue(const uint8_t& newTrackedValueIndex) {

            if (newTrackedValueIndex > (memories.size() - 1)) {

                std::cout << "!%! Invalid input index !%!\n";

                DEBUG_BREAK();

                return nullptr;
            }

            this->guardedValue = memories[newTrackedValueIndex];
        }

        uint8_t* findIndexOfElement(const T& element) {
            auto it = std::find(memories.begin(), memories.end(), element);

            if (it != memories.end()) {

                uint8_t index = static_cast<uint8_t>(std::distance(memories.begin(), it));

                return &index;

            }
            else {

                std::cout << "!%! HIGHER GUARD doesn't contain ELEMENT: " << element << " !%!\n";

                return nullptr;

            }
        }

        constexpr T* findElementAtIndex(const uint8_t& index) {

            if (index > (memories.size() - 1)) {

                std::cout << "!%! INVALID INDEX trying to find: | index: " << index << " | !%!\n";

                DEBUG_BREAK();

                return nullptr;
            }

            return &memories.at(index);
        }

        constexpr uint8_t* getTrackedIndex() {
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

        void printSavesList() {
            for (T elem : memories)
            {
                std::cout << elem << "\n";
            }
        }

        void ReInitializa() {
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

};
