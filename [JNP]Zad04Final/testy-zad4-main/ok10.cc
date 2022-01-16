#include "../treasure_hunt.h"

int main() {
    Treasure<char, false> test1{'a'};
    Treasure<char, false> test2{'b'};
    static_assert(&test1.isTrapped == &test2.isTrapped);
}
