#include "../aoc.h"

struct Location {
   long row = 0, col = 0;
   Location &operator += ( const Location &r) {
      row += r.row;
      col += r.col;
      return *this;
   }
};

struct Insn {
   char direction;
   uint8_t steps;
   int color;
   template <typename Dir, typename Steps> Location process(Location start, Dir dir, Steps steps) const {
      auto cnt = steps(*this);
      switch (dir(*this)) {
         case 'R': return { start.row, start.col + cnt };
         case 'D': return { start.row + cnt, start.col };
         case 'L': return { start.row, start.col - cnt };
         case 'U': return { start.row - cnt, start.col };
         default: abort();
      }
   }
};
using Insns = std::vector<Insn>;

Insns parse(std::istream &is) {
   Insns insns;
   for (int line = 0;; ++line) {
      char direction, lbrack, hash, rbrack;
      int count, color;
      is >> direction >> count >> lbrack >> hash >> std::hex >> color >> std::dec >> rbrack;
      if (!is)
         break;
      insns.emplace_back(direction, count, color);
   }
   return insns;
}

template <typename GetDir, typename GetSteps> long
shoelace(std::istream &is, GetDir dir, GetSteps steps) {
   Insns insns = parse(is);
   std::vector<Location> points;
   Location topleft, bottomright, current;
   // Get the sequence of points, and the stepcount.
   points.push_back(current);
   long stepcnt = 0;
   for (auto &insn: insns) {
      current = insn.process(current, dir, steps);
      stepcnt += steps(insn);
      points.push_back(current);
   }
   // do shoelace.
   long shoelace = 0;
   auto sz = points.size();
   for (size_t i = 0; i < sz; ++i)
      shoelace += (points[i].row) * (points[(i+sz-1)%sz].col - points[(i+sz+1)%sz].col);
   if (shoelace < 0)
      shoelace = -shoelace;
   shoelace /= 2;
   return stepcnt / 2 + 1 + shoelace; // Add  half + 1 of the exterior points
}

aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {
   os << shoelace(is,
         [](const Insn &l) { return l.direction; },
         [](const Insn &l) { return l.steps; }
       );});

aoc::Case part2("part2", [](std::istream &is, std::ostream &os) {
   static const char dirs[] = "RDLU";
   os << shoelace(is,
         [](const Insn &l) { return dirs[l.color & 0xf]; },
         [](const Insn &l) { return l.color >> 4; }
       );});

