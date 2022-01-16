#include "treasure.h"
#include "member.h"
#include "treasure_hunt.h"
#include <concepts>
#include <cassert>

/* Tests description
 * Group 1 - hunt_examples.cc
 * Group 2 - Instantiations and aliases of the Treasure template
 * Group 3 - Instantiations and aliases of Adventurer and Veteran templates
 * Group 4 - Logic correctness of Treasure and Member classes
 * Group 5 - EncounterSide concept
 * Group 6 - run() and expedition() functions
 */

/**************************************************************
 *                          Group 1                           *
 **************************************************************/

constexpr void testGroup1() {
    // TEST #101
    // is solo_hunt() example from hunt_examples.cc.

#if TEST_NUM == 101
    auto test = [] () constexpr {
        Explorer<int> a;
        Treasure<int, false> t1(5);
        Treasure<int, false> t2(6);
        SafeTreasure<int> t3(7);
        TrappedTreasure<int> t4(10);

        Encounter<decltype(a), decltype(t1)> e1 = {a, t1};
        Encounter<decltype(t2), decltype(a)> e2 = {t2, a};
        Encounter<decltype(t3), decltype(a)> e3 = {t3, a};
        Encounter<decltype(a), decltype(t4)> e4 = {a, t4};

        expedition(e1, e2, e3, e4);

        return a.pay(); // 18
    };

    static_assert(test() == 18);
#endif

    // TEST #102
    // is holyGrail() example from hunt_examples.cc.

#if TEST_NUM == 102
    auto test = [] () constexpr {
        TrappedTreasure<int> grail(10000);
        Veteran<int, 6> arthur;
        Adventurer<int, true> morgana(15);

        Encounter<decltype(arthur), decltype(grail)> e1 = {arthur, grail};
        Encounter<decltype(morgana), decltype(grail)> e2 = {morgana, grail};
        Encounter<decltype(arthur), decltype(morgana)> e3 = {arthur, morgana};

        expedition(e2, e1, e1, e3, e3);

        return arthur.pay(); // 10000
    };

    static_assert(test() == 10000);
#endif

    // TEST #103
    // is tiringTraps() example from hunt_examples.cc.

#if TEST_NUM == 103
    auto test = [] () constexpr {
        Adventurer<int, true> a(2);
        TrappedTreasure<int> t1(1);
        TrappedTreasure<int> t2(1);
        TrappedTreasure<int> t3(1);
        TrappedTreasure<int> t4(1);

        Encounter<decltype(a), decltype(t1)> e1 = {a, t1};
        Encounter<decltype(a), decltype(t1)> e2 = {a, t2};
        Encounter<decltype(a), decltype(t1)> e3 = {a, t3};
        Encounter<decltype(a), decltype(t1)> e4 = {a, t4};

        expedition(e1, e2, e3, e4);

        return a.pay(); // 2
    };

    static_assert(test() == 2);
#endif
} // Test Group 1

/**************************************************************
 *                          Group 2                           *
 **************************************************************/

#if TEST_NUM == 201
#include <numeric>
#endif

constexpr void testGroup2() {
    // TEST #201
    // checks proper instantiations of Treasure class templates
    // and the SafeTreasure and TrappedTreasure aliases.

#if TEST_NUM == 201
    constexpr Treasure<int, true> t1 {1000};
    constexpr Treasure<unsigned short, false> t2 {10};
    constexpr SafeTreasure<char> t3 {'b'};
    constexpr TrappedTreasure<uint64_t> t4 {std::numeric_limits<uint64_t>::max()};
    constexpr SafeTreasure<bool> t5 {true};
    constexpr Treasure<wchar_t, true> t6 {'v'};
#endif

    // TEST #202
    // checks types of methods and members of the Treasure class.

#if TEST_NUM == 202
    SafeTreasure<unsigned long> t {10};
    static_assert(std::same_as<unsigned long, decltype(t.evaluate())>);
    static_assert(std::same_as<unsigned long, decltype(t.getLoot())>);

    static_assert(std::same_as<const bool, decltype(SafeTreasure<int>::isTrapped)>);
    static_assert(Treasure<int, true>::isTrapped);
    static_assert(!Treasure<int, false>::isTrapped);
    static_assert(!SafeTreasure<int>::isTrapped);
    static_assert(TrappedTreasure<int>::isTrapped);
#endif

    // TEST #203
    // should not compile, because
    // incorrect instantiation of Treasure with ValueType = float.

#if TEST_NUM == 203
    [[maybe_unused]] constexpr SafeTreasure<float> t1 {3.14};
#endif

    // TEST #204
    // should not compile, because
    // incorrect instantiation of Treasure with ValueType = int*.

#if TEST_NUM == 204
    [[maybe_unused]] constexpr SafeTreasure<int*> t1 {0};
#endif
} // Test Group 2

/**************************************************************
 *                          Group 3                           *
 **************************************************************/

#if TEST_NUM > 300 && TEST_NUM < 400
    template<typename T>
    concept withStrength = requires (T x) {
        x.getStrength();
    };
#endif

constexpr void testGroup3() {
    // TEST #301
    // checks proper instantiations of Adventurer class templates
    // and the Explorer alias.

#if TEST_NUM == 301
    constexpr Adventurer<int, false> a1;
    constexpr Adventurer<int, true> a2 {100};
    constexpr Adventurer<char, false> a3;
    constexpr Adventurer<uint64_t, true> a4 {1000};
    constexpr Explorer<bool> e;
#endif

    // TEST #302
    // checks proper instantiations of Veteran class templates.

#if TEST_NUM == 302
    constexpr Veteran<int, 10> v1;
    constexpr Veteran<char, 5> v2;
#endif

    // TEST #303
    // checks types of methods and members of Explorers.

#if TEST_NUM == 303
    Explorer<uint8_t> e;

    static_assert(std::same_as<uint8_t, decltype(e.pay())>);

    SafeTreasure<uint8_t> safe {1};
    TrappedTreasure<uint8_t> trapped {1};
    static_assert(std::same_as<void, decltype(e.loot(std::move(safe)))>);
    static_assert(std::same_as<void, decltype(e.loot(std::move(trapped)))>);

    static_assert(std::same_as<const bool, decltype(Explorer<int>::isArmed)>);
    static_assert(!Explorer<int>::isArmed);

    static_assert(std::same_as<uint32_t, Explorer<int>::strength_t>);
    static_assert(!withStrength<Explorer<uint8_t>>);
#endif

    // TEST #304
    // checks types of methods and members of armed Adventurers.

#if TEST_NUM == 304
    Adventurer<uint64_t, true> a {111};

    static_assert(std::same_as<uint64_t, decltype(a.pay())>);

    SafeTreasure<uint64_t> safe {1};
    TrappedTreasure<uint64_t> trapped {1};
    static_assert(std::same_as<void, decltype(a.loot(std::move(safe)))>);
    static_assert(std::same_as<void, decltype(a.loot(std::move(trapped)))>);

    static_assert(std::same_as<const bool, decltype(decltype(a)::isArmed)>);
    static_assert(decltype(a)::isArmed);

    static_assert(std::same_as<uint32_t, decltype(a)::strength_t>);
    static_assert(withStrength<decltype(a)>);
#endif

    // TEST #305
    // checks types of methods and members of the Veteran class.

#if TEST_NUM == 305
    Veteran<char, 5> v;

    static_assert(std::same_as<char, decltype(v.pay())>);

    SafeTreasure<char> safe {1};
    TrappedTreasure<char> trapped {1};
    static_assert(std::same_as<void, decltype(v.loot(std::move(safe)))>);
    static_assert(std::same_as<void, decltype(v.loot(std::move(trapped)))>);

    static_assert(std::same_as<const bool, decltype(decltype(v)::isArmed)>);
    static_assert(decltype(v)::isArmed);

    static_assert(std::same_as<uint32_t, decltype(v)::strength_t>);
    static_assert(withStrength<decltype(v)>);
#endif

    // TEST #306
    // should not compile, because
    // incorrect instantiation of Adventurer with ValueType = double.

#if TEST_NUM == 306
    [[maybe_unused]] constexpr Adventurer<double, true> a {42.24};
#endif

    // TEST #307
    // should not compile, because
    // incorrect instantiation of Explorer with ValueType = int*.

#if TEST_NUM == 307
    [[maybe_unused]] constexpr Explorer<int*> e;
#endif

    // TEST #308
    // should not compile, because
    // incorrect instantiation of Veteran with ValueType = char*.

#if TEST_NUM == 308
    [[maybe_unused]] constexpr Veteran<char*, 7> v;
#endif

    // TEST #309
    // should not compile, because
    // incorrect instantiation of Veteran with over 24 completed expeditions.

#if TEST_NUM == 309
    [[maybe_unused]] constexpr Veteran<int, 25> v;
#endif
} // Test Group 3

/**************************************************************
 *                          Group 4                           *
 **************************************************************/

constexpr void testGroup4() {
    // TEST #401
    // checks correctness of the Treasure class logic.

#if TEST_NUM == 401
    SafeTreasure<int> t {100};
    assert(t.evaluate() == 100);
    assert(t.getLoot() == 100);
    assert(t.evaluate() == 0);
    assert(t.getLoot() == 0);
#endif

    // TEST #402
    // checks correctness of the Explorer class logic.

#if TEST_NUM == 402
    Explorer<int> e;
    assert(e.pay() == 0);

    SafeTreasure<int> safe {100};
    e.loot(std::move(safe));
    assert(e.pay() == 100);

    assert(e.pay() == 0);

    TrappedTreasure<int> trapped {999};
    e.loot(std::move(trapped));
    assert(e.pay() == 0);
#endif

    // TEST #403
    // checks correctness of the armed Adventurer class logic.

#if TEST_NUM == 403
    Adventurer<int, true> a {3};
    assert(a.pay() == 0);
    assert(a.getStrength() == 3);

    SafeTreasure<int> safe {100};
    a.loot(std::move(safe));
    assert(a.pay() == 100);

    assert(a.pay() == 0);

    TrappedTreasure<int> trapped {1};
    a.loot(std::move(trapped));
    assert(a.pay() == 1);
    assert(a.getStrength() == 1);

    TrappedTreasure<int> t1 {5};
    a.loot(std::move(t1));
    TrappedTreasure<int> t2 {6};
    a.loot(std::move(t2));

    assert(a.pay() == 5);
    assert(a.getStrength() == 0);

    SafeTreasure<int> t3 {999};
    a.loot(std::move(t3));
    assert(a.pay() == 999);
#endif

    // TEST #404
    // checks correctness of the Veteran class logic.

#if TEST_NUM == 404
    static_assert(Veteran<int, 1>().getStrength() == 1);
    static_assert(Veteran<int, 6>().getStrength() == 8);
    static_assert(Veteran<int, 24>().getStrength() == 46368);

    Veteran<int, 5> v;
    assert(v.pay() == 0);
    assert(v.getStrength() == 5);

    SafeTreasure<int> safe {100};
    v.loot(std::move(safe));
    assert(v.pay() == 100);

    assert(v.pay() == 0);

    TrappedTreasure<int> trapped {100};
    v.loot(std::move(trapped));

    assert(v.getStrength() == 5);
    assert(v.pay() == 100);

    Veteran<int, 0> weak;
    TrappedTreasure<int> t1 {99};
    SafeTreasure<int> t2 {1};

    weak.loot(std::move(t1));
    weak.loot(std::move(t2));

    assert(weak.pay() == 1);
#endif
} // Test Group 4

/**************************************************************
 *                          Group 5                           *
 **************************************************************/

#if TEST_NUM == 502
    struct Bystander {
        using strength_t = uint32_t;
        static const int *isArmed;
        int16_t pay() { return 0; }
        template<bool B>
        void loot(Treasure<int16_t, B> &&) {}
    };
#endif

#if TEST_NUM == 503
    template<std::integral T, bool B>
    class FakeTreasure {
    private:
        T val;
    public:
        constexpr static bool isTrapped = B;
        explicit constexpr FakeTreasure(T) {}
        constexpr T getLoot() { return val; }
        constexpr T evaluate() const { return val; }
    };
#endif

#if TEST_NUM == 504
    struct BadExpeditionMember {
        static const int isArmed = 10;
        int16_t pay() { return 0; }
        template<bool B>
        void loot(Treasure<int16_t, B> &&) {}
    };
#endif

#if TEST_NUM == 505
#include <string>

    struct BadExpeditionMember {
        using strength_t = uint32_t;
        static constexpr std::string_view isArmed = "10";
        int16_t pay() { return 0; }
        template<bool B>
        void loot(Treasure<int16_t, B> &&) {}
    };
#endif

#if TEST_NUM == 506
    struct BadExpeditionMember {
        using strength_t = uint32_t;
        static const int isArmed = 10;
        float pay() { return 0.0; }
        template<bool B>
        void loot(Treasure<int16_t, B> &&) {}
    };
#endif

#if TEST_NUM == 507
    struct BadExpeditionMember {
        using strength_t = uint32_t;
        static const int isArmed = 10;
        int16_t pay() { return 0; }
        template<bool B>
        void loot(Treasure<uint64_t, B> &&) {}
    };
#endif

#if TEST_NUM == 508
    struct BadExpeditionMember {
        using strength_t = uint32_t;
        static const int isArmed = 10;
        int16_t pay() { return 0; }
        void loot(SafeTreasure<int16_t> &&) {}
    };
#endif

#if TEST_NUM == 509
    struct BadExpeditionMember {
        using strength_t = uint32_t;
        static const int isArmed = 10;
        int16_t pay() { return 0; }
        void loot(TrappedTreasure<int16_t> &&) {}
    };
#endif

#if TEST_NUM == 510
    struct BadExpeditionMember {
        using strength_t = uint32_t;
        static const int isArmed = 10;
        template<bool B>
        void loot(Treasure<int16_t, B> &&) {}
    };
#endif

#if TEST_NUM == 511
    struct BadExpeditionMember {
        using strength_t = uint32_t;
        static const int isArmed = 10;
        int16_t pay() { return 0; }
    };
#endif

#if TEST_NUM == 512
    struct BadExpeditionMember {
        using strength_t = uint32_t;
        const int isArmed = 10;
        int16_t pay() { return 0; }
        template<bool B>
        void loot(Treasure<int16_t, B> &&) {}
    };
#endif

constexpr void testGroup5() {
    // TEST #501
    // checks if ExpeditionSide concept accepts all Treasure types.

#if TEST_NUM == 501
    static_assert(EncounterSide<SafeTreasure<int>>);
    static_assert(EncounterSide<TrappedTreasure<char>>);
    static_assert(EncounterSide<SafeTreasure<uint64_t>>);
#endif

    // TEST #502
    // checks if ExpeditionSide concept accepts a minimal
    // struct that passes all the listed requirements
    // of an expedition member.

#if TEST_NUM == 502
    static_assert(EncounterSide<Bystander>);
#endif

    // TEST #503
    // Incorrect treasure-like type

#if TEST_NUM == 503
    static_assert(!EncounterSide<FakeTreasure<int, false>>);
#endif

    // TEST #504
    // Incorrect expedition member with no dependent typename strength_t

#if TEST_NUM == 504
    static_assert(!EncounterSide<BadExpeditionMember>);
#endif

    // TEST #505
    // Incorrect expedition member with isArmed's type
    // not convertible to bool

#if TEST_NUM == 505
    static_assert(!EncounterSide<BadExpeditionMember>);
#endif

    // TEST #506
    // Incorrect expedition member with wrong type returned
    // from pay()

#if TEST_NUM == 506
    static_assert(!EncounterSide<BadExpeditionMember>);
#endif

    // TEST #507
    // Incorrect expedition member with method loot()
    // accepting Treasures with ValueTypes incompatible
    // with its pay() return type

#if TEST_NUM == 507
    static_assert(!EncounterSide<BadExpeditionMember>);
#endif

    // TEST #508
    // Incorrect expedition member with method loot()
    // only defined for SafeTreasures

#if TEST_NUM == 508
    static_assert(!EncounterSide<BadExpeditionMember>);
#endif

    // TEST #509
    // Incorrect expedition member with method loot()
    // only defined for TrappedTreasures

#if TEST_NUM == 509
    static_assert(!EncounterSide<BadExpeditionMember>);
#endif

    // TEST #510
    // Incorrect expedition member with no method pay()

#if TEST_NUM == 510
    static_assert(!EncounterSide<BadExpeditionMember>);
#endif

    // TEST #511
    // Incorrect expedition member with no method loot()

#if TEST_NUM == 511
    static_assert(!EncounterSide<BadExpeditionMember>);
#endif

    // TEST #512
    // should not compile (because of how GCC intereprets
    // the code inside a lambda, inside a requires clause,
    // in clang the concept is simply not satisfied).
    // Incorrect expedition member with public field isArmed

#if TEST_NUM == 512
    static_assert(!EncounterSide<BadExpeditionMember>);
#endif
} // Test Group 5

/**************************************************************
 *                          Group 6                           *
 **************************************************************/

#if TEST_NUM > 600 && TEST_NUM < 700
#include <tuple>
#endif

constexpr void testGroup6() {
    // TEST #601
    // checks proper Encounter declarations.

#if TEST_NUM == 601
    SafeTreasure<int> safe {100};
    TrappedTreasure<int> trapped {999};
    Explorer<int> e;
    Adventurer<int, true> a {1};
    Veteran<int, 10> v;

    [[maybe_unused]] Encounter<decltype(safe), decltype(trapped)> e1 = {safe, trapped};
    [[maybe_unused]] Encounter<decltype(e), decltype(trapped)> e2 = {e, trapped};
    [[maybe_unused]] Encounter<decltype(safe), decltype(e)> e3 = {safe, e};
    [[maybe_unused]] Encounter<decltype(e), decltype(a)> e4 = {e, a};
    [[maybe_unused]] Encounter<decltype(a), decltype(v)> e5 = {a, v};
    [[maybe_unused]] Encounter<decltype(safe), decltype(v)> e6 = {safe, v};
#endif

    // TEST #602
    // checks running Encounters correctly between
    // members and safe treasures.

#if TEST_NUM == 602
    auto test = [] () constexpr {
        std::tuple<
            std::tuple<int, int>,
            std::tuple<int, int, uint32_t>,
            std::tuple<int, int, uint32_t>
        > result;

        { // set 1
            SafeTreasure<int> t {10};
            Explorer<int> exp;
            Encounter<decltype(t), decltype(exp)> e = {t, exp};
            run(e);

            auto &r = get<0>(result);
            get<0>(r) = t.evaluate();   // 0
            get<1>(r) = exp.pay();      // 10
        } // set 1

        { // set 2
            SafeTreasure<int> t {10};
            Adventurer<int, true> a {6};
            Encounter<decltype(a), decltype(t)> e = {a, t};
            run(e);

            auto &r = get<1>(result);
            get<0>(r) = t.evaluate();       // 0
            get<1>(r) = a.pay();            // 10
            get<2>(r) = a.getStrength();    // 6
        } // set 2

        { // set 3
            SafeTreasure<int> t {10};
            Veteran<int, 3> v;
            Encounter<decltype(t), decltype(v)> e = {t, v};
            run(e);

            auto &r = get<2>(result);
            get<0>(r) = t.evaluate();       // 0
            get<1>(r) = v.pay();            // 10
            get<2>(r) = v.getStrength();    // 2
        } // set 3

        return result;
    };

    static_assert(test() == std::make_tuple(
        std::make_tuple(0, 10),     // set 1
        std::make_tuple(0, 10, 6),  // set 2
        std::make_tuple(0, 10, 2)   // set 3
    ));
#endif

    // TEST #603
    // checks running Encounters correctly between
    // members and trapped treasures.

#if TEST_NUM == 603
    auto test = [] () constexpr {
        std::tuple<
            std::tuple<int, int>,
            std::tuple<int, int, uint32_t>,
            std::tuple<int, int, uint32_t>
        > result;

        { // set 1
            TrappedTreasure<int> t {10};
            Explorer<int> exp;
            Encounter<decltype(t), decltype(exp)> e = {t, exp};
            run(e);

            auto &r = get<0>(result);
            get<0>(r) = t.evaluate();   // 10
            get<1>(r) = exp.pay();      // 0
        } // set 1

        { // set 2
            TrappedTreasure<int> t {10};
            Adventurer<int, true> a {6};
            Encounter<decltype(a), decltype(t)> e = {a, t};
            run(e);

            auto &r = get<1>(result);
            get<0>(r) = t.evaluate();       // 0
            get<1>(r) = a.pay();            // 10
            get<2>(r) = a.getStrength();    // 3
        } // set 2

        { // set 3
            TrappedTreasure<int> t {10};
            Veteran<int, 3> v;
            Encounter<decltype(t), decltype(v)> e = {t, v};
            run(e);

            auto &r = get<2>(result);
            get<0>(r) = t.evaluate();       // 0
            get<1>(r) = v.pay();            // 10
            get<2>(r) = v.getStrength();    // 2
        } // set 3

        return result;
    };

    static_assert(test() == std::make_tuple(
        std::make_tuple(10, 0),     // set 1
        std::make_tuple(0, 10, 3),  // set 2
        std::make_tuple(0, 10, 2)   // set 3
    ));
#endif

    // TEST #604
    // checks running Encounters correctly between
    // two expedition members.

#if TEST_NUM == 604
    auto test = [] () constexpr {
        std::tuple<
            std::tuple<int, int>,
            std::tuple<int, int>,
            std::tuple<int, int>,
            std::tuple<int, int>
        > result;

        { // set 1 - Explorer vs Adventurer (armed)
            SafeTreasure<int> t1 {10};
            Explorer<int> exp;
            Encounter<decltype(t1), decltype(exp)> e1 = {t1, exp};
            run(e1);

            SafeTreasure<int> t2 {10};
            Adventurer<int, true> a {6};
            Encounter<decltype(a), decltype(t2)> e2 = {a, t2};
            run(e2);

            Encounter<decltype(a), decltype(exp)> e = {a, exp};
            run(e);

            auto &r = get<0>(result);
            get<0>(r) = a.pay();    // 20
            get<1>(r) = exp.pay();  // 0
        } // set 1

        { // set 2 - Explorer vs Explorer
            SafeTreasure<int> t1 {10};
            Explorer<int> exp1;
            Encounter<decltype(t1), decltype(exp1)> e1 = {t1, exp1};
            run(e1);

            SafeTreasure<int> t2 {10};
            Explorer<int> exp2;
            Encounter<decltype(t2), decltype(exp2)> e2 = {t2, exp2};
            run(e2);

            Encounter<decltype(exp1), decltype(exp2)> e = {exp1, exp2};
            run(e);

            auto &r = get<1>(result);
            get<0>(r) = exp1.pay(); // 10
            get<1>(r) = exp2.pay(); // 10
        } // set 2

        { // set 3 - Adventurer (armed) vs Adventure (armed)
            SafeTreasure<int> t1 {10};
            Adventurer<int, true> a1 {3};
            Encounter<decltype(a1), decltype(t1)> e1 = {a1, t1};
            run(e1);

            SafeTreasure<int> t2 {10};
            Adventurer<int, true> a2 {6};
            Encounter<decltype(a2), decltype(t2)> e2 = {a2, t2};
            run(e2);

            Encounter<decltype(a1), decltype(a2)> e = {a1, a2};
            run(e);

            auto &r = get<2>(result);
            get<0>(r) = a1.pay(); // 0
            get<1>(r) = a2.pay(); // 20
        } // set 3

        { // set 4 - as above, but equal strength
            SafeTreasure<int> t1 {10};
            Adventurer<int, true> a1 {6};
            Encounter<decltype(a1), decltype(t1)> e1 = {a1, t1};
            run(e1);

            SafeTreasure<int> t2 {10};
            Adventurer<int, true> a2 {6};
            Encounter<decltype(a2), decltype(t2)> e2 = {a2, t2};
            run(e2);

            Encounter<decltype(a1), decltype(a2)> e = {a1, a2};
            run(e);

            auto &r = get<3>(result);
            get<0>(r) = a1.pay(); // 10
            get<1>(r) = a2.pay(); // 10
        }

        return result;
    };

    static_assert(test() == std::make_tuple(
        std::make_tuple(20, 0),     // set 1
        std::make_tuple(10, 10),    // set 2
        std::make_tuple(0, 20),     // set 3
        std::make_tuple(10, 10)     // set 4
    ));
#endif

    // TEST #605
    // checks expedition with one member
    // and only safe treasures.

#if TEST_NUM == 605
    auto test = [] () constexpr {
        Explorer<int> exp;
        SafeTreasure<int> t1 {1};
        SafeTreasure<int> t2 {1};
        SafeTreasure<int> t3 {1};

        Encounter<decltype(exp), decltype(t1)> e1 = {exp, t1};
        Encounter<decltype(exp), decltype(t2)> e2 = {exp, t2};
        Encounter<decltype(exp), decltype(t3)> e3 = {exp, t3};

        expedition(e1, e2, e3);

        return exp.pay();
    };

    static_assert(test() == 3);
#endif

    // TEST #606
    // checks expedition with one member whose strength
    // gets lowered to zero.

#if TEST_NUM == 606
    auto test = [] () constexpr {
        Adventurer<int, true> a {1};
        TrappedTreasure<int> t1 {1};
        TrappedTreasure<int> t2 {10};
        SafeTreasure<int> t3 {100};

        Encounter<decltype(a), decltype(t1)> e1 = {a, t1};
        Encounter<decltype(a), decltype(t2)> e2 = {a, t2};
        Encounter<decltype(a), decltype(t3)> e3 = {a, t3};

        expedition(e1, e2, e3);

        return a.pay();
    };

    static_assert(test() == 101);
#endif

    // TEST #607
    // checks expedition with two adventurers who fight.

#if TEST_NUM == 607
    auto test = [] () constexpr {
        Adventurer<int, true> a1 {10};
        Adventurer<int, true> a2 {6};
        TrappedTreasure<int> t1 {10};
        SafeTreasure<int> t2 {10};

        Encounter<decltype(a1), decltype(t1)> e1 = {a1, t1};
        Encounter<decltype(a2), decltype(t2)> e2 = {a2, t2};
        Encounter<decltype(a1), decltype(a2)> e3 = {a1, a2};

        expedition(e1, e2, e3);

        return a2.pay();
    };

    static_assert(test() == 20);
#endif

    // TEST #608
    // checks expedition with many members and a veteran
    // who steals everything in the end.

#if TEST_NUM == 608
    auto test = [] () constexpr {
        Adventurer<int, true> a1 {10};
        Adventurer<int, true> a2 {6};
        Veteran<int, 6> v;
        TrappedTreasure<int> t1 {10};
        SafeTreasure<int> t2 {10};
        TrappedTreasure<int> t3 {10};

        Encounter<decltype(v), decltype(t3)> e1 = {v, t3};
        Encounter<decltype(v), decltype(a1)> e2 = {v, a1};
        Encounter<decltype(a1), decltype(t1)> e3 = {a1, t1};
        Encounter<decltype(a2), decltype(t2)> e4 = {a2, t2};
        Encounter<decltype(v), decltype(a2)> e5 = {v, a2};
        Encounter<decltype(v), decltype(a1)> e6 = {v, a1};

        expedition(e1, e2, e3, e4, e5, e6);

        return v.pay();
    };

    static_assert(test() == 30);
#endif

    // TEST #609
    // checks expedition with no encounters to run.

#if TEST_NUM == 609
    expedition();
#endif

    // TEST #610
    // should not compile, because
    // incorrect call to run() with two treasures.

#if TEST_NUM == 610
    SafeTreasure<int> safe {100};
    TrappedTreasure<int> trapped {999};
    Encounter<decltype(safe), decltype(trapped)> e = {safe, trapped};

    run(e);
#endif
} // Test Group 6

int main() {
    testGroup1();
    testGroup2();
    testGroup3();
    testGroup4();
    testGroup5();
    testGroup6();
}
