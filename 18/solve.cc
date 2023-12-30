#include "../aoc.h"
#include <iomanip>
#include <map>

using Color = uint32_t;

struct Location {
   int row = 0, col = 0;
   Location operator + ( const Location &r) const { return { row + r.row, col + r.col } ; }
   Location &operator += ( const Location &r) {
      row += r.row;
      col += r.col;
      return *this;
   }
   auto operator <=> (const Location &rhs) const = default;
};

std::ostream &operator <<(std::ostream &os, const Location &l) {
   return os << " { row: " << l.row << ", col: " << l.col << " }";
}

struct Insn {
   char direction;
   uint8_t steps;
   Color color;
   Location process(Location start) {
      switch (direction) {
         case 'U': return { start.row - steps, start.col };
         case 'D': return { start.row + steps, start.col };
         case 'L': return { start.row, start.col - steps };
         case 'R': return { start.row, start.col + steps };
         default: abort();
      }
   }
};

std::ostream &operator <<(std::ostream &os, const Insn &insn) {
   return os
      << insn.direction << " "
      << int(insn.steps)
      << " (#" << std::hex  << std::setfill('0') << std::setw(6)
      << insn.color << std::dec << ")\n";
}

using Insns = std::vector<Insn>;

std::ostream &operator <<(std::ostream &os, const Insns &insns) {
   for (auto &i : insns)
      os << i;
   return os;
}

Insns parse(std::istream &is, int maxlines=100000) {
   Insns insns;
   for (int line = 0;; ++line) {
      char direction, lbrack, hash, rbrack;
      int count;
      Color color;
      is >> direction >> count >> lbrack >> hash >> std::hex >> color >> std::dec >> rbrack;
      if (!is || line == maxlines)
         break;
      if (color == 0)
         abort();
      insns.emplace_back(direction, count, color);
   }
   return insns;
}

const std::map<char, Location> dirs {
   { 'U', { -1, 0 } },
   { 'D', { 1, 0 } },
   { 'L', { 0, -1 } },
   { 'R', { 0, 1 } },
};

struct Grid {
   int width, height;
   Location topleft;
   std::vector<Color> colors;

   Grid(Location tl, Location br) :
         width { (br.col - tl.col) + 3},
         height { (br.row - tl.row) + 3},
         topleft { tl.row - 1, tl.col - 1 } {
      colors.resize(width * height);
   }

   Color &operator[](const Location &l) {
      return colors[(l.row - topleft.row) * width + (l.col - topleft.col)];
   }

   bool contains(const Location &l) const {
      return
         l.row >= topleft.row &&
         l.col >= topleft.col  &&
         l.row - topleft.row < height &&
         l.col - topleft.col < width;
   }

   unsigned fill(const Location &pos, Color from, Color to) {
      if (!contains(pos))
         return 0;
      auto &cur = (*this)[pos];
      if (cur != from)
         return 0;
      cur = to;
      unsigned count = 1;
      for (const auto &d : dirs)
         count += fill(pos + d.second, from, to);
      return count;
   }
};

template <typename T> struct PBM {
   const T &data;
   PBM(const T &data_) : data(data_) {}
};

std::ostream &operator <<(std::ostream &os, const PBM<Color> &c) {
   return os
      << ((c.data >> 16) & 255)
      << " "
      << ((c.data >> 8) & 255)
      << " "
      << (c.data & 255);
}

std::ostream &operator <<(std::ostream &os, const PBM<Grid> &g) {
   os << "P3 " << g.data.width << " " << g.data.height << " 255\n";
   unsigned x = 0;
   for (auto &c : g.data.colors) {
      os << PBM(c);
      if (++x % g.data.width == 0)
         os << "\n";
      else
         os << " ";
   }
   return os << "\n";
}

Color mkcolor(int r, int g, int b) {
   return r << 16 | g << 8 | b;
}

Grid
fillPart1(std::istream &is,  std::ostream &os) {
   auto insns = parse(is, 700);

   // First, calculate the extent of the grid.
   Location topleft, bottomright, current;
   for (auto &insn: insns) {
        current = insn.process(current);
        topleft.row = std::min(current.row, topleft.row);
        topleft.col = std::min(current.col, topleft.col);
        bottomright.row = std::max(current.row, bottomright.row);
        bottomright.col = std::max(current.col, bottomright.col);
   }
   std::cerr << "end at " << current << "\n";
   Grid g(topleft, bottomright);
   g[{0,0}] = mkcolor(255, 255, 255);
   Location cursor {0, 0};
   // g[cursor] = mkcolor(255, 255, 255);
   for (auto &insn: insns) {
      const auto &dir = dirs.at(insn.direction);
      for (size_t i = 0; i < insn.steps; ++i) {
         cursor += dir;
         g[cursor] = insn.color;
      }
   }
   return g;
}

void
part1(std::istream &is,  std::ostream &os) {
   auto g = fillPart1(is, os);
   os << g.width * g.height - g.fill(g.topleft, mkcolor(0,0,0), mkcolor(20, 20, 20));
}

void
part1v(std::istream &is,  std::ostream &os) {
   auto g = fillPart1(is, os);
   os << PBM(g);
}

aoc::Case p1("part1", part1);
aoc::Case p1v("part1-visualization", part1v);
