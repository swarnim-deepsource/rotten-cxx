// scatr-check: CXX-P2000
namespace std {
template <typename T> struct less {
  bool operator()(const T &lhs, const T &rhs) { return lhs < rhs; }
};

template <typename T> struct greater {
  bool operator()(const T &lhs, const T &rhs) { return lhs > rhs; }
};

struct iterator_type {};

template <typename K, typename Cmp = less<K>> struct set {
  typedef iterator_type iterator;
  iterator find(const K &k);
  unsigned count(const K &k);

  iterator begin();
  iterator end();
  iterator begin() const;
  iterator end() const;
};

struct other_iterator_type {};
template <typename K, typename V, typename Cmp = less<K>> struct map {
  typedef other_iterator_type iterator;
  iterator find(const K &k);
  unsigned count(const K &k);

  iterator begin();
  iterator end();
  iterator begin() const;
  iterator end() const;
};

template <typename K, typename V> struct multimap : map<K, V> {};
template <typename K> struct unordered_set : set<K> {};
template <typename K, typename V> struct unordered_map : map<K, V> {};
template <typename K> struct unordered_multiset : set<K> {};
template <typename K, typename V> struct unordered_multimap : map<K, V> {};

template <typename K, typename Cmp = less<K>> struct multiset : set<K, Cmp> {};

template <typename FwIt, typename K>
FwIt find(FwIt, FwIt end, const K &) { return end; }

template <typename FwIt, typename K, typename Cmp>
FwIt find(FwIt, FwIt end, const K &, Cmp) { return end; }

template <typename FwIt, typename Pred>
FwIt find_if(FwIt, FwIt end, Pred) { return end; }

template <typename FwIt, typename K>
unsigned count(FwIt, FwIt, const K &) { return 0; }

template <typename FwIt, typename K>
FwIt lower_bound(FwIt, FwIt end, const K &) { return end; }

template <typename FwIt, typename K, typename Ord>
FwIt lower_bound(FwIt, FwIt end, const K &, Ord) { return end; }
}

#define FIND_IN_SET(x) find(x.begin(), x.end(), 10)
// CHECK-FIXES: #define FIND_IN_SET(x) find(x.begin(), x.end(), 10)

template <typename T> void f(const T &t) {
  std::set<int> s;
  // [CXX-P2000]: "Using inefficient algorithm `find`"
  find(s.begin(), s.end(), 46);

  find(t.begin(), t.end(), 46);
}

int main() {
  std::set<int> s;
  // [CXX-P2000]: "Using inefficient algorithm `find`"
  auto it = std::find(s.begin(), s.end(), 43);
  // [CXX-P2000]: "Using inefficient algorithm `count`"
  auto c = count(s.begin(), s.end(), 43);

  // FIXME: Not raising issue due to https://linear.app/deepsource/issue/LAE-6675/raise-issues-in-macro-use-location
#define SECOND(x, y, z) y
  SECOND(q,std::count(s.begin(), s.end(), 22),w);

  it = find_if(s.begin(), s.end(), [](int) { return false; });

  std::multiset<int> ms;
  // [CXX-P2000]: "Using inefficient algorithm `find`"
  find(ms.begin(), ms.end(), 46);

  const std::multiset<int> &msref = ms;
  // [CXX-P2000]: "Using inefficient algorithm `find`"
  find(msref.begin(), msref.end(), 46);

  std::multiset<int> *msptr = &ms;
  // [CXX-P2000]: "Using inefficient algorithm `find`"
  find(msptr->begin(), msptr->end(), 46);

  // FIXME: Raise a new issue here!
  it = std::find(s.begin(), s.end(), 43, std::greater<int>());
  // CHECK-MESSAGES: :[[@LINE-1]]:42: warning: different comparers used in the algorithm and the container -performance-inefficient-algorithm-

  std::set<int, std::greater<int>> news;
  // [CXX-P2000]: "Using inefficient algorithm `find`"
  it = std::find(news.begin(), news.end(), 43, std::greater<int>());
  
  // FIXME: Not raising issue due to https://linear.app/deepsource/issue/LAE-6675/raise-issues-in-macro-use-location
  FIND_IN_SET(s);

  f(s);

  std::unordered_set<int> us;
  lower_bound(us.begin(), us.end(), 10);
  // [CXX-P2000]: "Using inefficient algorithm `find`"
  find(us.begin(), us.end(), 10);

  std::unordered_multiset<int> ums;
  // [CXX-P2000]: "Using inefficient algorithm `find`"
  find(ums.begin(), ums.end(), 10);

  std::map<int, int> intmap;
  // [CXX-P2000]: "Using inefficient algorithm `find`"
  find(intmap.begin(), intmap.end(), 46);

  std::multimap<int, int> intmmap;
  // [CXX-P2000]: "Using inefficient algorithm `find`"
  find(intmmap.begin(), intmmap.end(), 46);

  std::unordered_map<int, int> umap;
  // [CXX-P2000]: "Using inefficient algorithm `find`"
  find(umap.begin(), umap.end(), 46);

  std::unordered_multimap<int, int> ummap;
  // [CXX-P2000]: "Using inefficient algorithm `find`"
  find(ummap.begin(), ummap.end(), 46);
}

struct Value {
  int value;
};

struct Ordering {
  bool operator()(const Value &lhs, const Value &rhs) const {
    return lhs.value < rhs.value;
  }
  bool operator()(int lhs, const Value &rhs) const { return lhs < rhs.value; }
};

void g(std::set<Value, Ordering> container, int value) {
  // [CXX-P2000]: "Using inefficient algorithm `lower_bound`"
  lower_bound(container.begin(), container.end(), value, Ordering());
}
