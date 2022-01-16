#include "../virus_genealogy.h"
#include <iterator>

class Virus {
public:
    using id_type = int;

    Virus(Virus::id_type _id)
    : id(_id) {}

    Virus::id_type get_id() const { return id; }

private:
    id_type id;
};

int main() {
    static_assert(
        std::bidirectional_iterator<VirusGenealogy<Virus>::children_iterator>);
}
