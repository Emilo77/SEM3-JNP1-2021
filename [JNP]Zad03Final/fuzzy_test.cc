/*
 * File:       fuzzy_test.cc
 * Created on: 4th November 2021
 * Author:     Przemysław Rutka
 */

/* Tests description
 * Group 1 - fuzzy_example.cc and stream operator
 * Group 2 - constructors, assignments, arithmetics,
             order of parameters of fuzzy numbers
 * Group 3 - comparison operators
 * Group 4 - sets of fuzzy numbers
 * Group 5 - compile time evaluations, const correctness,
             crisp_zero inlining
 * Group 6 - non-compiling codes
 */

#include "fuzzy.h"

#if (TEST_NUM >= 101 && TEST_NUM <= 404) \
    || (TEST_NUM >= 502 && TEST_NUM <= 503) \
    || TEST_NUM == 505
  #include <cassert>
#endif

#if TEST_NUM == 101
  #include <iostream>
#endif

#if TEST_NUM == 102
  #include <sstream>
#endif

#if TEST_NUM == 505
  extern TriFuzzyNum const * crisp_zero_addr();
#endif

namespace {

#if TEST_NUM == 101
  using std::cout;
  using std::endl;
#endif

#if TEST_NUM == 102
  using std::ostringstream;
#endif

#if TEST_NUM == 201 || (TEST_NUM >= 401 && TEST_NUM <= 403)
  using std::move;
#endif

#if TEST_NUM == 302
  using std::partial_ordering;
#endif

#if TEST_NUM == 401 || TEST_NUM == 402
  using std::length_error;
  using std::runtime_error;
#endif

#if TEST_NUM == 404
  using std::length_error;
#endif

#if TEST_NUM == 501
  constexpr TriFuzzyNum num1(1, 2, 3);
  constexpr TriFuzzyNum num2(-3, -2, -1);
#endif

#if TEST_NUM >= 201 && TEST_NUM <= 204 || TEST_NUM == 403 \
    || TEST_NUM == 404 || TEST_NUM == 503
  // Checks if parameters of the fuzzy number x are equal to l, m, and u.
  [[maybe_unused]] bool check(TriFuzzyNum const & x,
                              real_t l, real_t m, real_t u) {
    return x.lower_value() == l
           && x.modal_value() == m
           && x.upper_value() == u;
  }
#endif

}; // anonymous namespace

int main() {

// TEST: fuzzy_example.cc without couts
#if TEST_NUM == 101
  TriFuzzyNum num1(1, 2, 3);
  TriFuzzyNum num2(0.5, 0.25, 0.75);
  constexpr TriFuzzyNum num3(1, 1, 1);

  // Using cout have been moved to the stream operator test.

  assert(num1 + num2 == TriFuzzyNum(1.25, 2.5, 3.75));
  assert(num1 - num2 == TriFuzzyNum(0.25, 1.5, 2.75));
  assert(num1 * num2 == TriFuzzyNum(0.25, 1, 2.25));

  assert(num1 > num2);
  assert(num1 >= num2);
  assert(num1 != num2);
  assert(num2 < num1);
  assert(num2 <= num1);
  assert(num3 == crisp_number(1));
  static_assert(num3 == crisp_number(1));

  assert(num1 <=> num2 > 0);
  assert(num2 <=> num1 < 0);

  assert((num1 += num3) == TriFuzzyNum(2, 3, 4));
  assert((num1 -= num3) == TriFuzzyNum(1, 2, 3));
  assert((num1 *= num3) == num1);
  assert((num1 += crisp_zero) == num1);

  static_assert(num3.lower_value() == 1);
  static_assert(num3.modal_value() == 1);
  static_assert(num3.upper_value() == 1);

  TriFuzzyNumSet fn_set1({TriFuzzyNum(1, 2, 4), TriFuzzyNum(1, 3, 6)});
  assert(fn_set1.arithmetic_mean() == TriFuzzyNum(1, 2.5, 5));

  fn_set1.insert(TriFuzzyNum(1, 4, 5));
  assert(fn_set1.arithmetic_mean() == TriFuzzyNum(1, 3, 5));

  fn_set1.remove(TriFuzzyNum(1, 3, 6));
  assert(fn_set1.arithmetic_mean() == TriFuzzyNum(1, 3, 4.5));

  TriFuzzyNumSet fn_set2({TriFuzzyNum(1, 2, 3),
                          TriFuzzyNum(1, 2, 3),
                          TriFuzzyNum(1, 5, 6)});
  assert(fn_set2.arithmetic_mean() == TriFuzzyNum(1, 3, 4));

  constinit static TriFuzzyNum num4(1.25, 2.25, 3.25);
  num4 += crisp_number(0.25);
  assert(num4 == TriFuzzyNum(1.5, 2.5, 3.5));
#endif // 101

// TEST: stream operator
#if TEST_NUM == 102
  TriFuzzyNum num1(1, 2, 3);
  ostringstream sout1;
  sout1 << num1;
  assert(sout1.str().compare("(1, 2, 3)") == 0
         || sout1.str().compare("(1, 2, 3)") == 0);

  TriFuzzyNum num2(0.5, 0.25, 0.75);
  ostringstream sout2;
  sout2 << num2;
  assert(sout2.str().compare("(0.25, 0.5, 0.75)") == 0
         || sout2.str().compare("(0.25, 0.5, 0.75)") == 0);

  constexpr TriFuzzyNum num3(1, 1, 1);
  ostringstream sout3;
  sout3 << num3;
  assert(sout3.str().compare("(1, 1, 1)") == 0
         || sout3.str().compare("(1, 1, 1)") == 0);

  TriFuzzyNum const num4(-1.25, 0, 1.25);
  ostringstream sout4;
  sout4 << num4;
  assert(sout4.str().compare("(-1.25, 0, 1.25)") == 0
         || sout4.str().compare("(-1.25, 0, 1.25)") == 0);

  constinit static TriFuzzyNum num5(-3.25, -1, -0.75);
  ostringstream sout5;
  sout5 << num5;
  assert(sout5.str().compare("(-3.25, -1, -0.75)") == 0
         || sout5.str().compare("(-3.25, -1, -0.75)") == 0);
#endif // 102

// TEST: constructors of fuzzy numbers
#if TEST_NUM == 201
  TriFuzzyNum num1(1, 2, 3);
  assert(check(num1, 1, 2, 3));

  TriFuzzyNum const num2(-1, 0, 1);
  assert(check(num2, -1, 0, 1));

  constexpr TriFuzzyNum num3(0.25, 0.5, 0.75);
  assert(check(num3, 0.25, 0.5, 0.75));

  constinit static TriFuzzyNum num4(-3.125, -2.125, -1.125);
  assert(check(num4, -3.125, -2.125, -1.125));

  TriFuzzyNum num5a(num1);
  assert(check(num5a, 1, 2, 3) && check(num1, 1, 2, 3));
  TriFuzzyNum num5b = num1;
  assert(check(num5b, 1, 2, 3) && check(num1, 1, 2, 3));

  TriFuzzyNum num6a(num2);
  assert(check(num6a, -1, 0, 1) && check(num2, -1, 0, 1));
  TriFuzzyNum num6b = num2;
  assert(check(num6b, -1, 0, 1) && check(num2, -1, 0, 1));

  TriFuzzyNum num7a(num3);
  assert(check(num7a, 0.25, 0.5, 0.75) && check(num3, 0.25, 0.5, 0.75));
  TriFuzzyNum num7b = num3;
  assert(check(num7b, 0.25, 0.5, 0.75) && check(num3, 0.25, 0.5, 0.75));

  TriFuzzyNum num8a(num4);
  assert(check(num8a, -3.125, -2.125, -1.125)
         && check(num4, -3.125, -2.125, -1.125));
  TriFuzzyNum num8b = num4;
  assert(check(num8b, -3.125, -2.125, -1.125)
         && check(num4, -3.125, -2.125, -1.125));

  TriFuzzyNum num9a(move(num5a));
  assert(check(num9a, 1, 2, 3));
  TriFuzzyNum num9b = move(num5b);
  assert(check(num9b, 1, 2, 3));

  TriFuzzyNum num10a(move(num2));
  assert(check(num10a, -1, 0, 1) && check(num2, -1, 0, 1));
  TriFuzzyNum num10b = move(num2);
  assert(check(num10b, -1, 0, 1) && check(num2, -1, 0, 1));

  TriFuzzyNum num11a(move(num3));
  assert(check(num11a, 0.25, 0.5, 0.75) && check(num3, 0.25, 0.5, 0.75));
  TriFuzzyNum num11b = move(num3);
  assert(check(num11b, 0.25, 0.5, 0.75) && check(num3, 0.25, 0.5, 0.75));

  TriFuzzyNum num12a(move(num8a));
  assert(check(num12a, -3.125, -2.125, -1.125));
  TriFuzzyNum num12b = move(num8b);
  assert(check(num12b, -3.125, -2.125, -1.125));

  assert(check(::crisp_zero, 0, 0, 0));

  // Expected to work without any error
  TriFuzzyNum inv1(-1.0 / 0.0, 0.0 / 0.0, 1.0 / 0.0);
  [[maybe_unused]] TriFuzzyNum inv2(inv1);
  [[maybe_unused]] TriFuzzyNum inv3 = inv1;
#endif // 201

// TEST: assignments
#if TEST_NUM == 202
  TriFuzzyNum num1(1, 2, 3);
  TriFuzzyNum num2(0.125, 0.5, 0.625);
  TriFuzzyNum num3(num1);
  TriFuzzyNum num4(-0.125, 0.25, 0.375);

  num1 += num2;
  assert(check(num1, 1.125, 2.5, 3.625) && check(num2, 0.125, 0.5, 0.625));
  num1 -= num2;
  assert(check(num1, 0.5, 2, 3.5) && check(num2, 0.125, 0.5, 0.625));
  num1 = num3;
  assert(check(num1, 1, 2, 3) && check(num3, 1, 2, 3));
  num2 *= num1;
  assert(check(num2, 0.125, 1, 1.875) && check(num1, 1, 2, 3));
  num1 += num1;
  assert(check(num1, 2, 4, 6));
  num1 *= num1;
  assert(check(num1, 4, 16, 36));
  num1 = num3;
  num1 -= num1;
  assert(check(num1, -2, 0, 2));
  num3 = num4;
  assert(check(num3, -0.125, 0.25, 0.375) && check(num4, -0.125, 0.25, 0.375));
  num4 += num4;
  assert(check(num3, -0.125, 0.25, 0.375) && check(num4, -0.25, 0.5, 0.75));
  num3 = num3;
  assert(check(num3, -0.125, 0.25, 0.375));
#endif // 202

// TEST: arithmetics
#if TEST_NUM == 203
  TriFuzzyNum num1(1, 2, 3);
  TriFuzzyNum num2(0.125, 0.5, 0.625);
  assert(check(num1 + num2, 1.125, 2.5, 3.625) && check(num1, 1, 2, 3)
         && check(num2, 0.125, 0.5, 0.625));
  assert(check(num1 - num2, 0.375, 1.5, 2.875) && check(num1, 1, 2, 3)
         && check(num2, 0.125, 0.5, 0.625));
  assert(check(num1 * num2, 0.125, 1, 1.875) && check(num1, 1, 2, 3)
         && check(num2, 0.125, 0.5, 0.625));
  assert(check(num1 + num1, 2, 4, 6) && check(num1, 1, 2, 3));
  assert(check(num1 - num1, -2, 0, 2) && check(num1, 1, 2, 3));
  assert(check(num1 * num1, 1, 4, 9) && check(num1, 1, 2, 3));

  TriFuzzyNum num3(-3, -2, -1);
  TriFuzzyNum num4(-0.625, -0.5, -0.125);
  assert(check(num3 + num4, -3.625, -2.5, -1.125) && check(num3, -3, -2, -1)
         && check(num4, -0.625, -0.5, -0.125));
  assert(check(num3 - num4, -2.875, -1.5, -0.375) && check(num3, -3, -2, -1)
         && check(num4, -0.625, -0.5, -0.125));
  assert(check(num3 * num4, 0.125, 1, 1.875) && check(num3, -3, -2, -1)
         && check(num4, -0.625, -0.5, -0.125));
  assert(check(num3 + num3, -6, -4, -2) && check(num3, -3, -2, -1));
  assert(check(num3 - num3, -2, 0, 2) && check(num3, -3, -2, -1));
  assert(check(num3 * num3, 1, 4, 9) && check(num3, -3, -2, -1));
#endif // 203

// TEST: the order of parameters in a triangular fuzzy number
#if TEST_NUM == 204
  TriFuzzyNum num1(1, 2, 3);
  TriFuzzyNum num2(1, 3, 2);
  TriFuzzyNum num3(2, 1, 3);
  TriFuzzyNum num4(2, 3, 1);
  TriFuzzyNum num5(3, 1, 2);
  TriFuzzyNum num6(3, 2, 1);
  assert(check(num1, 1, 2, 3));
  assert(check(num2, 1, 2, 3));
  assert(check(num3, 1, 2, 3));
  assert(check(num4, 1, 2, 3));
  assert(check(num5, 1, 2, 3));
  assert(check(num6, 1, 2, 3));

  TriFuzzyNum num7(-1, -2, -3);
  TriFuzzyNum num8(-1, -3, -2);
  TriFuzzyNum num9(-2, -1, -3);
  TriFuzzyNum num10(-2, -3, -1);
  TriFuzzyNum num11(-3, -1, -2);
  TriFuzzyNum num12(-3, -2, -1);
  assert(check(num7, -3, -2, -1));
  assert(check(num8, -3, -2, -1));
  assert(check(num9, -3, -2, -1));
  assert(check(num10, -3, -2, -1));
  assert(check(num11, -3, -2, -1));
  assert(check(num12, -3, -2, -1));

  TriFuzzyNum num13(-1, 0, 1);
  assert(check(num13 * num13, 0, 1, 1));
  num13 *= num13;
  assert(check(num13, 0, 1, 1));
#endif // 204

// TEST: comparisons of fuzzy numbers
#if TEST_NUM == 301
  TriFuzzyNum num1(1, 2, 3);
  TriFuzzyNum num2(1, 2.125, 3);
  TriFuzzyNum num3(1.125, 2, 3);
  TriFuzzyNum num4(1, 2, 2.875);
  TriFuzzyNum num5(1.125, 2, 2.875);
  TriFuzzyNum num6(2, 3, 4);
  TriFuzzyNum num7(-2, -1, 0);
  TriFuzzyNum num8(2.5, 2.5, 2.5);
  TriFuzzyNum num9(1.5, 1.5, 1.5);
  TriFuzzyNum num10(1, 2, 3);
  TriFuzzyNum num11(2.5, 2.5, 2.5);

  assert(num1 < num2 && num1 <= num2 && num1 != num2
         && num2 >= num1 && num2 > num1 && !(num1 == num2));
  assert(num1 <=> num2 < 0);

  assert(num1 < num3 && num1 <= num3 && num1 != num3
         && num3 >= num1 && num3 > num1 && !(num1 == num3));
  assert(num1 <=> num3 < 0);

  assert(num4 < num1 && num4 <= num1 && num1 != num4
         && num1 >= num4 && num1 > num4 && !(num1 == num4));
  assert(num1 <=> num4 > 0);

  assert(num1 < num5 && num1 <= num5 && num1 != num5
         && num5 >= num1 && num5 > num1 && !(num1 == num5));
  assert(num5 <=> num1 > 0);

  assert(num1 < num6 && num1 <= num6 && num1 != num6
         && num6 >= num1 && num6 > num1 && !(num1 == num6));
  assert(num1 <=> num6 < 0);

  assert(num7 < num1 && num7 <= num1 && num1 != num7
         && num1 >= num7 && num1 > num7 && !(num1 == num7));
  assert(num7 <=> num1 < 0);

  assert(num1 < num8 && num1 <= num8 && num1 != num8
         && num8 >= num1 && num8 > num1 && !(num1 == num8));
  assert(num1 <=> num8 < 0);

  assert(num9 < num1 && num9 <= num1 && num1 != num9
         && num1 >= num9 && num1 > num9 && !(num1 == num9));
  assert(num1 <=> num9 > 0);

  assert(num9 < num8 && num9 <= num8 && num8 != num9
         && num8 >= num9 && num8 > num9 && !(num8 == num9));
  assert(num8 <=> num9 > 0);

  assert(num1 == num1 && num1 <= num1 && num1 >= num1
         && !(num1 < num1) && !(num1 > num1) && !(num1 != num1));
  assert(num1 <=> num1 == 0);

  assert(num8 == num8 && num8 <= num8 && num8 >= num8
         && !(num8 < num8) && !(num8 > num8) && !(num8 != num8));
  assert(num8 <=> num8 == 0);

  assert(num1 == num10 && num1 <= num10 && num1 >= num10
         && !(num1 < num10) && !(num1 > num10) && !(num1 != num10));
  assert(num1 <=> num10 == 0);
  assert(num10 <=> num1 == 0);

  assert(num8 == num11 && num8 <= num11 && num8 >= num11
         && !(num8 < num11) && !(num8 > num11) && !(num8 != num11));
  assert(num8 <=> num11 == 0);
  assert(num11 <=> num8 == 0);

  constexpr TriFuzzyNum num12(1.5, 2.5, 3.5);
  constexpr TriFuzzyNum num13(1, 2, 3);
  assert(num1 < num12 && num1 <= num12 && num1 != num12
         && num12 >= num1 && num12 > num1 && !(num1 == num12));
  assert(num1 <=> num12 < 0);
  assert(num1 == num13 && num1 <= num13 && num1 >= num13
         && !(num1 < num13) && !(num1 > num13) && !(num1 != num13));
  assert(num1 <=> num13 == 0);

  constinit static TriFuzzyNum num14(1.5, 2.5, 3.5);
  constinit static TriFuzzyNum num15(1, 2, 3);
  assert(num1 < num14 && num1 <= num14 && num1 != num14
         && num14 >= num1 && num14 > num1 && !(num1 == num14));
  assert(num1 <=> num14 < 0);
  assert(num1 == num15 && num1 <= num15 && num1 >= num15
         && !(num1 < num15) && !(num1 > num15) && !(num1 != num15));
  assert(num1 <=> num15 == 0);
  assert(num13 == num15 && num13 <= num15 && num13 >= num15
         && !(num13 < num15) && !(num13 > num15) && !(num13 != num15));
  assert(num13 <=> num15 == 0);
#endif // 301

// TEST: incomparable fuzzy numbers
#if TEST_NUM == 302
  TriFuzzyNum inv1(1.0 / 0.0, 2.0 / 0.0, 3.0 / 0.0);
  TriFuzzyNum inv2(-3.0 / 0.0, -2.0/ 0.0, -1.0 / 0.0);
  TriFuzzyNum inv3(0.0 / 0.0, 0.0 / 0.0, 0.0 / 0.0);
  TriFuzzyNum inv4(-0.25 / 0.0, 0.0 / 0.0, 1.5 / 0.0);

  assert(inv1 != inv2 && !(inv1 < inv2) && !(inv1 <= inv2)
         && !(inv1 > inv2) && !(inv1 >= inv2) && !(inv1 == inv2));
  assert(inv1 != inv3 && !(inv1 < inv3) && !(inv1 <= inv3)
         && !(inv1 > inv3) && !(inv1 >= inv3) && !(inv1 == inv3));
  assert(inv2 != inv3 && !(inv2 < inv3) && !(inv2 <= inv3)
         && !(inv2 > inv3) && !(inv2 >= inv3) && !(inv2 == inv3));
  assert(inv4 != inv4 && !(inv4 < inv4) && !(inv4 <= inv4)
         && !(inv4 > inv4) && !(inv4 >= inv4) && !(inv4 == inv4));
  assert(inv1 <=> inv2 == partial_ordering::unordered);
  assert(inv1 <=> inv3 == partial_ordering::unordered);
  assert(inv2 <=> inv3 == partial_ordering::unordered);
  assert(inv4 <=> inv4 == partial_ordering::unordered);
#endif // 302

// TEST: set constructors
// No exception is expected in this test
#if TEST_NUM == 401
  TriFuzzyNumSet set1;
  TriFuzzyNumSet set2({});

  try {
    [[maybe_unused]] auto num1 = set1.arithmetic_mean();
    throw runtime_error("set1 should be empty");
  } catch (length_error const & e) {}

  try {
    [[maybe_unused]] auto num2 = set2.arithmetic_mean();
    throw runtime_error("set2 should be empty");
  } catch (length_error const & e) {}

  TriFuzzyNumSet set3({TriFuzzyNum(1, 2, 3)});
  TriFuzzyNumSet set4({TriFuzzyNum(1, 2, 3),
                       TriFuzzyNum(-3, -2, -1),
                       TriFuzzyNum(1, 2, 3)});

  TriFuzzyNumSet set5(set3);
  TriFuzzyNumSet set6 = set4;
  assert(set5.arithmetic_mean() == set3.arithmetic_mean());
  assert(set6.arithmetic_mean() == set4.arithmetic_mean());

  TriFuzzyNumSet set7(move(set3));
  TriFuzzyNumSet set8 = move(set4);
  assert(set7.arithmetic_mean() == set5.arithmetic_mean());
  assert(set8.arithmetic_mean() == set6.arithmetic_mean());

  try {
    [[maybe_unused]] auto num3 = set3.arithmetic_mean();
    throw runtime_error("set3 should be empty");
  } catch (length_error const & e) {}

  try {
    [[maybe_unused]] auto num4 = set4.arithmetic_mean();
    throw runtime_error("set4 should be empty");
  } catch (length_error const & e) {}

  TriFuzzyNum const num5(1, 2, 4);
  TriFuzzyNum const num6(1, 3, 6);
  TriFuzzyNumSet set9({num5, num6});
  assert(set9.arithmetic_mean() == TriFuzzyNum(1, 2.5, 5));
#endif // 401

// TEST: set assignments
// No exception is expected in this test.
#if TEST_NUM == 402
  TriFuzzyNumSet set1({TriFuzzyNum(1, 3, 6), TriFuzzyNum(1, 2, 4)});
  TriFuzzyNumSet set2;
  TriFuzzyNumSet set3({TriFuzzyNum(1, 2, 3)});

  set2 = set1;
  assert(set2.arithmetic_mean() == set1.arithmetic_mean());

  set3 = move(set1);
  assert(set3.arithmetic_mean() == set2.arithmetic_mean());
  try {
    [[maybe_unused]] auto num = set1.arithmetic_mean();
    throw runtime_error("set1 should be empty");
  } catch (length_error const & e) {}
#endif // 402

// TEST: inserting into a set
#if TEST_NUM == 403
  TriFuzzyNum num1(1, 2, 3);
  TriFuzzyNum num2(-1, 0, 5);
  TriFuzzyNum num3(-3, -2, 1);
  TriFuzzyNum num4(-1, 0, 3);
  TriFuzzyNumSet set1({num1, num2});
  assert(check(set1.arithmetic_mean(), 0, 1, 4));
  set1.insert(num3);
  assert(check(set1.arithmetic_mean(), -1, 0, 3));
  set1.insert(move(num4));
  assert(check(set1.arithmetic_mean(), -1, 0, 3));

  TriFuzzyNum const num5(1, 2, 4);
  TriFuzzyNum const num6(1, 3, 6);
  TriFuzzyNumSet set2({num5, num6});
  set2.insert(num1);

  TriFuzzyNumSet set3({num5});
  set3.insert(num6);
  assert(set3.arithmetic_mean() == TriFuzzyNum(1, 2.5, 5));
#endif // 403

// TEST: removing from a set
// No exception is expected in this test.
#if TEST_NUM == 404
  TriFuzzyNum num1(1, 2, 3);
  TriFuzzyNum num2(-1, 0, 5);
  TriFuzzyNum num3(-3, -2, 1);
  TriFuzzyNum num4(-1, 0, 3);
  TriFuzzyNumSet set1({num1, num3, num2});
  assert(check(set1.arithmetic_mean(), -1, 0, 3));
  set1.remove(num3);
  assert(check(set1.arithmetic_mean(), 0, 1, 4));
  set1.remove(num4);
  assert(check(set1.arithmetic_mean(), 0, 1, 4));
  set1.remove(TriFuzzyNum(-1, 0, 5));
  assert(check(set1.arithmetic_mean(), 1, 2, 3));

  set1.insert(num1);
  set1.insert(TriFuzzyNum(1, 2, 3));
  assert(check(set1.arithmetic_mean(), 1, 2, 3));
  set1.remove(num1);
  set1.remove(TriFuzzyNum(1, 2, 3));
  try {
    [[maybe_unused]] auto num = set1.arithmetic_mean(); // Throws if removed all.
    assert(check(num, 1, 2, 3));
  } catch (length_error const & e) {}

  TriFuzzyNum const num5(1, 2, 4);
  TriFuzzyNum const num6(1, 3, 6);
  TriFuzzyNumSet set2({num5, num6});
  set2.remove(num5);
  assert(set2.arithmetic_mean() == TriFuzzyNum(1, 3, 6));
#endif // 404

// TEST: compile time evaluations
#if TEST_NUM == 501
  constexpr TriFuzzyNum num3(-1.75, 0, 2.375);

  static_assert(num1.lower_value() == 1);
  static_assert(num1.modal_value() == 2);
  static_assert(num1.upper_value() == 3);

  static_assert(num2.lower_value() == -3);
  static_assert(num2.modal_value() == -2);
  static_assert(num2.upper_value() == -1);

  static_assert(num3.lower_value() == -1.75);
  static_assert(num3.modal_value() == 0);
  static_assert(num3.upper_value() == 2.375);

  static_assert(::crisp_number(0.25).lower_value() == 0.25);
  static_assert(::crisp_number(0.25).modal_value() == 0.25);
  static_assert(::crisp_number(0.25).upper_value() == 0.25);
#endif // 501

// TEST: const correctness of methods
#if TEST_NUM == 502
  TriFuzzyNum const num1(1, 2, 3);
  TriFuzzyNum const num2(0.5, 1, 1.5);

  assert(num1.lower_value() == 1);
  assert(num1.modal_value() == 2);
  assert(num1.upper_value() == 3);

  assert(num1 > num2);
  assert(num1 >= num2);
  assert(num2 < num1);
  assert(num2 <= num1);
  assert(num1 != num2);
  assert(!(num1 == num2));

  TriFuzzyNumSet const set({TriFuzzyNum(1, 2, 4), TriFuzzyNum(1, 3, 6)});
  [[maybe_unused]] auto res = set.arithmetic_mean();
#endif // 502

// TEST: const correctness of assignments
#if TEST_NUM == 503
  TriFuzzyNum num1(1, 2, 3);
  TriFuzzyNum num2(-1, 0, 1);
  TriFuzzyNum num3(3, 3.5, 4);
  assert(check(num1 = num2 = num3, 3, 3.5, 4));
  assert(check(num1, 3, 3.5, 4));
  assert(check(num2, 3, 3.5, 4));
  assert(check(num1 = num2 = TriFuzzyNum(-3, -2, -1), -3, -2, -1));
  assert(check(num1, -3, -2, -1));
  assert(check(num2, -3, -2, -1));
  assert(check((num1 = num2) = num3, 3, 3.5, 4));
  assert(check(num1, 3, 3.5, 4));
  assert(check(num2, -3, -2, -1));
  assert(check((num1 += num2) = num3, 3, 3.5, 4));
  assert(check(num1, 3, 3.5, 4));
  assert(check(num2, -3, -2, -1));
  assert(check((num1 -= num2) = num3, 3, 3.5, 4));
  assert(check(num1, 3, 3.5, 4));
  assert(check(num2, -3, -2, -1));
  assert(check((num1 *= num2) = num3, 3, 3.5, 4));
  assert(check(num1, 3, 3.5, 4));
  assert(check(num2, -3, -2, -1));

  TriFuzzyNum const num4(1, 1.5, 2);
  [[maybe_unused]] auto res = num4;
  res += num4;
  res -= num4;
  res *= num4;

  TriFuzzyNumSet set1({num1});
  TriFuzzyNumSet set2({num2});
  TriFuzzyNumSet set3({num3});
  assert(check((set1 = set2 = set3).arithmetic_mean(), 3, 3.5, 4));
  assert(check(set1.arithmetic_mean(), 3, 3.5, 4));
  assert(check(set2.arithmetic_mean(), 3, 3.5, 4));
  assert(check(
    ((set1 = TriFuzzyNumSet({TriFuzzyNum(1, 2, 3)})) = set3).arithmetic_mean(),
    3, 3.5, 4));
  assert(check(set1.arithmetic_mean(), 3, 3.5, 4));
  set2 = TriFuzzyNumSet({TriFuzzyNum(1, 2, 3)});
  assert(check(((set1 = set2) = set3).arithmetic_mean(), 3, 3.5, 4));
  assert(check(set1.arithmetic_mean(), 3, 3.5, 4));
  assert(check(set2.arithmetic_mean(), 1, 2, 3));
#endif // 503

// TEST: const correctness of arithmetic operators
#if TEST_NUM == 504
  TriFuzzyNum const num1(1, 2, 3);
  TriFuzzyNum num2(0.5, 1, 1.5);
  TriFuzzyNum const num3(1.5, 2, 3);

  [[maybe_unused]] auto res = num1 + num2;
  res = num1 - num2;
  res = num1 * num2;
  res = num2 + num1;
  res = num2 - num1;
  res = num2 * num1;
  res = num1 + num3;
  res = num1 - num3;
  res = num1 * num3;
#endif

// TEST: one instance of crisp_zero
// This test should be linked with fuzzy_test_external.cc unit.
#if TEST_NUM == 505
  assert(&::crisp_zero == crisp_zero_addr());
#endif // 505

// The first one in this group must compile.
#if TEST_NUM == 601
  [[maybe_unused]] auto num = TriFuzzyNum(1, 2, 3);
#endif // 601

// TESTS: non-compiling constructors of fuzzy numbers
#if TEST_NUM == 602
  TriFuzzyNum num;
#endif // 602

#if TEST_NUM == 603
  TriFuzzyNum num(1);
#endif // 603

#if TEST_NUM == 604
  TriFuzzyNum num(1, 2);
#endif // 604

#if TEST_NUM == 605
  TriFuzzyNum num(1, 1, 1, 1);
#endif // 605

// TEST: non-compiling crisp_number calls
#if TEST_NUM == 606
  TriFuzzyNum num1(1, 2, 3);
  [[maybe_unused]] constexpr TriFuzzyNum num2 =
                                      ::crisp_number(num1.modal_value());
  [[maybe_unused]] TriFuzzyNum num3 = ::crisp_number(num1.upper_value());
#endif // 606

// TESTS: non-compiling constructors of sets
#if TEST_NUM == 607
  TriFuzzyNumSet set(TriFuzzyNum(1, 2, 3));
#endif // 607

#if TEST_NUM == 608
  TriFuzzyNumSet set(TriFuzzyNum(1, 2, 3),
                     TriFuzzyNum(-3, -2, -1),
                     TriFuzzyNum(1, 2, 3));
#endif // 608

// TESTS: non-compiling const correctness (assignments)
#if TEST_NUM == 609
  TriFuzzyNum const num(1, 2, 3);
  ::crisp_zero = num;
#endif // 609

#if TEST_NUM == 610
  ::crisp_zero = TriFuzzyNum(1, 2, 3);
#endif // 610

#if TEST_NUM == 611
  ::crisp_zero += TriFuzzyNum(1, 2, 3);
#endif // 611

#if TEST_NUM == 612
  ::crisp_zero -= TriFuzzyNum(1, 2, 3);
#endif // 612

#if TEST_NUM == 613
  ::crisp_zero *= TriFuzzyNum(1, 2, 3);
#endif // 613

// TEST: Non-compiling usage of a const set
#if TEST_NUM == 614
  TriFuzzyNumSet const set({TriFuzzyNum(1, 2, 4), TriFuzzyNum(1, 3, 6)});
  set.insert(TriFuzzyNum(1, 2, 3));
#endif // 614

#if TEST_NUM == 615
  TriFuzzyNumSet const set({TriFuzzyNum(1, 2, 4), TriFuzzyNum(1, 3, 6)});
  TriFuzzyNum num(1, 2, 3);
  set.insert(num);
#endif // 615

#if TEST_NUM == 616
  TriFuzzyNumSet const set({TriFuzzyNum(1, 2, 4), TriFuzzyNum(1, 3, 6)});
  TriFuzzyNum const num(1, 2, 3);
  set.insert(num);
#endif // 616

#if TEST_NUM == 617
  TriFuzzyNumSet const set({TriFuzzyNum(1, 2, 4), TriFuzzyNum(1, 3, 6)});
  set.remove(TriFuzzyNum(1, 2, 4));
#endif // 617

#if TEST_NUM == 618
  TriFuzzyNumSet const set({TriFuzzyNum(1, 2, 4), TriFuzzyNum(1, 3, 6)});
  TriFuzzyNum num(1, 2, 4);
  set.remove(num);
#endif // 618

#if TEST_NUM == 619
  TriFuzzyNumSet const set({TriFuzzyNum(1, 2, 4), TriFuzzyNum(1, 3, 6)});
  TriFuzzyNum const num(1, 2, 4);
  set.remove(num);
#endif // 619

#if TEST_NUM == 620
  TriFuzzyNumSet const set({TriFuzzyNum(1, 2, 4), TriFuzzyNum(1, 3, 6)});
  TriFuzzyNum num(1, 2, 3);
  set.remove(num);
#endif // 620


} // the end of main
