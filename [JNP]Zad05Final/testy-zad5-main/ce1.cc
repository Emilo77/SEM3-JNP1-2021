#include "../virus_genealogy.h"

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
	VirusGenealogy<Virus> g;
}
