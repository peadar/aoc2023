#include "../aoc.h"
using Location = std::pair<long, long>;
using Insn = std::tuple<char, uint8_t, int>;

std::istream &operator >> (std::istream &is, Insn &insn) {
   char _;
   int count;
   is >> get<0>(insn) >> count >> _ >> _ >> std::hex >> get<2>(insn) >> std::dec >> _;
   get<1>(insn) = count;
   return is;
}

template <typename GetDir, typename GetSteps> long
shoelace(std::istream &is, GetDir dir, GetSteps steps) {
   std::vector<Location> points;
   Location current;
   points.push_back(current);
   long stepcnt = 0;
   for (Insn insn;; stepcnt += steps(insn)) {
      if (!(is >> insn))
         break;
      switch (dir(insn)) {
         case 'R': current.second += steps(insn); break;
         case 'D': current.first += steps(insn); break;
         case 'L': current.second -= steps(insn); break;
         case 'U': current.first -= steps(insn); break;
         default: abort();
      }
      points.push_back(current);
   }
   long shoelace = 0;
   for (size_t i = 0, sz = points.size(); i < sz; ++i)
      shoelace += (points[i].first) * (points[(i+sz-1)%sz].second - points[(i+sz+1)%sz].second);
   shoelace = std::abs(shoelace) / 2;
   return stepcnt / 2 + 1 + shoelace; // Add  half + 1 of the exterior points
}

aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {
   os << shoelace(is,
         [](const Insn &l) { return get<0>(l); },
         [](const Insn &l) { return get<1>(l); });});

aoc::Case part2("part2", [](std::istream &is, std::ostream &os) {
   static const char dirs[] = "RDLU";
   os << shoelace(is,
         [](const Insn &l) { return dirs[get<2>(l) & 0xf]; },
         [](const Insn &l) { return get<2>(l) >> 4; });});
