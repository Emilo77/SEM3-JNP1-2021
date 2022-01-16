#include "../fuzzy.h"

void f(const TriFuzzyNumSet& S) {
  S.arithmetic_mean();
}

int main() {
  TriFuzzyNumSet S;
  S.insert({1,2,3});
  f(S);
}
