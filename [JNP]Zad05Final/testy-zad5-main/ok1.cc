#include "../virus_genealogy.h"
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

template<class Ex>
bool check_throws_up(auto f) {
	try {
		f();
	}
	catch(const Ex &e) {
		return true;
	}
	catch(...) {
		assert(false);
	}
	assert(false);
	return false;
}

template<class T> bool has_same_content(std::vector<T> a, std::vector<T> b) {
	sort(a.begin(), a.end());
	sort(b.begin(), b.end());
	return a == b;
}

using Iter = VirusGenealogy<Virus>::children_iterator;

bool has_same_addresses(Iter begin, Iter end, std::vector<const Virus*> addresses) {
	std::vector<const Virus*> v;
	for(; begin != end; ++begin) 
		v.emplace_back(&(*begin));
	return has_same_content(v, addresses);
}

int main() {
	Virus::id_type root = 5;
	VirusGenealogy<Virus> g(root);
	assert(g.exists(root));
	assert(not g.exists(0));
	assert(g.get_stem_id() == root);
	g[root];
	assert(check_throws_up<VirusNotFound>([&] { g[0]; }));

	Virus::id_type n1 = 7, n2 = 8, n3 = 9;
	g.create(n1, root);
	g.create(n2, root);
	g.create(n3, root);
	assert(has_same_content(g.get_parents(n1), std::vector{root}));
	assert(has_same_content(g.get_parents(n2), std::vector{root}));
	assert(has_same_content(g.get_parents(n3), std::vector{root}));

	Virus::id_type n4 = 3;
	g.create(n4, {n1, n2, n3});
	assert(has_same_content(g.get_parents(n4), std::vector{n1, n2, n3}));
	g.connect(n4, root);
	assert(has_same_content(g.get_parents(n4), std::vector{n1, n2, n3, root}));
	g.connect(n4, root);
	assert(has_same_content(g.get_parents(n4), std::vector{n1, n2, n3, root})); // ?

	assert(check_throws_up<VirusAlreadyCreated>([&] { g.create(n4, {n1, n2, n3}); }));
	assert(check_throws_up<TriedToRemoveStemVirus>([&] { g.remove(root); }));
	assert(check_throws_up<VirusNotFound>([&] { g.remove(0); }));
	assert(check_throws_up<VirusNotFound>([&] { g.connect(0, root); }));
	assert(check_throws_up<VirusNotFound>([&] { g.connect(root, 0); }));
	assert(check_throws_up<VirusNotFound>([&] { g.create(0, {-1}); }));
	assert(check_throws_up<VirusNotFound>([&] { g[0]; }));
	assert(check_throws_up<VirusNotFound>([&] { g.get_parents(0); }));
	assert(check_throws_up<VirusNotFound>([&] { g.get_children_begin(0); }));
	assert(check_throws_up<VirusNotFound>([&] { g.get_children_end(0); }));
	assert(check_throws_up<VirusNotFound>([&] { g.get_parents(0); }));

	assert(has_same_addresses(g.get_children_begin(root), g.get_children_end(root), std::vector<const Virus*>{&g[n1], &g[n2], &g[n3], &g[n4]}));
	assert(has_same_addresses(g.get_children_begin(n1), g.get_children_end(n1), std::vector<const Virus*>{&g[n4]}));
	assert(has_same_addresses(g.get_children_begin(n2), g.get_children_end(n2), std::vector<const Virus*>{&g[n4]}));
	assert(has_same_addresses(g.get_children_begin(n3), g.get_children_end(n3), std::vector<const Virus*>{&g[n4]}));
	assert(has_same_addresses(g.get_children_begin(n4), g.get_children_end(n4), std::vector<const Virus*>{}));

	g.remove(n1);
	assert(check_throws_up<VirusNotFound>([&] { g[n1]; }));
	assert(has_same_content(g.get_parents(root), std::vector<Virus::id_type>{}));
	assert(has_same_content(g.get_parents(n2), std::vector{root}));
	assert(has_same_content(g.get_parents(n3), std::vector{root}));
	assert(has_same_content(g.get_parents(n4), std::vector{root, n2, n3}));
	assert(has_same_addresses(g.get_children_begin(root), g.get_children_end(root), std::vector<const Virus*>{&g[n2], &g[n3], &g[n4]}));
	assert(has_same_addresses(g.get_children_begin(n2), g.get_children_end(n2), std::vector<const Virus*>{&g[n4]}));
	assert(has_same_addresses(g.get_children_begin(n3), g.get_children_end(n3), std::vector<const Virus*>{&g[n4]}));
	assert(has_same_addresses(g.get_children_begin(n4), g.get_children_end(n4), std::vector<const Virus*>{}));

	g.connect(n4, root);

	static_assert(std::is_same<decltype(*g.get_children_begin(0)), const Virus&>::value);
	static_assert(std::is_base_of<std::exception, VirusNotFound>::value);
	static_assert(std::is_base_of<std::exception, VirusAlreadyCreated>::value);
	static_assert(std::is_base_of<std::exception, TriedToRemoveStemVirus>::value);
}

