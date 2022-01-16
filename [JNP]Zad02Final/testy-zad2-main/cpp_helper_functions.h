#include "../maptel.h"
#include <random>
#include <cassert>
#include <string>
#include <string.h>
using std::string;
using std::to_string;

namespace {
	std::mt19937 rng(0);
};

void set_seed(string s) {
	std::seed_seq seq(s.begin(), s.end());
	rng.seed(seq);
}

int rd(int l, int r) {
	return std::uniform_int_distribution<int>(l, r)(rng);
}

void insert(unsigned long id, string i, string j) {
	jnp1::maptel_insert(id, i.c_str(), j.c_str());
}
void insert(unsigned long id, int i, int j) {
	insert(id, to_string(i), to_string(j));
}

void assert_transform_is_equal(unsigned long id, string from, string to) {
	char tel_dst[jnp1::TEL_NUM_MAX_LEN + 1];
	jnp1::maptel_transform(id, from.c_str(), tel_dst, jnp1::TEL_NUM_MAX_LEN + 1);
	assert(strcmp(tel_dst, to.c_str()) == 0);
}
void assert_transform_is_equal(unsigned long id, int from, int to) {
	assert_transform_is_equal(id, to_string(from), to_string(to));
}

string random_tel() {
	string ret(jnp1::TEL_NUM_MAX_LEN, '0');
	for(char &c : ret)
		c += rd(0, 9);
	return ret;
}
