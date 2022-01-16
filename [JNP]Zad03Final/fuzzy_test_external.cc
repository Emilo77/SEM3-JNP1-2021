/*
 * File:       fuzzy_test_external.cc
 * Created on: 25th November 2021
 * Author:     Przemysław Rutka
 */

#if TEST_NUM == 505
  #include "fuzzy.h"

  TriFuzzyNum const * crisp_zero_addr() {
    return &::crisp_zero;
  }
#endif // 505
