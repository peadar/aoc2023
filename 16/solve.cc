#include <iostream>
#include <vector>
#include <algorithm>

namespace {
struct Location {
   int row, col;
   Location &operator += (Location rhs) {
      row += rhs.row;
      col += rhs.col;
      return *this;
   }
   Location operator + (Location d) const {
      Location neu = *this;
      neu += d;
      return neu;
   }
};

struct Tile {
   char content, directions; // one bit each for NSEW.
   Tile(char c) : content{c}, directions{0} {}
};

struct Grid {
   std::vector<std::vector<Tile>> d;
   Tile &operator[](Location l) { return d[l.row][l.col]; }
   unsigned long energy() {
      unsigned long total = 0;
      for (auto row : d)
         for (const auto & col : row)
            if (col.directions != 0)
               ++total;
      return total;
   }
   int rowcount() { return d.size(); }
   int colcount() { return d[0].size(); }
   bool contains(Location l) {
      return l.row >= 0 && l.row < rowcount() && l.col >= 0 && l.col < colcount();
   }
   void clear() {
      for (auto &row : d)
         for (auto &tile : row)
            tile.directions = 0;
   }
   void cast(Location l, Location d);
   Grid(std::istream &);
};

void Grid::cast(Location l, Location d) {
   auto dirbit = [](const Location &l)
      { return l.row == -1 ? 0 : l.row == 1 ? 1 : l.col == -1 ? 2 : 3; };
   int pos = dirbit(d);
   for (;;) {
      l += d;
      if (!contains(l))
         return;
      Tile &tile = (*this)[l];
      if (tile.directions & (1<<pos))
         return;
      tile.directions |= (1 << pos);
      switch (tile.content) {
         case  '/':
            d.row = - d.row;
            d.col = - d.col;
            /* fallthrough */
         case '\\':
            std::swap(d.row, d.col);
            pos = dirbit(d);
            break;
         case '|':
            if (d.col) {
               cast(l, {-1, 0});
               cast(l, {1, 0});
               return;
            }
            break;
         case '-':
            if (d.row) {
               cast(l, {0, 1});
               cast(l, {0, -1});
               return;
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

void part1(std::istream &is, std::ostream &os) {
   Grid g{is};
   g.cast({0, -1}, {0, 1});
   os << "part 1: " << g.energy() << "\n";
}

void part2(std::istream &is, std::ostream &os) {
   Grid g{is};
   unsigned long maxenergy = 0;
   auto dopos = [&](Location start, Location d) {
      g.cast(start, d);
      maxenergy = std::max(maxenergy, g.energy());
      g.clear();
   };
   for (int row = 0; row < g.rowcount();  ++row) {
      dopos({row, -1}, {0, 1});
      dopos({row, g.colcount()}, {0, -1});
   }
   for (int col = 0; col < g.colcount();  ++col) {
      dopos({-1, col}, {1, 0});
      dopos({g.rowcount(), col}, {-1,0});
   }
   os << "part 2: " << maxenergy <<  "\n";
}
