/* Testy do zadania Genealogia Wirusów */
#include "virus_genealogy.h"

template <class Virus>
std::vector<typename Virus::id_type> gen_get_children(const VirusGenealogy<Virus>& gen,
                                                      typename Virus::id_type arg) {
  std::vector<typename Virus::id_type> res;
  std::transform(gen.get_children_begin(arg),
                 gen.get_children_end(arg),
                 std::back_inserter(res),
                 [](const Virus &v) -> typename Virus::id_type {return v.get_id();});
  return res;
}

#if TEST_NUM <= 199 || TEST_NUM >= 900

#include <cassert>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

class Virus {
  public:
    using id_type = std::string;
    Virus(id_type const &_id) : id(_id) {
    }
    id_type get_id() const {
      return id;
    }
  private:
    id_type id;
};

#elif TEST_NUM <= 699

#include <cassert>
#include <exception>

// Wyjątek zgłaszany przez klasy Virus i Virus::id_type
class ExceptionSafetyTester : public std::exception {
  public:
     ExceptionSafetyTester() : std::exception() {
     }
};

// Pomocnicza klasa służąca do systematycznego zgłaszania wyjątków przy
// każdej nadarzającej się okazji
class Thrower {
  public:
    Thrower() {
    }
    void reset(long val) noexcept {
      throw_value = val;
      throw_counter = 0;
      thrown = false;
    }
    void enable(bool en) noexcept {
      enabled = en;
    }
    void enableBadAlloc(bool en) noexcept {
      allocEnabled = en;
    }
    void execute() {
      if (enabled) {
        if (throw_value == throw_counter++) {
          thrown = true;
          throw ExceptionSafetyTester();
        }
      }
    }
    void badAlloc() {
      if (allocEnabled) {
        if (throw_value == throw_counter++) {
          thrown = true;
          throw std::bad_alloc();
        }
      }
    }
  private:
    bool enabled = false;
    bool allocEnabled = false;
    long throw_value = 0;
    long throw_counter = 0;
  public:
    bool thrown = false;
};

static Thrower thrower;

// Nasza bardzo wyjątkowa klasa
class Virus {
  public:
    class id_type {
      public:
        // Virus::id_type ma konstruktor bezargumentowy, konstruktor
        // kopiujący, operator przypisania.
        id_type() : id(0) {
          thrower.execute();
        }
        id_type(id_type const &that) : id(that.id) {
          thrower.execute();
        }
        id_type& operator=(id_type const& that) {
          thrower.execute();
          id = that.id;
          return *this;
        }

        // Konstruktor do tworzenia identyfikatorów
        id_type(int i) : id(i) {
          thrower.execute();
        }

        // Destruktor musi być no-throw.
        ~id_type() noexcept {
        }

        // Wartości typu Virus::id_type tworzą porządek liniowy i można je
        // porównywać za pomocą operatorów ==, !=, <=, >=, <, >, <=>.
        bool operator==(id_type const &that) const {
          thrower.execute();
          return id == that.id;
        }
        bool operator!=(id_type const &that) const {
          thrower.execute();
          return id != that.id;
        }
        bool operator<(id_type const &that) const {
          thrower.execute();
          return id < that.id;
        }
        bool operator<=(id_type const &that) const {
          thrower.execute();
          return id <= that.id;
        }
        bool operator>(id_type const &that) const {
          thrower.execute();
          return id > that.id;
        }
        bool operator>=(id_type const &that) const {
          thrower.execute();
          return id >= that.id;
        }
        int operator<=>(id_type const &that) const {
          thrower.execute();
          if (id < that.id)
            return -1;
          else if (id > that.id)
            return 1;
          else
            return 0;
        }

        // Gwarantujemy, że swap jest no-throw.
        void swap(id_type &that) noexcept {
          int tmp = id;
          id = that.id;
          that.id = tmp;
        }

      private:
        int id;
    };

  public:
    // Klasa Virus ma konstruktor przyjmujący argument typu Virus::id_type.
    Virus(id_type const &id) : vid(id) {
      thrower.execute();
    }

    // Destruktor musi być no-throw.
    ~Virus() noexcept {
    }

    // Klasa Virus ma metodę Virus::id_type get_id() const.
    id_type get_id() const {
      thrower.execute();
      return vid;
    }

    // Do testowania, czy obiekt faktycznie istnieje.
    void set(int v) noexcept {
      value = v;
    }
    int get() const noexcept {
      return value;
    }

  private:
    id_type vid;
    int value;

    Virus() = delete;
    Virus(Virus const &) = delete;
    Virus(Virus &&) = delete;
    void operator=(Virus) = delete;
};

// Gwarantujemy, że swap jest no-throw.
void swap(Virus::id_type &a, Virus::id_type &b) noexcept {
  a.swap(b);
}

#if TEST_NUM >= 604 && TEST_NUM <= 613
// Alokator też może rzucać wyjątki.
void* operator new(std::size_t size) {
  thrower.badAlloc();
  void *p = malloc(size);
  if (p)
    return p;
  throw "operator new() error";
}

void* operator new[](std::size_t size) {
  thrower.badAlloc();
  void *p = malloc(size);
  if (p)
    return p;
  throw "operator new[]() error";
}

void* operator new(std::size_t size, std::align_val_t al) {
  thrower.badAlloc();
  void *p = aligned_alloc(static_cast<std::size_t>(al), size);
  if (p)
    return p;
  throw "operator new() error";
}

void* operator new[](std::size_t size, std::align_val_t al) {
  thrower.badAlloc();
  void *p = aligned_alloc(static_cast<std::size_t>(al), size);
  if (p)
    return p;
  throw "operator new[]() error";
}

void operator delete(void* p) noexcept {
  free(p);
}

void operator delete(void* p, [[maybe_unused]] std::size_t s) noexcept {
  free(p);
}

void operator delete[](void *p) noexcept {
  free(p);
}

void operator delete[](void *p, [[maybe_unused]] std::size_t s) noexcept {
  free(p);
}

void operator delete(void* p, [[maybe_unused]] std::align_val_t al) noexcept {
  free(p);
}

void operator delete[](void* p, [[maybe_unused]] std::align_val_t al) noexcept {
  free(p);
}

void operator delete(void* p, [[maybe_unused]] std::size_t sz,
                     [[maybe_unused]] std::align_val_t al) noexcept {
  free(p);
}

void operator delete[](void* p, [[maybe_unused]] std::size_t sz,
                       [[maybe_unused]] std::align_val_t al) noexcept {
  free(p);
}
#endif

int stem_id = 10777;

// BuildGraph, ModifyGraph, CheckBuilGraph, CheckModifiedGraph - funkcje zawierające
//   kod do przetestowania;
// enable_badalloc_exceptions - ustawia, że chcemy testować błędy w czasie alokacji;
//   błędy w czasie alokacji "konkurują" z innymi i mogą się dowolnie przeplatać
//   w fazach sprawdzania poprawności, pozostałe są opisane w treści funkcji.
void RunTest(void (* BuildGraph)(VirusGenealogy<Virus> &),
             void (* ModifyGraph)(VirusGenealogy<Virus> &),
             void (* CheckBuiltGraph)(VirusGenealogy<Virus> &),
             void (* CheckModifiedGraph)(VirusGenealogy<Virus> &),
             bool enable_badalloc_exceptions,
             bool enable_constructor_exceptions,
             bool enable_method_exceptions,
             bool enable_destruction_phase_exceptions,
             bool virus_already_created_exception_expected,
             bool virus_not_found_exception_expected,
             bool tried_to_remove_stem_virus_exception_expected) {
  const bool bad_alloc_exception_expected [[maybe_unused]]= enable_badalloc_exceptions;
  for (long throw_value = 0; ; ++throw_value) {
    try {
      thrower.reset(throw_value);

      // Enable, aby testować odporność na wyjątki w konstruktorze.
      thrower.enable(enable_constructor_exceptions);
      thrower.enableBadAlloc(enable_badalloc_exceptions);

      // Jak są wyjątki przy alokacji, to będą błędy przy alokacji.
      // Tworzymy tu osobną zmienną, żeby było systematycznie.

      VirusGenealogy<Virus> r(stem_id);

      // Nie chcemy tu wyjątków.
      thrower.enable(false);
      BuildGraph(r);
      CheckBuiltGraph(r);

      // Stosujemy tę konstrukcję, bo w C++ nie można łapać różnych wyjątków bez
      // wspólnej nadklasy, a potrzebujemy oba obsłużyć tak samo.
      {
        bool exceptionSafetyTeserOrBadAllocThrown = false;
        try {
          // Enable, aby testować odporność na wyjątki w poszczególnych metodach.
          thrower.enable(enable_method_exceptions);
          ModifyGraph(r);

          // Nie chcemy wyjątków przy sprawdzaniu poprawności.
          thrower.enable(false);
          CheckModifiedGraph(r);
        }
        catch (ExceptionSafetyTester const&) {
          exceptionSafetyTeserOrBadAllocThrown = true;

        }
        catch (std::bad_alloc const&) {
          exceptionSafetyTeserOrBadAllocThrown = true;
        }
        catch (...) {
          // Zakładamy, że tylko ModifyGraph zgłasza wyjątki inne niż
          // ExceptionSafetyTester lub bad_alloc.
          thrower.enable(false);
          CheckBuiltGraph(r);

          // Pozostałe przepuszczaj po sprawdzeniu.
          throw;
        }
        if (exceptionSafetyTeserOrBadAllocThrown) {
          // Nie chcemy wyjątków przy sprawdzaniu poprawności.
          thrower.enable(false);
          assert(thrower.thrown);
          thrower.thrown = false;
          CheckBuiltGraph(r);

          // Testuj, aż przestaną być zgłaszane wyjątki.
          continue;
        }
      }

      // Enable, aby testować no-throw destruktora.
      thrower.enable(enable_destruction_phase_exceptions);
    }
    catch (std::bad_alloc const&) {
      assert(bad_alloc_exception_expected);
      // Destruktor nie może zgłaszać wyjątków.
      assert(!enable_destruction_phase_exceptions);
      return;
    }
    catch (ExceptionSafetyTester const&) {
      // Tu wchodzimy, gdy wyjątek został zgłoszony w konstruktorze lub destruktorze.
      assert(thrower.thrown);
      thrower.thrown = false;
      // Destruktor nie może zgłaszać wyjątków.
      assert(!enable_destruction_phase_exceptions);
      continue;
    }
    catch (VirusAlreadyCreated const&) {
      assert(virus_already_created_exception_expected);
      // Destruktor nie może zgłaszać wyjątków.
      assert(!enable_destruction_phase_exceptions);
      return;
    }
    catch (VirusNotFound const&) {
      assert(virus_not_found_exception_expected);
      // Destruktor nie może zgłaszać wyjątków.
      assert(!enable_destruction_phase_exceptions);
      return;
    }
    catch (TriedToRemoveStemVirus const&) {
      assert(tried_to_remove_stem_virus_exception_expected);
      // Destruktor nie może zgłaszać wyjątków.
      assert(!enable_destruction_phase_exceptions);
      return;
    }
    catch (...) {
      // Każdy inny wyjątek jest tu błędem.
      assert(false);
      return;
    }

    // Nie było wyjątku, a oczekiwaliśmy go.
    assert(!thrower.thrown);
    assert(!virus_already_created_exception_expected);
    assert(!virus_not_found_exception_expected);
    assert(!tried_to_remove_stem_virus_exception_expected);

    // Nie ma więcej wyjątków, więc test zakończył się pomyślnie.
    return;
  }
}

void NOP(VirusGenealogy<Virus> &) {
}

void CheckGraph0(VirusGenealogy<Virus>& graph) {
  assert(!graph.exists(1));
  assert(!graph.exists(2));

  assert(graph.get_stem_id() == stem_id);
  assert(gen_get_children(graph, stem_id).size() == 0);
  assert(graph.get_parents(stem_id).size() == 0);
}

void CheckGraph1(VirusGenealogy<Virus>& graph) {
  assert(graph.exists(1));
  assert(!graph.exists(2));

  assert(graph.get_stem_id() == stem_id);
  assert(graph.get_parents(stem_id).size() == 0);
  auto children0 = gen_get_children(graph, stem_id);
  assert(children0.size() == 1);
  assert(children0[0] == 1);

  auto parents1 = graph.get_parents(1);
  assert(parents1.size() == 1);
  assert(parents1[0] == stem_id);
  assert(gen_get_children(graph, 1).size() == 0);
}

void CheckGraph12(VirusGenealogy<Virus>& graph) {
  assert(graph.exists(1));
  assert(graph.exists(2));

  assert(graph.get_stem_id() == stem_id);
  assert(graph.get_parents(stem_id).size() == 0);
  auto children0 = gen_get_children(graph, stem_id);
  assert(children0.size() == 1);
  assert(children0[0] == 1);

  auto parents1 = graph.get_parents(1);
  assert(parents1.size() == 1);
  assert(parents1[0] == stem_id);
  auto children1 = gen_get_children(graph, 1);
  assert(children1.size() == 1);
  assert(children1[0] == 2);

  auto parents2 = graph.get_parents(2);
  assert(parents2.size() == 1);
  assert(parents2[0] == 1);
  assert(gen_get_children(graph, 2).size() == 0);
}

void CheckGraph2(VirusGenealogy<Virus>& graph) {
  assert(!graph.exists(1));
  assert(graph.exists(2));

  assert(graph.get_stem_id() == stem_id);
  assert(graph.get_parents(stem_id).size() == 0);
  auto children0 = gen_get_children(graph, stem_id);
  assert(children0.size() == 1);
  assert(children0[0] == 2);

  auto parents2 = graph.get_parents(2);
  assert(parents2.size() == 1);
  assert(parents2[0] == stem_id);
  assert(gen_get_children(graph, 2).size() == 0);
}

void CheckGraph123(VirusGenealogy<Virus>& graph) {
  assert(graph.exists(1));
  assert(graph.exists(2));

  assert(graph.get_stem_id() == stem_id);
  assert(graph.get_parents(stem_id).size() == 0);
  auto children0 = gen_get_children(graph, stem_id);
  assert(children0.size() == 2);
  assert((children0[0] == 1 && children0[1] == 2) ||
         (children0[0] == 2 && children0[1] == 1));

  auto parents1 = graph.get_parents(1);
  assert(parents1.size() == 1);
  assert(parents1[0] == stem_id);
  auto children1 = gen_get_children(graph, 1);
  assert(children1.size() == 1);
  assert(children1[0] == 2);

  auto parents2 = graph.get_parents(2);
  assert(parents2.size() == 2);
  assert((parents2[0] == 1 && parents2[1] == stem_id) ||
         (parents2[0] == stem_id && parents2[1] == 1));
  assert(gen_get_children(graph, 2).size() == 0);
}

void Create1(VirusGenealogy<Virus>& graph) {
  graph.create(1, stem_id);
}

void Create2single(VirusGenealogy<Virus>& graph) {
  graph.create(2, 1);
}

void Create2vector(VirusGenealogy<Virus>& graph) {
  std::vector<Virus::id_type> parents;
  parents.push_back(1);
  parents.push_back(stem_id);
  graph.create(2, parents);
}

void CreateEmpty(VirusGenealogy<Virus>& graph) {
  std::vector<Virus::id_type> parents;
  graph.create(2, parents);
}

void Graph2(VirusGenealogy<Virus>& graph) {
  Create1(graph);
  Create2single(graph);
}

void Graph3(VirusGenealogy<Virus>& graph) {
  Create1(graph);
  Create2vector(graph);
}

void Connect20(VirusGenealogy<Virus>& graph) {
  graph.connect(2, stem_id);
}

void Connect25(VirusGenealogy<Virus>& graph) {
  graph.connect(2, 5);
}

void Remove0(VirusGenealogy<Virus>& graph) {
  graph.remove(stem_id);
}

void Remove1(VirusGenealogy<Virus>& graph) {
  graph.remove(1);
}

void Remove2(VirusGenealogy<Virus>& graph) {
  graph.remove(2);
}

void Connect20Remove2(VirusGenealogy<Virus>& graph) {
  Connect20(graph);
  Remove2(graph);
}

void GetChildren(VirusGenealogy<Virus>& graph) {
  assert(gen_get_children(graph, 100).size() == 0);
}

void GetParents(VirusGenealogy<Virus>& graph) {
  assert(graph.get_parents(100).size() == 0);
}

void Operator(VirusGenealogy<Virus>& graph) {
  assert(graph[100].get_id() == 0);
}

void SimpleTest(VirusGenealogy<Virus>& graph) {
  graph.create(7, stem_id);
  assert(graph.exists(stem_id));
  assert(!graph.exists(6));
  assert(graph.exists(7));
  assert(!graph.exists(8));
  const Virus &v0_a = graph[stem_id];
  const Virus &v7_a = graph[7];
  const Virus &v0_b = graph[stem_id];
  const Virus &v7_b = graph[7];
  assert(&v0_a == &v0_b);
  assert(&v7_a == &v7_b);
}

void AdvancedTest(VirusGenealogy<Virus>& graph) {
  int i, n;

  for (n = 2; n <= 10; ++n) {
    graph.create(1, stem_id);
    graph.create(2, stem_id);
    for (i = 1; i <= n - 2; ++i) {
      graph.create(i + 2, i);
    }
    for (i = 1; i <= n - 3; ++i) {
      graph.connect(i + 3, i);
    }
    graph.remove(1);
    graph.remove(2);
    assert(graph.exists(stem_id));
    for (i = 0; i <= n + 1; ++i) {
      assert(!graph.exists(i));
    }
  }
}


#elif TEST_NUM <= 799

// Test, czy jest header guard.
#include "virus_genealogy.h"

#elif TEST_NUM <= 899

#include <cassert>
#include <exception>

#endif

int main() {

  // Test z treści zadania

  #if TEST_NUM == 100
  VirusGenealogy<Virus> gen("A1H1");
  Virus::id_type const id1 = gen.get_stem_id();
  assert(gen.exists(id1));
  assert(gen.get_parents(id1).size() == 0);
  gen.create("A", id1);
  gen.create("B", id1);
  assert(gen_get_children(gen, id1).size() == 2);
  gen.create("C", "A");
  gen.connect("C", "B");
  assert(gen.get_parents("C").size() == 2);
  assert(gen_get_children(gen, "A").size() == 1);
  std::vector<Virus::id_type> parents;
  parents.push_back("A");
  parents.push_back("B");
  gen.create("D", parents);
  assert(gen.get_parents("D").size() == parents.size());
  assert(gen_get_children(gen, "A").size() == 2);
  assert("D" == gen["D"].get_id());
  gen.remove("A");
  assert(!gen.exists("A"));
  assert(gen.exists("B"));
  assert(gen.exists("C"));
  assert(gen.exists("D"));
  gen.remove("B");
  assert(!gen.exists("A"));
  assert(!gen.exists("B"));
  assert(!gen.exists("C"));
  assert(!gen.exists("D"));
  try {
    gen["E"];
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    std::stringstream sstr;
    sstr << e.what();
    assert(sstr.str() == "VirusNotFound");
  }
  try {
    gen.create("E", "A1H1");
    gen.create("E", "A1H1");
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    std::stringstream sstr;
    sstr << e.what();
    assert(sstr.str() == "VirusAlreadyCreated");
  }
  try {
    gen.remove("A1H1");
  }
  catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    std::stringstream sstr;
    sstr << e.what();
    assert(sstr.str() == "TriedToRemoveStemVirus");
  }
  #endif

  // Proste testy poprawności poszczególnych operacji

  #if TEST_NUM == 200
  RunTest(NOP, NOP, CheckGraph0, CheckGraph0,
          false, false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 201
  RunTest(NOP, Create1, CheckGraph0, CheckGraph1,
          false, false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 202
  RunTest(Create1, Create2single, CheckGraph1, CheckGraph12,
          false, false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 203
  RunTest(Create1, Create2vector, CheckGraph1, CheckGraph123,
          false, false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 204
  RunTest(Graph2, Connect20, CheckGraph12, CheckGraph123,
          false, false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 205
  RunTest(Graph2, Remove1, CheckGraph12, CheckGraph0,
          false, false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 206
  RunTest(Graph2, Remove2, CheckGraph12, CheckGraph1,
          false, false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 207
  RunTest(Graph3, Remove1, CheckGraph123, CheckGraph2,
          false, false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 208
  RunTest(Graph3, Remove2, CheckGraph123, CheckGraph1,
          false, false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 209
  RunTest(Graph3, Connect20, CheckGraph123, CheckGraph123,
          false, false, false, false, false, false, false);
  #endif

  #if TEST_NUM == 210
  RunTest(Graph3, Connect20Remove2, CheckGraph123, CheckGraph1,
          false, false, false, false, false, false, false);
  #endif

  // Testy poprawności zgłaszania wyjątków

  #if TEST_NUM == 300
  RunTest(Create1, Create1, CheckGraph1, CheckGraph1,
          false, false, false, false, true, false, false); // VirusAlreadyCreated
  #endif

  #if TEST_NUM == 301
  RunTest(Graph2, Create2vector, CheckGraph12, CheckGraph12,
          false, false, false, false, true, false, false); // VirusAlreadyCreated
  #endif

  #if TEST_NUM == 302
  RunTest(NOP, Create2single, CheckGraph0, CheckGraph0,
          false, false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 303
  RunTest(NOP, Create2vector, CheckGraph0, CheckGraph0,
          false, false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 304
  RunTest(NOP, Connect20, CheckGraph0, CheckGraph0,
          false, false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 305
  RunTest(NOP, Remove0, CheckGraph0, CheckGraph0,
          false, false, false, false, false, false, true); // TriedToRemoveStemVirus
  #endif

  #if TEST_NUM == 306
  RunTest(Graph3, Remove0, CheckGraph123, CheckGraph123,
          false, false, false, false, false, false, true); // TriedToRemoveStemVirus
  #endif

  #if TEST_NUM == 307
  RunTest(NOP, GetChildren, CheckGraph0, CheckGraph0,
          false, false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 308
  RunTest(NOP, GetParents, CheckGraph0, CheckGraph0,
          false, false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 309
  RunTest(NOP, Operator, CheckGraph0, CheckGraph0,
          false, false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 310
  RunTest(Graph2, Connect25, CheckGraph12, CheckGraph12,
          false, false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 311
  RunTest(Create1, Remove2, CheckGraph1, CheckGraph1,
          false, false, false, false, false, true, false); // VirusNotFound
  #endif

  #if TEST_NUM == 312
  RunTest(NOP, CreateEmpty, CheckGraph0, CheckGraph0,
          false, false, false, false, false, true, false); // VirusNotFound
  #endif

  // Testy no-throw destruktora

  #if TEST_NUM == 400
  RunTest(NOP, NOP, CheckGraph0, CheckGraph0,
          false, false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 401
  RunTest(NOP, Create1, CheckGraph0, CheckGraph1,
          false, false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 402
  RunTest(Create1, Create2single, CheckGraph1, CheckGraph12,
          false, false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 403
  RunTest(Create1, Create2vector, CheckGraph1, CheckGraph123,
          false, false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 404
  RunTest(Graph2, Connect20, CheckGraph12, CheckGraph123,
          false, false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 405
  RunTest(Graph2, Remove1, CheckGraph12, CheckGraph0,
          false, false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 406
  RunTest(Graph2, Remove2, CheckGraph12, CheckGraph1,
          false, false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 407
  RunTest(Graph3, Remove1, CheckGraph123, CheckGraph2,
          false, false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 408
  RunTest(Graph3, Remove2, CheckGraph123, CheckGraph1,
          false, false, false, true, false, false, false);
  #endif

  #if TEST_NUM == 409
  RunTest(Graph3, Connect20, CheckGraph123, CheckGraph123,
          false, false, false, true, false, false, false);
  #endif

  // Testy odporności na wyjątki

  #if TEST_NUM == 500
  RunTest(NOP, NOP, CheckGraph0, CheckGraph0,
          false, true, false, false, false, false, false);
  #endif

  #if TEST_NUM == 501
  RunTest(NOP, Create1, CheckGraph0, CheckGraph1,
          false, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 502
  RunTest(Create1, Create2single, CheckGraph1, CheckGraph12,
          false, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 503
  RunTest(Create1, Create2vector, CheckGraph1, CheckGraph123,
          false, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 504
  RunTest(Graph2, Connect20, CheckGraph12, CheckGraph123,
          false, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 505
  RunTest(Graph2, Remove1, CheckGraph12, CheckGraph0,
          false, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 506
  RunTest(Graph2, Remove2, CheckGraph12, CheckGraph1,
          false, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 507
  RunTest(Graph3, Remove1, CheckGraph123, CheckGraph2,
          false, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 508
  RunTest(Graph3, Remove2, CheckGraph123, CheckGraph1,
          false, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 509
  RunTest(Graph3, Connect20, CheckGraph123, CheckGraph123,
          false, false, true, false, false, false, false);
  #endif

  // Różne testy

  #if TEST_NUM == 600
  RunTest(NOP, SimpleTest, NOP, NOP,
          false, true, true, false, false, false, false);
  #endif

  #if TEST_NUM == 601
  RunTest(NOP, SimpleTest, NOP, NOP,
          false, false, true, true, false, false, false);
  #endif

  #if TEST_NUM == 602
  RunTest(NOP, AdvancedTest, NOP, NOP,
          false, true, true, false, false, false, false);
  #endif

  #if TEST_NUM == 603
  RunTest(NOP, AdvancedTest, NOP, NOP,
          false, false, true, true, false, false, false);
  #endif

  // Testy odporności na wyjątki w operatorze new

  #if TEST_NUM == 604
  RunTest(NOP, NOP, CheckGraph0, CheckGraph0,
          true, true, false, false, false, false, false);
  #endif

  #if TEST_NUM == 605
  RunTest(NOP, Create1, CheckGraph0, CheckGraph1,
          true, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 606
  RunTest(Create1, Create2single, CheckGraph1, CheckGraph12,
          true, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 607
  RunTest(Create1, Create2vector, CheckGraph1, CheckGraph123,
          true, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 608
  RunTest(Graph2, Connect20, CheckGraph12, CheckGraph123,
          true, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 609
  RunTest(Graph2, Remove1, CheckGraph12, CheckGraph0,
          true, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 610
  RunTest(Graph2, Remove2, CheckGraph12, CheckGraph1,
          true, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 611
  RunTest(Graph3, Remove1, CheckGraph123, CheckGraph2,
          true, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 612
  RunTest(Graph3, Remove2, CheckGraph123, CheckGraph1,
          true, false, true, false, false, false, false);
  #endif

  #if TEST_NUM == 613
  RunTest(Graph3, Connect20, CheckGraph123, CheckGraph123,
          true, false, true, false, false, false, false);
  #endif

  // Testy, czy wyjątki dziedziczą po std::exception.

  #if TEST_NUM == 800
  try {
    throw VirusAlreadyCreated();
  }
  catch (const std::exception&) {
    return 0;
  }
  catch (...) {
    assert(false);
  }
  #endif

  #if TEST_NUM == 801
  try {
    throw VirusNotFound();
  }
  catch (const std::exception &) {
    return 0;
  }
  catch (...) {
    assert(false);
  }
  #endif

  #if TEST_NUM == 802
  try {
    throw TriedToRemoveStemVirus();
  }
  catch (const std::exception&) {
    return 0;
  }
  catch (...) {
    assert(false);
  }
  #endif

  // Próba użycia konstruktora kopiującego lub operatora przypisania dla
  // obiektów klasy VirusGenealogy powinna zakończyć się błędem kompilacji.

  #if TEST_NUM == 900
  // Musi być jeden test, który zawsze powinien się skompilować,
  // aby niekompilujący się program nie dostał punktu.
  VirusGenealogy<Virus> r1("ABC");
  #endif

  #if TEST_NUM == 901
  VirusGenealogy<Virus> r1("ABC");
  VirusGenealogy<Virus> r2(r1);
  #endif

  #if TEST_NUM == 902
  VirusGenealogy<Virus> r1("ABC");
  VirusGenealogy<Virus> r2(std::move(r1));
  #endif

  #if TEST_NUM == 903
  VirusGenealogy<Virus> r1("A");
  VirusGenealogy<Virus> r2("B");
  r1 = r2;
  #endif

  #if TEST_NUM == 904
  VirusGenealogy<Virus> r1("A");
  VirusGenealogy<Virus> r2("B");
  r1 = std::move(r2);
  #endif

  // Test spełniania warunków narzuconych na iterator.

  #if TEST_NUM == 1000
  VirusGenealogy<Virus> r1("A");
  auto i = r1.get_children_begin("A");
  static_assert(std::bidirectional_iterator<decltype(i)>);
  assert(typeid(*i) == typeid(const Virus &));
  #endif
}
