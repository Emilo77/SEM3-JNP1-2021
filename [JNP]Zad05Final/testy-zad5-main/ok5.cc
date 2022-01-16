// Testuje wyjątek w konstruktorze Virusa

#include "../virus_genealogy.h"
#include <cassert>
#include <exception>
#include <iostream>

class Error : public std::exception {
public:
	const char *what() const throw() {
		return "Rzucił wyjątkiem\n";
	}
};

class ThrowingVirus {
public:
	using id_type = int;
	ThrowingVirus(id_type const &_id)
	: id(_id) {
		if (id == 15) {
			throw Error();
		}
	}
	id_type get_id() const {
		return id;
	}

private:
	id_type id;
};

template <typename Virus>
int size(VirusGenealogy<Virus> &gen, typename Virus::id_type parent_id) {
	int size = 0;
	for (auto iter = gen.get_children_begin(parent_id);
	     iter != gen.get_children_end(parent_id); iter++)
	{
		size++;
	}
	return size;
}

int main() {
	try {
		// Czy jak po prostu rzuca w konstruktorze genealogii to jest ok?
		VirusGenealogy<ThrowingVirus> thr(15);
		assert(false);
	} catch (const Error &) {
	} catch (const std::exception &) {
		assert(false);
	} catch (...) {
	}
	VirusGenealogy<ThrowingVirus> gen(10);
	ThrowingVirus::id_type st_id = gen.get_stem_id();
	assert(gen.exists(st_id));

	// Rzucanie w create(id, rodzic)
	gen.create(11, st_id);
	try {
		gen.create(15, st_id);
		assert(false);
	} catch (const Error &) {
	} catch (const std::exception &) {
		assert(false);
	}
	assert(gen.exists(11));
	assert(!gen.exists(15));

	assert(size(gen, st_id) == 1);

	try {
		gen.connect(15, 11);
		assert(false);
	} catch (const std::exception &e) {
	}

	// Rzucanie w create(id, rodzice)
	try {
		std::vector<ThrowingVirus::id_type> parent_ids = {st_id, 11};
		gen.create(15, parent_ids);
		assert(false);
	} catch (const Error &) {
	} catch (const std::exception &) {
		assert(false);
	}

	assert(!gen.exists(15));
	assert(size(gen, st_id) == 1);
	assert(size(gen, 11) == 0);

	return 0;
}
