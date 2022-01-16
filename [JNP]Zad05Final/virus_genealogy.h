#ifndef VIRUS_GENEALOGY_H
#define VIRUS_GENEALOGY_H

#include <cassert>
#include <map>
#include <memory>
#include <set>
#include <vector>

class VirusAlreadyCreated : public std::exception {
	virtual const char *what() const throw() {
		return "VirusAlreadyCreated";
	}
} virus_already_created;

class VirusNotFound : public std::exception {
	virtual const char *what() const throw() {
		return "VirusNotFound";
	}
} virus_not_found;

class TriedToRemoveStemVirus : public std::exception {
	virtual const char *what() const throw() {
		return "TriedToRemoveStemVirus";
	}
} tried_to_remove_stem_virus;

template <typename Virus>
class VirusGenealogy {
private:
	using id_type = typename Virus::id_type;
	const id_type stem_id;

	// Każdy wirus przechowuję jako wskaźnik na odpowiedni Virus
	// oraz set'y rodziców i synów
	struct VirusInfo {
		std::shared_ptr<Virus> virus;
		std::set<id_type> parents;
		using Set = std::set<std::shared_ptr<Virus>>;
		Set children;

		VirusInfo(const id_type id) {
			virus = std::make_shared<Virus>(id);
		}
	};
	std::map<id_type, VirusInfo> viruses;

	// Trzy funkcje pomocnicze, dzięki którym
	// obsługa wyjątków staje się trochę łatwiejsza.
	VirusInfo &get_virusinfo_or_throw(const id_type &id) {
		auto it = viruses.find(id);
		if (it == viruses.end())
			throw virus_not_found;
		return it->second;
	}

	VirusInfo &get_virusinfo(const id_type &id) {
		auto it = viruses.find(id);
		assert(it != viruses.end());
		return it->second;
	}

	const VirusInfo &get_const_virusinfo_or_throw(const id_type &id) const {
		auto it = viruses.find(id);
		if (it == viruses.end())
			throw virus_not_found;
		return it->second;
	}

public:
	class children_iterator {
	private:
		VirusInfo::Set::iterator it;

	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = Virus;
		using difference_type = ptrdiff_t;
		using pointer = const std::shared_ptr<value_type>;
		using reference = const value_type &;

		children_iterator() {}

		children_iterator(VirusInfo::Set::iterator _it)
		: it(_it) {}

		reference operator*() const {
			return **it;
		}

		pointer operator->() const {
			return *it;
		}

		children_iterator &operator++() {
			++it;
			return *this;
		}

		children_iterator &operator--() {
			--it;
			return *this;
		}

		children_iterator operator++(int) {
			children_iterator tmp = *this;
			++(*this);
			return tmp;
		}

		children_iterator operator--(int) {
			children_iterator tmp = *this;
			--(*this);
			return tmp;
		}

		bool operator==(const children_iterator &other) const {
			return it == other.it;
		}
	};

	// Tworzy nową genealogię.
	// Tworzy także węzeł wirusa macierzystego o identyfikatorze stem_id.
	VirusGenealogy(id_type const &_stem_id)
	: stem_id(_stem_id) {
		create(stem_id, std::vector<id_type>{});
	}

	VirusGenealogy() = delete;
	VirusGenealogy(VirusGenealogy &) = delete;
	VirusGenealogy &operator=(VirusGenealogy &) = delete;

	// Zwraca identyfikator wirusa macierzystego.
	id_type get_stem_id() const {
		return stem_id;
	}

	// Zwraca iterator pozwalający przeglądać listę identyfikatorów
	// bezpośrednich następników wirusa o podanym identyfikatorze.
	children_iterator get_children_begin(id_type const &id) const {
		return children_iterator(
		    get_const_virusinfo_or_throw(id).children.begin());
	}

	// Iterator wskazujący na element za końcem wyżej wspomnianej listy.
	children_iterator get_children_end(id_type const &id) const {
		return children_iterator(
		    get_const_virusinfo_or_throw(id).children.end());
	}

	// Zwraca listę identyfikatorów bezpośrednich poprzedników wirusa
	// o podanym identyfikatorze.
	std::vector<id_type> get_parents(id_type const &id) const {
		const auto &set = get_const_virusinfo_or_throw(id).parents;
		return {set.begin(), set.end()};
	}

	// Sprawdza, czy wirus o podanym identyfikatorze istnieje.
	bool exists(id_type const &id) const {
		return viruses.contains(id);
	}

	// Zwraca referencję do obiektu reprezentującego wirus o podanym
	// identyfikatorze.
	const Virus &operator[](id_type const &id) const {
		return *get_const_virusinfo_or_throw(id).virus;
	}

	// Tworzy węzeł reprezentujący nowy wirus o identyfikatorze id
	// powstały z wirusów o podanym identyfikatorze parent_id lub
	// podanych identyfikatorach parent_ids.
	void create(id_type const &id, id_type const &parent_id) {
		create(id, std::vector{parent_id});
	}
	void create(id_type const &id, std::vector<id_type> const &parent_ids) {
		// Sprawdzam poprawność argumentów.
		if (exists(id))
			throw virus_already_created;
		for (const auto &p : parent_ids)
			if (not exists(p))
				throw virus_not_found;
		if (parent_ids.empty() && id != stem_id) {
			// Wierzchołek nie ma rodziców, więc od razu go usuwam.
			return;
		}

		// Konstruuję nowy Virus.
		auto it_new_virus = viruses.emplace(id, VirusInfo(id)).first;
		VirusInfo &info = it_new_virus->second;

		// Zapamiętuję co się zmieniło, by umożliwić robienie rollbacków.
		using set_t = decltype(info.children);
		std::vector<std::pair<typename set_t::iterator, set_t &>> inserted;

		try {
			// Kopiowanie id_type'ów może rzucić wyjątek.
			info.parents =
			    std::set<id_type>(parent_ids.begin(), parent_ids.end());

			for (const auto &parent_id : info.parents) {
				auto &set = get_virusinfo(parent_id).children;
				// Dodawanie do kontenerów może rzucić wyjątek.
				auto it = set.emplace(info.virus).first;
				try {
					inserted.emplace_back(it, set);
				} catch (...) {
					set.erase(it);
					throw;
				}
			}
		} catch (...) {
			for (auto &it_set : inserted) {
				it_set.second.erase(it_set.first);
			}
			viruses.erase(it_new_virus);
			throw;
		}
	}

	// Dodaje nową krawędź w grafie genealogii.
	void connect(id_type const &child_id, id_type const &parent_id) {
		VirusInfo &child = get_virusinfo_or_throw(child_id);
		VirusInfo &parent = get_virusinfo_or_throw(parent_id);
		if (child.parents.contains(parent_id)) {
			// Krawędź już istnieje.
			return;
		}

		auto it = child.parents.emplace(parent_id).first;
		try {
			parent.children.emplace(child.virus);
		} catch (...) {
			child.parents.erase(it);
			throw;
		}
	}

private:
	// Pomocnicza funkcja rekurencyjna do usuwania wierzchołków.
	// Nie ma żadnej gwarancji przy wyjątkach.
	void remove_rec(id_type const &id) {
		if (id == stem_id)
			throw tried_to_remove_stem_virus;

		VirusInfo &info = get_virusinfo_or_throw(id);
		// Wśród rodziców usuwam tego wirusa z ich synów.
		for (const id_type &parent : info.parents)
			get_virusinfo(parent).children.erase(info.virus);

		for (const auto &child_shared_ptr : info.children) {
			VirusInfo &child_info =
			    get_virusinfo(child_shared_ptr->get_id());
			// Wśród synów usuwam tego wirusa z ich rodziców.
			child_info.parents.erase(id);
			if (child_info.parents.empty()) {
				// Jeżeli wirus przestał mieć rodziców, rekurencyjnie go usuwam.
				remove_rec(child_shared_ptr->get_id());
			}
		}

		viruses.erase(id);
	}

public:
	// Usuwa wirus o podanym identyfikatorze.
	// Funkcja jest silnie odporna na wyjątki.
	void remove(id_type const &id) {
		auto viruses_copy = viruses;
		try {
			remove_rec(id);
		} catch (...) {
			swap(viruses, viruses_copy);
			throw;
		}
	}
};

#endif
