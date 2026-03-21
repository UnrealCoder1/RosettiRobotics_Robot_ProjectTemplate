#pragma once

#include <iterator>
#include <numbers>

template<typename Type>
class T_Array {

private:

    uint32_t size;

    Type stack_alloc_vector[size];

};
