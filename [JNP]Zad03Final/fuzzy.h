#ifndef FUZZY_H
#define FUZZY_H

//Wykorzystywane biblioteki
#include <initializer_list>
#include <iostream>
#include <algorithm>
#include <array>
#include <set>
#include <cmath>

//Typ reprezentujący współrzedną liczby rozmytej
using real_t = double;

//Klasa definiująca trójkątną liczbę rozmytą
class TriFuzzyNum {

public:
    TriFuzzyNum() = delete;

    //Konstruktor liczby rozmytej
    constexpr TriFuzzyNum(const real_t val1, const real_t val2, const
    real_t val3)
            : l(val1), m(val2), u(val3) {
        fix_representation();
    }

    //Konstruktor liczby rozmytej za pomocą kopiowania
    constexpr TriFuzzyNum(const TriFuzzyNum &other) = default;

    //Konstruktor liczby rozmytej za pomocą przenoszenia
    constexpr TriFuzzyNum(const TriFuzzyNum &&other) :
            l(std::move(other.l)),
            m(std::move(other.m)),
            u(std::move(other.u)) {}

    //Operacja przypisania w wersji kopiującej
    TriFuzzyNum &operator=(TriFuzzyNum other) {
        std::swap(l, other.l);
        std::swap(m, other.m);
        std::swap(u, other.u);
        return *this;
    }

    //Operacja przypisania w wersji przenoszącej
    TriFuzzyNum &operator=(TriFuzzyNum &&other) {
        l = std::move(other.l);
        m = std::move(other.m);
        u = std::move(other.u);
        return *this;
    }

    //Operacja dodawania
    TriFuzzyNum operator+(const TriFuzzyNum &other) const {
        return TriFuzzyNum(l + other.l, m + other.m,
                           u + other.u);
    }

    //Operacja odejmowania
    TriFuzzyNum operator-(const TriFuzzyNum &other) const {
        return TriFuzzyNum(l - other.u, m - other.m,
                           u - other.l);
    }

    //Operacja mnożenia
    TriFuzzyNum operator*(const TriFuzzyNum &other) const {
        return TriFuzzyNum(l * other.l, m * other.m,
                           u * other.u);
    }

    //Operacja dodawania w wersji z przypisaniem
    TriFuzzyNum &operator+=(const TriFuzzyNum &other) {
        l += other.l;
        m += other.m;
        u += other.u;
        return *this;
    }

    //Operacja odejmowania w wersji z przypisaniem
    TriFuzzyNum &operator-=(const TriFuzzyNum &other) {
        l -= other.u;
        m -= other.m;
        u -= other.l;
        fix_representation();
        return *this;
    }

    //Operacja mnożenia w wersji z przypisaniem
    TriFuzzyNum &operator*=(const TriFuzzyNum &other) {
        l *= other.l;
        m *= other.m;
        u *= other.u;
        fix_representation();
        return *this;
    }

    //Operacja porównania za pomocą operatora "=="
    bool operator==(const TriFuzzyNum &b) const = default;

    //Operacja porównania za pomocą operatora "<=>"
    std::partial_ordering operator<=>(const TriFuzzyNum &b) const {
        std::array<real_t, 3> rank_a = get_rank();
        std::array<real_t, 3> rank_b = b.get_rank();
        return rank_a <=> rank_b;
    }

    //Operacja wysłania do Strumienia wyjściowego
    friend std::ostream &operator<<(std::ostream &os, const TriFuzzyNum &tfn) {
        os << "(" << tfn.l << ", " << tfn.m << ", " << tfn.u << ")";
        return os;
    };

    //Gettery do współrzędnych liczby rozmytej
    constexpr real_t lower_value() const { return l; }

    constexpr real_t modal_value() const { return m; }

    constexpr real_t upper_value() const { return u; }

private:
    //Współrzędne liczby rozmytej
    real_t l;
    real_t m;
    real_t u;

    //Poprawienie kolejności współrzędnych liczby rozmytej
    constexpr void fix_representation() {
        std::array<real_t, 3> to_sort = {l, m, u};
        std::sort(to_sort.begin(), to_sort.end());
        l = to_sort[0], m = to_sort[1], u = to_sort[2];
    }

    //Funkcja pomocnicza zwracająca kwadrat z podanej współrzędnej
    static constexpr real_t square(real_t v) {
        return v * v;
    }

    //Obliczenie rangi liczby rozmytej
    constexpr std::array<real_t, 3> get_rank() const {
        real_t z = (u - l) + sqrt(1 + square(u - m)) +
                   sqrt(1 + square(m - l));
        real_t x = ((u - l) * m + sqrt(1 + square(u - m)) * l +
                    sqrt(1 + square(m - l)) * u) / z;
        real_t y = (u - l) / z;
        return {x - y / 2, 1 - y, m};
    }
};

//Klasa implementująca wielozbiór trójkątnych liczb rozmytych
class TriFuzzyNumSet {

private:
    //Kontener do przechowywania trójkątnych liczb rozmytych
    std::multiset<TriFuzzyNum> triFuzzyNums;

public:
    //Konstruktor do tworzenia pustej kolekcji
    TriFuzzyNumSet() = default;

    //Konstruktor korzystający z listy inicjalizującej
    TriFuzzyNumSet(std::initializer_list<TriFuzzyNum> args)
            : triFuzzyNums(args) {}

    //Konstruktor za pomocą kopiowania innej kolekcji
    TriFuzzyNumSet(const TriFuzzyNumSet &other) = default;

    //Konstruktor za pomocą przenoszenia innej kolekcji
    TriFuzzyNumSet(TriFuzzyNumSet &&other)
            : triFuzzyNums(move(other.triFuzzyNums)) {}

    //Operator przypisania za pomocą kopiowania
    TriFuzzyNumSet &operator=(const TriFuzzyNumSet &other) {
        triFuzzyNums = other.triFuzzyNums;
        return *this;
    }

    //Operator przypisania za pomocą przenoszenia
    TriFuzzyNumSet &operator=(TriFuzzyNumSet &&other) {
        triFuzzyNums = (move(other.triFuzzyNums));
        return *this;
    }

    //Metoda wstawiania do kolekcji w wersji kopiującej
    void insert(const TriFuzzyNum &element) {
        triFuzzyNums.emplace(element);
    }

    //Metoda wstawiania do kolekcji w wersji przenoszącej
    void insert(TriFuzzyNum &&element) {
        triFuzzyNums.insert(std::move(element));
    }

    //Metoda usuwania elementu z kolekcji
    void remove(const TriFuzzyNum &element) {
        auto iterator = find(triFuzzyNums.begin(),
                             triFuzzyNums.end(), element);
        if (iterator != triFuzzyNums.end())
            triFuzzyNums.erase(iterator);
    }

    /*
     * Metoda zwracająca liczbę rozmytą, której parametry
     * są średnimi arytmetycznymi parametrów wszystkich liczb przechowywanych
     * w kolekcji.
     */
    TriFuzzyNum arithmetic_mean() {
        if (!triFuzzyNums.empty()) {
            real_t lowerValueSum = 0;
            real_t modalValueSum = 0;
            real_t upperValueSum = 0;
            unsigned long multiSetSize = triFuzzyNums.size();
            for (auto element: triFuzzyNums) {
                lowerValueSum += element.lower_value();
                modalValueSum += element.modal_value();
                upperValueSum += element.upper_value();
            }
            real_t avgLowerValue = lowerValueSum / (double) multiSetSize;
            real_t avgModalValue = modalValueSum / (double) multiSetSize;
            real_t avgUpperValue = upperValueSum / (double) multiSetSize;

            return TriFuzzyNum({avgLowerValue, avgModalValue,
                                avgUpperValue});
        } else {
            throw std::length_error(
                    "TriFuzzyNumSet::arithmetic_mean - the set is empty.");
        }
    }

};

/*
 * Globalna funkcja zwracająca liczbę rozmytą o wszystkich parametrach
 * równych jednej wartości
 */
consteval TriFuzzyNum crisp_number(real_t v) {
    return TriFuzzyNum(v, v, v);
}

//Stała globalna reprezentująca liczbę rozmytą (0, 0, 0)
constinit const TriFuzzyNum crisp_zero = crisp_number(0);

#endif
