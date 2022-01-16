#include "../fuzzy.h"
#include <cassert>
#include <vector>

int main() {
  std::vector<TriFuzzyNum> T{
    TriFuzzyNum(1.,2.,3.),
    TriFuzzyNum(1.,3.,2.),
    TriFuzzyNum(2.,1.,3.),
    TriFuzzyNum(2.,3.,1.),
    TriFuzzyNum(3.,1.,2.),
    TriFuzzyNum(3.,2.,1.)
  };
  for (const auto& x : T) {
    assert(x == TriFuzzyNum(1.,2.,3.));
    assert(x.lower_value() == 1.);
    assert(x.modal_value() == 2.);
    assert(x.upper_value() == 3.);
  }
}
