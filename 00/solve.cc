#include "../aoc.h"

struct Day {
};

Day parse([[maybe_unused]] std::istream &is) {
   return Day();
}

aoc::Case parseFn("parse",
      [](std::istream &is, std::ostream &) { parse(is); });

aoc::Case part1("part1", [](std::istream &is, [[maybe_unused]] std::ostream &os) {
   auto day = parse(is);
});

aoc::Case part2("part2", [](std::istream &is, [[maybe_unused]] std::ostream &os) {
   auto day = parse(is);
});
