#ifndef TREASURE_H
#define TREASURE_H

#include <iostream>
#include <concepts>


/*Implementacja klasy Treasure*/
template<class ValueType, bool IsTrapped> requires std::integral<ValueType>
class Treasure {

private:
    ValueType myLoot;

public:
    constexpr static bool isTrapped = IsTrapped;

    /*Metody klasy:*/
    constexpr ValueType evaluate() const {
        return myLoot;
    }

    constexpr ValueType getLoot() {
        ValueType oldLoot = myLoot;
        myLoot = 0;
        return oldLoot;
    }

    /*Konstruktory klasy:*/
    constexpr explicit Treasure(ValueType value) : myLoot(value) {}
};

/*Makro do bezpiecznej skrzyni*/
template<class ValueType> requires std::integral<ValueType>
using SafeTreasure = Treasure<ValueType, false>;

/*Makro do skrzyni z pułapką*/
template<class ValueType> requires std::integral<ValueType>
using TrappedTreasure = Treasure<ValueType, true>;

#endif //TREASURE_H
