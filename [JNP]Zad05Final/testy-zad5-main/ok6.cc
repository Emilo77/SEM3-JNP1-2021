#include "../virus_genealogy.h"
#include <random>
#include <cassert>

namespace helper {

std::mt19937 rng(0);
bool try_throwing = true;
const int how_often = 10;
const int funny_number = 104;

void turn_on() {
	try_throwing = true;
}

void turn_off() {
	try_throwing = false;
}

void just_do_it() {
	if (try_throwing && rng() % how_often == 0) {
		throw funny_number;
	}
}

} // helper

struct FunnyInt {
	int x;
	FunnyInt() {
		helper::just_do_it();
	}
	FunnyInt(int _x) : x(_x) {
		helper::just_do_it();
	}
	FunnyInt(const FunnyInt& other) {
		helper::just_do_it();
		this->x = other.x;
	}
	FunnyInt& operator=(const FunnyInt& other) {
		helper::just_do_it();
		this->x = other.x;
		return *this;
	}
	auto operator<=>(const FunnyInt& other) const {
		helper::just_do_it();
		return this->x <=> other.x;
	}
	bool operator==(const FunnyInt& other) const {
		helper::just_do_it();
		return this->x == other.x;
	}
};

class SafeVirus {
public:
	using id_type = int;

	SafeVirus(SafeVirus::id_type _id): id(_id) {}

	SafeVirus::id_type get_id() const {
		return id;
	}

private:
	id_type id;
};

class FunnyVirus {
public:
	using id_type = FunnyInt;

	FunnyVirus(FunnyVirus::id_type _id) : id(_id) {
		helper::just_do_it();
	}

	FunnyVirus::id_type get_id() const {
		helper::just_do_it();
		return id.x;
	}

private:
	id_type id;
};

int main() {
	helper::turn_off();
	VirusGenealogy<FunnyVirus> g_funny(0);
	helper::turn_on();
	VirusGenealogy<SafeVirus> g_safe(0);
	const int n = int(1e5);
	const int limit = 100;
	for(int i = 1; i < n; ++i) {
		const int type = helper::rng() % 9;
		if (type == 0) {
			try {
				g_funny.get_stem_id();
			}
			catch (int e) {
				continue;
			}
			g_safe.get_stem_id();
		}
		else if (type == 1) {
			int x = helper::rng() % limit;
			helper::turn_off();
			FunnyInt fx(x);
			helper::turn_on();
			bool thrown = false;
			try {
				g_funny.get_children_begin(fx);
			}
			catch (int e) {
				continue;
			}
			catch (const VirusNotFound& e) {
				thrown = true;
			}
			try {
				g_safe.get_children_begin(x);
			}
			catch (const VirusNotFound& e) {
				assert(thrown);
				continue;
			}
			assert(!thrown);
		}
		else if (type == 2) {
			int x = helper::rng() % limit;
			helper::turn_off();
			FunnyInt fx(x);
			helper::turn_on();
			bool thrown = false;
			try {
				g_funny.get_children_end(fx);
			}
			catch (int e) {
				continue;
			}
			catch (const VirusNotFound& e) {
				thrown = true;
			}
			try {
				g_safe.get_children_end(x);
			}
			catch (const VirusNotFound& e) {
				assert(thrown);
				continue;
			}
			assert(!thrown);
		}
		else if (type == 3) {
			int x = helper::rng() % limit;
			helper::turn_off();
			FunnyInt fx(x);
			helper::turn_on();
			bool thrown = false;
			try {
				g_funny.get_parents(fx);
			}
			catch (int e) {
				continue;
			}
			catch (const VirusNotFound& e) {
				thrown = true;
			}
			try {
				g_safe.get_parents(x);
			}
			catch (const VirusNotFound& e) {
				assert(thrown);
				continue;
			}
			assert(!thrown);
		}
		else if (type == 4) {
			int x = helper::rng() % limit;
			helper::turn_off();
			FunnyInt fx(x);
			helper::turn_on();
			bool ans, fans;
			try {
				fans = g_funny.exists(fx);
			}
			catch (int e) {
				continue;
			}
			ans = g_safe.exists(x);
			assert(ans == fans);
		}
		else if (type == 5) {
			int x = helper::rng() % limit;
			helper::turn_off();
			FunnyInt fx(x);
			helper::turn_on();
			bool thrown = false;
			try {
				g_funny[fx];
			}
			catch (int e) {
				continue;
			}
			catch (const VirusNotFound& e) {
				thrown = true;
			}
			try {
				g_safe[x];
			}
			catch (const VirusNotFound& e) {
				assert(thrown);
				continue;
			}
			assert(!thrown);
		}
		else if (type == 6) {
			int x = helper::rng() % (limit - 1) + 1;
			int s = helper::rng() % 5 + 1;
			std::vector<int> v(s);
			for (int i = 0; i < s; ++i) {
				v[i] = helper::rng() % x;
			}
			helper::turn_off();
			FunnyInt fx(x);
			std::vector<FunnyInt> fv(s, FunnyInt(0));
			for (int i = 0; i < s; ++i) {
				fv[i] = FunnyInt(v[i]);
			}
			helper::turn_on();
			bool thrown1 = false;
			bool thrown2 = false;
			try {
				g_funny.create(fx, fv);
			}
			catch (int e) {
				continue;
			}
			catch (const VirusNotFound& e) {
				thrown1 = true;
			}
			catch (const VirusAlreadyCreated& e) {
				thrown2 = true;
			}
			try {
				g_safe.create(x, v);
			}
			catch (const VirusNotFound& e) {
				assert(thrown1 && !thrown2);
				continue;
			}
			catch (const VirusAlreadyCreated& e) {
				assert(!thrown1 && thrown2);
				continue;
			}
			assert(!thrown1 && !thrown2);
		}
		else if (type == 7) {
			int x = 0, y = 0;
			while (x == y) {
				x = helper::rng() % limit;
				y = helper::rng() % limit;
			}
			if (x > y) {
				std::swap(x, y);
			}
			helper::turn_off();
			FunnyInt fx(x);
			FunnyInt fy(y);
			helper::turn_on();
			bool thrown = false;
			try {
				g_funny.connect(fy, fx);
			}
			catch (int e) {
				continue;
			}
			catch (const VirusNotFound& e) {
				thrown = true;
			}
			try {
				g_safe.connect(y, x);
			}
			catch (const VirusNotFound& e) {
				assert(thrown);
				continue;
			}
			assert(!thrown);
		}
		else if (type == 8) {
			int x = helper::rng() % (limit - 1) + 1;
			helper::turn_off();
			FunnyInt fx(x);
			helper::turn_on();
			bool thrown1 = false;
			bool thrown2 = false;
			try {
				g_funny.remove(fx);
			}
			catch (int e) {
				continue;
			}
			catch (const VirusNotFound& e) {
				thrown1 = true;
			}
			catch (const TriedToRemoveStemVirus& e) {
				thrown2 = true;
			}
			try {
				g_safe.remove(x);
			}
			catch (const VirusNotFound& e) {
				assert(thrown1 && !thrown2);
				continue;
			}
			catch (const TriedToRemoveStemVirus& e) {
				assert(!thrown1 && thrown2);
				continue;
			}
			assert(!thrown1 && !thrown2);
		}
	}
}
