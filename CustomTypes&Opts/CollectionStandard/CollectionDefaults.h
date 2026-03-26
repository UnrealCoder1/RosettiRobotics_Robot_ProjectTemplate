
#pragma once

#include <vector>
#include <deque>
#include <type_traits>
#include <concepts>
#include <algorithm>
#include <iostream>

template<typename T, typename Ty>
concept Collection = std::same_as<std::remove_cvref_t<T>, std::vector<Ty>> || std::same_as<std::remove_cvref_t<T>, std::deque<Ty>>;

template<typename T>
concept Func = std::is_invocable_v<T>;

template<typename Type>
class CollectionDefaults {

protected:

    template<typename GenericCollection> requires Collection<GenericCollection, Type>
    static bool ContainsElement(const Type& element,GenericCollection&& collectionToCheck) {

        if (std::find(collectionToCheck.begin(), collectionToCheck.end(), element) != collectionToCheck.end()) return true;

        return false;
    }

    template<typename GenericCollection> requires Collection<GenericCollection, Type>
    static Type* FindElementAtIndex(const size_t& index, GenericCollection&& collection) {
        if (index > (collection.size() - 1)) return nullptr;

        return &collection[index];
    }

    template<typename GenericCollection> requires Collection<GenericCollection, Type>
    static size_t FindIndexOfElement(const Type& element, GenericCollection&& collection) {
        if (!ContainsElement(element, collection)) {
            std::cout << "!%! Invalid ELEMENT !%!\n";

            return -1;
        }

        auto search = std::find(collection.begin(), collection.end(), element);

        return std::distance(collection.begin(), search);
    }

    template<typename GenericCollection> requires Collection<GenericCollection, Type>
    static void Sort_Ascending(GenericCollection&& collection) {

        std::sort(collection.begin(), collection.end());
    }

    template<typename GenericCollection> requires Collection<GenericCollection, Type>
    static void Sort_Descending(GenericCollection&& collection) {

        std::sort(collection.begin(), collection.end(), [](Type a, Type b) -> bool { return b < a; });
    }

    template<typename GenericCollection, Func ComparisonFunction> requires Collection<GenericCollection, Type>
    static void Sort(GenericCollection&& collection, ComparisonFunction&& function) {

        std::sort(collection.begin(), collection.end(), function);
    }

    template<typename GenericCollection> requires Collection<GenericCollection, Type>
    static void PrintCollection(GenericCollection&& collection) {
        std::cout << "====COLLECTION PRINT====\n";

        for (Type elem : collection){
            std::cout << elem << "\n";
        }

        std::cout << "========================\n";
    }

};
