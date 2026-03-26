#pragma once

#include <iterator>
#include <numbers>
#include <array>
#include <type_traits>
#include <initializer_list>

template<typename T>
concept Array = std::is_array_v<T>;

template<typename Type>
class ContainerArray {

    Type containerArray[];

    ContainerArray(size_t size) {
        containerArray = Type[size];
    }

    template<size_t Size>
    std::array<Type, Size> operator=(const std::array<Type, Size>& other) {

        std::array<Type, other.size()> returnArray;

        for (size_t i = 0; i < other.size(); i++) {
            returnArray[i] = other[i];
        }

        return returnArray;
    }

};

template<typename Type>
class T_Array {

private:

    size_t size = 1;

    const size_t old_capacity = size;

    std::array<Type, 100> stack_container;

private:



    const size_t CalculateGrowth(const size_t& added_num_of_elems) const {
        return old_capacity += added_num_of_elems;
    }

public:

    T_Array(std::initializer_list<Type> elems) {

        std::array<Type, 10> intermediary;

        stack_container = intermediary;

        size = elems.size();

        for (int i = 0; i < elems.size(); i++)
        {
            auto list_element = elems.begin();

            std::advance(list_element, i);

            (stack_container)[i] = std::move(*list_element);
        }
    }

    T_Array() {
        size = 0;
    }

    void PrintArray() {
        for (int i = 0; i < stack_container.size(); i++) {
            std::cout << (stack_container)[i] << "\n";
        }
    }

};
