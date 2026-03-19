#pragma once

#include<type_traits>
#include<iostream>

#if defined(_MSC_VER)
#include <intrin.h>
#define DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__) || defined(__clang__)
#define DEBUG_BREAK() __builtin_trap()
#else
#define DEBUG_BREAK() std::abort()
#endif

template<typename T>
concept Memory_Value = (std::is_pointer_v<T>)
                    || (std::is_reference_v<T> && std::is_const_v<std::remove_reference_t<T>>)
                    || (std::is_reference_v<T>)
                    || (!std::is_pointer_v<T> && !std::is_reference_v<T>);

class Verifications {
public:

    template<Memory_Value MemoryValue>
    static const bool& Ensure(MemoryValue&& ensuredValue) {

        if constexpr (std::is_pointer_v<MemoryValue>) {
            if (ensuredValue != nullptr) return true;

            std::cout << "!%! NULL POINTER Exception : PASSED by ENSURE safety FUNCTION !%!\n";

            return false;
        }
        else if constexpr (std::is_reference_v<MemoryValue> && std::is_const_v<std::remove_reference_t<MemoryValue>>) {

            if (&ensuredValue != nullptr) return true;

            std::cout << "!%! NULL CONST REFRENCE Identified : PASSED by ENSURE safety FUNCTION !%!\n";

            return false;

        }
        else if constexpr (std::is_reference_v<MemoryValue>) {
            if (&ensuredValue != nullptr) return true;

            std::cout << "!%! NULL REFRENCE Identified : PASSED by ENSURE safety FUNCTION !%!\n";

            return false;
        }
        else if constexpr (!std::is_pointer_v<MemoryValue> && !std::is_reference_v<MemoryValue>){
            if (ensuredValue != NULL) return true;

            std::cout << "!%! NULL OBJECT Identified : PASSED by ENSURE safety FUNCTION !%!\n";

            return false;
        }
    }

    template<Memory_Value MemoryValue>
    static const bool& Check(MemoryValue&& checkedValue) {
        if (!Ensure(checkedValue)) {
            DEBUG_BREAK();

            return false;
        }
        else
        {
            return true;
        }
    }

    template<Memory_Value MemoryValue>
    static std::remove_cvref_t<MemoryValue> EnsureIsValid(MemoryValue&& value)
    {
        using CleanT = std::remove_cvref_t<MemoryValue>;

        if (!Ensure(value))
        {
            if constexpr (std::is_pointer_v<CleanT>)
                return nullptr;

            return {};
        }

        return static_cast<CleanT>(value);
    }

    template<Memory_Value MemoryValue>
    static std::remove_cvref_t<MemoryValue> CheckIsValid(MemoryValue&& value) {

        if constexpr (std::is_pointer_v<std::remove_cvref_t<MemoryValue>>) {

            if (EnsureIsValid(value) != nullptr) {
                return static_cast<std::remove_cvref_t<MemoryValue>>(value);
            }
            else {

                DEBUG_BREAK();

                return nullptr;
            }
        }
        else {
            if (value == NULL) DEBUG_BREAK();

            return {};
        }
    }
};
