#include "../virus_genealogy.h"

#include <cassert>

class Virus {
public:
	using id_type = int;

	Virus(Virus::id_type _id)
		: id(_id)
	{}

	Virus::id_type get_id() const {
		return id;
	}

private:
	id_type id;
};

int main() {
	VirusGenealogy<Virus> g(0);
	constexpr int n = int(1e5);
	g.create(1, std::vector<typename Virus::id_type>{});
	assert(not g.exists(1));
}
