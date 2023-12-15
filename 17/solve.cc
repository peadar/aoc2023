#include <bits/stdc++.h>
#include "../aoc.h"

namespace {
struct Location {
   size_t row = 0, col = 0;
   Location operator + ( const std::pair<int, int> &rhs) { return { row + rhs.first, col + rhs.second } ; }
   auto operator <=> (const Location &rhs) const = default;
};

using Distance = unsigned long;
Distance constexpr INFINITE = std::numeric_limits<Distance>::max();

enum Cardinal : uint8_t {
   NORTH, WEST, SOUTH, EAST
};

std::ostream &operator <<(std::ostream &os, Cardinal c) {
   switch (c) {
      case NORTH: return os << "^";
      case SOUTH: return os << "v";
      case EAST: return os << ">";
      case WEST: return os << "<";
      default: return os << "?";
   }
}

const std::pair<int, int> cardinal[] {
   { -1, 0 },
   { 0, -1 },
   { 1, 0 },
   { 0, 1 },
};


const std::pair<int, int> operator * ( const std::pair<int, int> &vec, int scalar)  {
   return { vec.first * scalar, vec.second * scalar };
}

struct Node {
   Location location;
   Cardinal direction;
   uint8_t remaining;
   Distance distance;
   Node *prev;
   Node(const Location &location_, Cardinal direction_, uint8_t remaining_, Distance distance_)
      : location(location_)
      , direction(direction_)
      , remaining(remaining_)
      , distance(distance_)
      , prev(nullptr)
   { }
};

std::ostream &operator <<(std::ostream &os, const Location &l) {
   return os << "{ " << l.row << ", " << l.col << " }";
}

[[maybe_unused]]
std::ostream &operator <<(std::ostream &os, const Node &n) {
   return os << "{ location: " << n.location << ", direction: " << n.direction << ", distance: " << n.distance
      << ", remaining: " << int(n.remaining) << " }";
}

struct Grid {
   std::vector<std::vector<uint8_t>> costs;
   std::map<Location, std::map<std::tuple<Cardinal, int>, Node *>> allNodes;
   bool contains(const Location &l) const {
      return l.row < costs.size() && l.col < costs[0].size();
   }
   Grid(std::istream &);
   void dijkstra(size_t minstep, size_t maxremain);
   Distance solve(size_t, size_t);
};

Grid::Grid(std::istream &is) {
   for (std::string line; std::getline(is, line) && is; ) {
      costs.emplace_back();
      for (char c: line)
         costs.back().push_back(uint8_t(c - '0'));
   }
}

void Grid::dijkstra(size_t minstep, size_t maxremain) {
   struct LTNode {
      bool operator()(const Node *l, const Node *r) { return l->distance < r->distance; }
   } compare;
   std::vector<Node *> q;

   auto a = new Node({0, 0}, WEST, maxremain, 0);
   auto b = new Node({0, 0}, NORTH, maxremain, 0);
   q.emplace_back( a );
   q.emplace_back( b );
   allNodes[Location{0,0}][std::make_tuple(WEST, 1)] = a;
   while (!q.empty()) {
      // get the current lowest-cost node.
      auto it = std::min_element(q.begin(), q.end(), compare);
      if (it != q.end() - 1)
         std::swap(*it, *(q.end() - 1));
      Node *cur = q.back();
      q.pop_back();

      // check each adjacent node.
      for (auto turn : { -1, 1, 0 }) {
         auto newdir = Cardinal((cur->direction + turn + 4) % 4);
         auto newpos = cur->location + cardinal[newdir] * (turn == 0 ? 1 : minstep);
         if (!contains(newpos))
            continue;
         int newremaining;
         if (turn == 0) {
            if (cur->remaining == 0)
               continue;
            newremaining = cur->remaining - 1;
         } else {
            newremaining = maxremain - minstep;
         }
         auto &adj = allNodes[newpos][std::make_tuple(newdir, newremaining)];
         if (adj == nullptr) {
            // not seen before, place at the end of the heap with infinite distance.
            adj = new Node(newpos, newdir, newremaining, INFINITE);
            q.push_back(adj);
         }

         int cost = 0;
         for (Location loc = adj->location;
               loc != cur->location;
               loc = loc + cardinal[(6 + newdir) % 4]) {
            cost += costs[loc.row][loc.col];
         }
         auto newtent = cur->distance + cost;
         if (newtent < adj->distance) {
            adj->distance = newtent;
            adj->prev = cur;
         }
      }
   }
}

void showpath(Node *n) {
   if (n->prev)
      showpath(n->prev);
   std::cout << *n << "\n";
}

Distance Grid::solve(size_t step, size_t maxremain) {
   dijkstra(step, maxremain);
   auto endpos = Location{ costs.size() - 1, costs[0].size() - 1 };
   Node *bestn = nullptr;
   for (auto &[key, n] : allNodes[endpos]) {
      if (n->location == endpos) {
         if (bestn == nullptr || n->distance < bestn->distance)
            bestn = n;
      }
   }
   //showpath(bestn);
   return bestn->distance;
}

void part1(std::istream &is, std::ostream &os) {
   Grid g{is};
   os << g.solve(1, 3);
}
void part2(std::istream &is, std::ostream &os) {
   Grid g{is};
   os << g.solve(4, 10);
}

aoc::Case p1("part1", part1);
aoc::Case p2("part2", part2);
}
