#ifndef MEMBER_H
#define MEMBER_H

#include "treasure.h"

/*implementacja klasy Adventurer*/
template<class ValueType, bool IsArmed> requires std::integral<ValueType>
class Adventurer {
public:
    using strength_t = uint32_t;
    static constexpr bool isArmed = IsArmed;

private:
    ValueType value = 0;
    strength_t strength = 0;

public:

    /*Konstruktory klasy:*/
    constexpr Adventurer() requires (!IsArmed) = default;

    constexpr Adventurer() requires(IsArmed) = delete;

    constexpr explicit Adventurer(strength_t newStrength) requires IsArmed:
            strength(newStrength) {}

    /*Metody klasy:*/
    constexpr strength_t getStrength() const requires IsArmed {
        return strength;
    }

    template<bool IsTrapped>
    constexpr void loot(Treasure<ValueType, IsTrapped> &&treasure) {
        if (IsTrapped) {
            if (IsArmed && strength > 0) {
                value += treasure.getLoot();
                strength /= 2;
            }
        } else {
            value += treasure.getLoot();
        }
    }

    constexpr ValueType pay() {
        ValueType giveawayLoot = value;
        value = 0;
        return giveawayLoot;
    }
};

/*Makro do Adventurer nieuzbrojonego*/
template<class ValueType> requires std::integral<ValueType>
using Explorer = Adventurer<ValueType, false>;


/*Implementacja klasy Veteran*/
template<class ValueType, std::size_t CompletedExpeditions> requires
std::integral<ValueType> && (CompletedExpeditions < 25)
class Veteran {
public:
    using strength_t = uint32_t;

private:
    ValueType value;
    strength_t strength;

    /*Funkcja do obliczania n-tej liczby fibonacciego*/
    static constexpr ValueType fib(std::size_t number) {
        int num1 = 0;
        int num2 = 1;
        int nextnum = 1;
        for (std::size_t i = 1; i < number; i++) {
            nextnum = num1 + num2;
            num1 = num2;
            num2 = nextnum;
        }
        return number == 0 ? 0 : nextnum;
    }

public:
    static constexpr bool isArmed = true;

    /*Konstruktory klasy:*/
    constexpr explicit Veteran()
            : value(0),
              strength(fib(CompletedExpeditions)) {
    }

    /*Metody klasy:*/
    constexpr strength_t getStrength() const {
        return this->strength;
    }

    template<bool IsTrapped>
    constexpr void loot(Treasure<ValueType, IsTrapped> &&treasure) {
        if (IsTrapped) {
            if (strength > 0) {
                value += treasure.getLoot();
            }
        } else {
            value += treasure.getLoot();
        }
    }

    constexpr ValueType pay() {
        ValueType giveawayLoot = value;
        value = 0;
        return giveawayLoot;
    }
};

#endif //MEMBER_H
