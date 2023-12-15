#include <bits/stdc++.h>
#include "../aoc.h"

namespace {
using Location = std::pair<int, int>;

struct Tile {
   char content, directions; // one bit each for NSEW.
   Tile(char c) : content{c}, directions{0} {}
};

struct Grid {
   std::vector<std::vector<Tile>> d;
   unsigned long energy() {
      unsigned long total = 0;
      for (auto &row : d)
         for (auto & col : row)
            if (col.directions != 0) {
               ++total;
               col.directions = 0;
            }
      return total;
   }
   int rowcount() const noexcept { return d.size(); }
   int colcount() const noexcept { return d[0].size(); }
   bool contains(Location l) const noexcept {
      return l.first >= 0 &&
         l.first < rowcount() &&
         l.second >= 0 && l.second < colcount();
   }
   Grid & cast(Location l, Location d);
   Grid(std::istream &);
};

Grid &Grid::cast(Location l, Location dir) {
   for (;;) {
      int pos = dir.first == -1 ? 0 : dir.first == 1 ? 1 : dir.second == -1 ? 2 : 3;
      l.first += dir.first;
      l.second += dir.second;
      if (!contains(l))
         return *this;
      Tile &tile = d[l.first][l.second];
      if (tile.directions & (1<<pos))
         return *this;
      tile.directions |= (1 << pos);
      switch (tile.content) {
         case  '/':
            dir.first = - dir.first;
            dir.second = - dir.second;
            /* fallthrough */
         case '\\':
            std::swap(dir.first, dir.second);
            break;
         case '|':
            if (dir.second) {
               cast(l, {-1, 0});
               cast(l, {1, 0});
               return *this;
            }
            break;
         case '-':
            if (dir.first) {
               cast(l, {0, 1});
               cast(l, {0, -1});
               return *this;
            }
            break;
         default:
            break;
      }
   }
}

Grid::Grid(std::istream &is) {
   for (std::string line; std::getline(is, line) && is; ) {
      d.emplace_back();
      std::copy(line.begin(), line.end(), std::back_inserter(d.back()));
   }
}
}

aoc::Case part1( "part1", [](std::istream &is, std::ostream &os) {
   os << std::move(Grid{is}).cast({0, -1}, {0, 1}).energy();
});

aoc::Case part2( "part2", [](std::istream &is, std::ostream &os) {
   Grid g{is};
   unsigned long maxenergy = 0;
   auto dopos = [&](Location start, Location d) {
      g.cast(start, d);
      maxenergy = std::max(maxenergy, g.energy());
   };
   for (int row = 0; row < g.rowcount();  ++row) {
      dopos({row, -1}, {0, 1});
      dopos({row, g.colcount()}, {0, -1});
   }
   for (int col = 0; col < g.colcount();  ++col) {
      dopos({-1, col}, {1, 0});
      dopos({g.rowcount(), col}, {-1,0});
   }
   os << maxenergy;
});
