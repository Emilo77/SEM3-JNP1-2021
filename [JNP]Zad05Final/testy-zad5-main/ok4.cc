#include "../virus_genealogy.h"
#include <cassert>

class Virus {
public:
	using id_type = int;

	Virus(Virus::id_type _id)
	: id(_id) {}

	Virus::id_type get_id() const {
		return id;
	}

private:
	id_type id;
};

template <typename V>
int size(VirusGenealogy<V> &gen, typename V::id_type parent_id) {
	int size = 0;
	for (auto iter = gen.get_children_begin(parent_id);
	     iter != gen.get_children_end(parent_id); iter++)
	{
		size++;
	}
	return size;
}

int main() {
	VirusGenealogy<Virus> g(0);
	int stem_id = g.get_stem_id();

	g.create(1, stem_id);
	g.create(2, stem_id);

	g.create(3, {1, 2});

	g.remove(1);
	assert(g.get_parents(3).size() == 1);
	assert(size(g, 0) == 1);
}
