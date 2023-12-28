#include <bits/stdc++.h>
#include "../aoc.h"

namespace {
struct Location {
   unsigned row = 0, col = 0;
   Location operator + ( const std::pair<int, int> &rhs) const { return { row + rhs.first, col + rhs.second } ; }
   auto operator <=> (const Location &rhs) const = default;
};

using Distance = unsigned;
Distance constexpr INFINITE = std::numeric_limits<Distance>::max();

enum Cardinal : uint8_t { NORTH, WEST, SOUTH, EAST };

const std::pair<int, int> cardinal[] { { -1, 0 }, { 0, -1 }, { 1, 0 }, { 0, 1 }, };

const std::pair<int, int> operator * ( const std::pair<int, int> &vec, int scalar)  {
   return { vec.first * scalar, vec.second * scalar };
}

struct Key {
   Location location;
   Cardinal direction;
   uint8_t remaining;
   Key(const Location &location_, Cardinal direction_, uint8_t remaining_)
      : location(location_)
      , direction(direction_)
      , remaining(remaining_)
   { }
   auto operator <=> (const Key &rhs) const = default;
};
}

namespace std {
template <> struct hash<Key> {
   size_t operator() (const Key &k) const {
      return (k.location.row << 24) ^ (k.location.col << 8 ) ^ (k.direction << 4) ^ k.remaining;
   }
};
}

namespace {

struct Value {
   Distance distance;
   Value() : distance(INFINITE) {}
};

struct Grid {
   std::vector<std::vector<uint8_t>> costs;
   std::unordered_map<Key, Value> visited;
   bool contains(const Location &l) const { return l.row < costs.size() && l.col < costs[0].size(); }
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

   // a proper priority queue for "unvisited" would be better. The time taken
   // to min_element this dominates everything.
   std::unordered_map<Key, Value> unvisited;

   unvisited[ Key{ {0, 0}, WEST, uint8_t(maxremain) } ].distance = 0;
   unvisited[ Key{ {0, 0}, NORTH, uint8_t(maxremain) } ].distance = 0;

   while (!unvisited.empty()) {
      // get the current lowest-cost node.
      auto it = std::min_element(unvisited.begin(), unvisited.end(),
         [this](const auto &l, const auto &r) { return l.second.distance < r.second.distance; } );
      const Key key = it->first;
      const Value value = it->second;
      unvisited.erase(it);
      visited[key] = value;

      // check each adjacent node.
      for (auto turn : { -1, 1, 0 }) {
         auto newdir = Cardinal((key.direction + turn + 4) % 4);
         auto newpos = key.location + cardinal[newdir] * (turn == 0 ? 1 : minstep);
         if (!contains(newpos))
            continue;
         int newremaining;
         if (turn == 0) {
            if (key.remaining == 0)
               continue;
            newremaining = key.remaining - 1;
         } else {
            newremaining = maxremain - minstep;
         }
         auto newkey = Key(newpos, newdir, newremaining);
         if (visited.find(newkey) != visited.end())
            continue;
         Value &adj = unvisited[Key(newpos, newdir, newremaining)];
         int cost = 0;
         for (Location loc = newpos;
               loc != key.location;
               loc = loc + cardinal[(6 + newdir) % 4]) {
            cost += costs[loc.row][loc.col];
         }
         auto newtent = value.distance + cost;
         if (newtent < adj.distance)
            adj.distance = newtent;
      }
   }
}

Distance Grid::solve(size_t step, size_t maxremain) {
   dijkstra(step, maxremain);
   auto endpos = Location{ unsigned(costs.size() - 1), unsigned(costs[0].size() - 1) };
   Distance best = INFINITE;
   for (auto &[key, val] : visited)
      if (key.location == endpos)
         if (val.distance < best)
            best = val.distance;
   return best;
}

aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {
   Grid g{is};
   os << g.solve(1, 3);
});

aoc::Case part2("part2",  [](std::istream &is, std::ostream &os) {
   Grid g{is};
   os << g.solve(4, 10);
});
}
