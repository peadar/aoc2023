#include "../aoc.h"
#include <map>
#include <vector>
#include <string>
#include <map>
#include <unordered_map>

namespace {

struct Node {
   std::string name;
   size_t l, r;
};

std::ostream & operator << (std::ostream &os, const Node &node) {
   return os << "Node{" << node.name << "}";
}

struct size_t_dflt {
   size_t s = size_t(-1);
   operator size_t &() { return s; }
   size_t operator = (size_t neu) {
      return s = neu;
   }
};

struct Graph {
   std::vector<Node> nodes;
   std::string path;
   std::map<std::string, size_t_dflt> name2idx;

   auto getnode(const std::string &n) {
      auto &s = name2idx[n];
      if (s == size_t(-1)) {
         s = nodes.size();
         nodes.emplace_back(n, -1, -1);
      }
      return s;
   };
};

Graph parse(std::istream &is) {
   Graph rv;

   std::getline(is, rv.path);

   std::string l;
   std::getline(is, l);

   for (;;) {
      std::string name, l, r, discard;
      std::getline(is, name, ' ');
      if (name == "")
         break;
      std::getline(is, discard, '(');
      std::getline(is, l, ',');
      is.get(); // the space after the comma
      std::getline(is, r, ')');
      std::getline(is, discard); // skip to end-of-line

      auto self = rv.getnode(name);
      rv.nodes[self].l = rv.getnode(l);
      rv.nodes[self].r = rv.getnode(r);
   }
   return rv;
}

template <typename Iter> auto lcm(Iter begin, Iter end) {
   using I = Iter::value_type;
   std::unordered_map<I, I> factors;
   I max = *std::max_element(Iter(begin), end);
   std::vector<bool> notprime(max);
   bool done = false;
   for (I curprime = 2; !done; ) {
      size_t maxcnt = 0;
      for (Iter cb = begin; cb != end; ++cb) {
         size_t cnt = 0;
         done = true;
         while (*cb % curprime == 0) {
            *cb /= curprime;
            cnt++;
         }
         if (*cb != 1)
            done = false;
         maxcnt = std::max(maxcnt, cnt);
      }
      if (maxcnt)
         factors[curprime] = maxcnt;
      for (I filter = curprime * 2; filter <= max; filter += curprime)
         notprime[filter] = true;
      for (++curprime; notprime[curprime]; ++curprime)
         ;
   }
   ulong lcm = 1;
   for (auto &[prime, cnt] : factors)
      while (cnt--)
         lcm *= prime;
   return lcm;
}
}

void part1(std::istream &is, std::ostream &os) {
   auto g = parse(is);
   const char *p = g.path.data();
   const char *e = p + g.path.size();

   size_t start = g.getnode("AAA");
   size_t end = g.getnode("ZZZ");

   size_t i, count;
   for (count = 0, i = start; i != end; ++count) {
      Node &n = g.nodes[i];
      switch (*p++) {
         case 'L': i = n.l; break;
         case 'R': i = n.r; break;
         default: __builtin_unreachable();
      }
      if (p == e)
         p = g.path.data();
   }
   os << "part1: " << count << "\n";
}

void part2(std::istream &is, std::ostream &os) {
   auto g = parse(is);

   std::vector<ulong> pathlens;
   for (size_t start = 0; start < g.nodes.size(); ++start) {
      if (g.nodes[start].name[2] != 'A')
         continue;
      const char *p = g.path.data();
      const char *e = p + g.path.size();

      size_t i, pathlen;
      for (pathlen = 0, i = start; g.nodes[i].name[2] != 'Z'; ++pathlen) {
         Node &n = g.nodes[i];
         switch (*p++) {
            case 'L': i = n.l; break;
            case 'R': i = n.r; break;
            default: __builtin_unreachable();
         }
         if (p == e)
            p = g.path.data();
      }
      pathlens.push_back(pathlen);
   }
   os << "part2: " << lcm(pathlens.begin(), pathlens.end()) << std::endl;
}
