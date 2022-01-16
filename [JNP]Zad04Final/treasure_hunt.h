#ifndef TREASURE_HUNT_H
#define TREASURE_HUNT_H

#include <iostream>
#include <concepts>
#include <type_traits>
#include "treasure.h"
#include "member.h"

template<class T>
struct isTreasure : std::false_type {
};

template<std::integral ValueType, bool IsTrapped>
struct isTreasure<Treasure<ValueType, IsTrapped>> : std::true_type {
};

/*Concept sprawdzający, czy typ Treasure jest poprawny*/
template<class T>
concept EncounterSideTreasure = isTreasure<T>::value;


/*Concept sprawdzający, czy typ Member jest poprawny*/
template<class Type>
concept EncounterSideMember = requires(Type obj) {
    typename Type::strength_t;
    { []() constexpr { return Type::isArmed; }() };
    { obj.isArmed } -> std::convertible_to<bool>;
    { obj.pay() } -> std::integral<>;
    (obj.loot(Treasure<decltype(obj.pay()), true>(0)));
    (obj.loot(Treasure<decltype(obj.pay()), false>(0)));
};

/*Concept EncounterSide*/
template<class T>
concept EncounterSide = requires(T v){
    EncounterSideTreasure<T> || EncounterSideMember<T>;
};

/*Implementacja klasy Encounter*/
template<class sideA, class sideB> requires EncounterSide<sideA>
                                            && EncounterSide<sideB>
class Encounter {
public:
    sideA &first;
    sideB &second;;

    /*Konstruktor klasy:*/
    constexpr Encounter(sideA &firstSide, sideB &secondSide) :
            first(firstSide), second(secondSide) {}
};

/*Implementacje funkcji run:*/
template<class A, class B>
requires EncounterSideTreasure<A> && EncounterSideTreasure<B>
constexpr void run(Encounter<A, B> encounter) = delete;

template<class A, class B>
requires EncounterSideTreasure<A> && EncounterSideMember<B>
constexpr void run(Encounter<A, B> encounter) {
    encounter.second.loot(std::move(encounter.first));
}

template<class A, class B>
requires EncounterSideMember<A> && EncounterSideTreasure<B>
constexpr void run(Encounter<A, B> encounter) {
    encounter.first.loot(std::move(encounter.second));
}

/*Pomocnicza funkcja do run():*/
template<EncounterSideMember A, EncounterSideMember B>
constexpr void steal(A &memberA, B &memberB) {
    auto value = memberB.pay();
    Treasure T = SafeTreasure < decltype(value) > {value};
    memberA.loot(std::move(T));
}

template<class A, class B>
requires EncounterSideMember<A> && EncounterSideMember<B>
constexpr void run(Encounter<A, B> encounter) {

    if constexpr(A::isArmed && B::isArmed) {
        if (encounter.first.getStrength() > encounter.second.getStrength()) {
            steal(encounter.first, encounter.second);
        } else if (encounter.first.getStrength() <
                   encounter.second.getStrength()) {
            steal(encounter.second, encounter.first);
        } else return;
    } else if constexpr (A::isArmed && !B::isArmed) {
        steal(encounter.first, encounter.second);
    } else if constexpr(!A::isArmed && B::isArmed) {
        steal(encounter.second, encounter.first);
    }
}

/*Implementacja ekspedycji bez zdarzeń*/
constexpr void expedition() {};

/*Implementacja ekspedycji ze zdarzeniami*/
template<class A, class B, class... T>
requires EncounterSide<A> && EncounterSide<B>
constexpr void expedition(Encounter<A, B> encounter, const T... args) {
    run(encounter);
    expedition(args...);
}

#endif //TREASURE_HUNT_H
