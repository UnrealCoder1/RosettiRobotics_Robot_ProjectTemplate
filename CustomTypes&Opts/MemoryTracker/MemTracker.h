#pragma once
#include <memory>

static int allocations, copies, moves;

#define INIT_ALLOC_TRACKER \
void* operator new(size_t size) { \
\
    allocations += 1; \
\
    return malloc(size); \
}


class MemTracker {

public:

    static constexpr int* GetTotalNumberOfAllocations() {
        return &allocations;
    }

    static constexpr int* GetTotalNumberOfCopies() {
        return &copies;
    }

    static constexpr int* GetTotalNumberOfMoves() {
        return &moves;
    }

    MemTracker() noexcept {

    }

    MemTracker(const MemTracker& other) {
        copies += 1;
    }

    MemTracker(MemTracker&& other) noexcept {
        moves += 1;
    }

    static void PrintStats() {
        std::cout << "Number of HEAP ALLOCATIONS: " << allocations << "\n";
        std::cout << "Number of COPIES: " << copies << "\n";
        std::cout << "Number of MOVES: " << moves << "\n";
    }

};
