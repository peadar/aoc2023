#include <iostream>
#include <vector>
#include <algorithm>

namespace {

struct Day {
   std::vector<std::vector<char>> grid;
   Day(std::istream &is);
   template <typename Direction> Day &shuffle(const Direction &d) noexcept;
   long tally() const noexcept;
   Day &cycle() noexcept;
   bool operator ==(const Day &) const = default;
};

// There is definitely a more succinct way to do all this...
// Share the same shuffle() code with classes to hide the details of how the
// cardinal direction affects the indexing.
struct Vertical {
   size_t majorsize(Day &d) const noexcept { return d.grid.size(); }
   size_t minorsize(Day &d) const noexcept { return d.grid[0].size(); }
};

struct Horizontal {
   size_t majorsize(Day &d) const noexcept { return d.grid[0].size(); }
   size_t minorsize(Day &d) const noexcept { return d.grid.size(); }
};

struct North : Vertical {
   char &operator ()(Day &d, size_t maj, size_t min) const noexcept { return d.grid[maj][min]; }
};

struct South : Vertical {
   char &operator ()(Day &d, size_t maj, size_t min) const noexcept { return d.grid[d.grid.size() - 1 - maj][min]; }
};

struct West : Horizontal {
   char &operator ()(Day &d, size_t maj, size_t min) const noexcept { return d.grid[min][maj]; }
};

struct East : Horizontal {
   char &operator ()(Day &d, size_t maj, size_t min) const noexcept { return d.grid[min][d.grid[0].size() - 1 - maj]; }
};

Day::Day(std::istream &is) {
   for (std::string line; std::getline(is, line) && is; ) {
      grid.emplace_back();
      std::copy(line.begin(), line.end(), std::back_inserter(grid.back()));
   }
}

template <typename Direction> Day & Day::shuffle(const Direction &d) noexcept {
   for (size_t r = 1; r < d.majorsize(*this); ++r)
      for (size_t c = 0; c < d.minorsize(*this); ++c)
         if (d(*this, r, c) == 'O')
            for (size_t newr = r - 1; newr < d.majorsize(*this) && d(*this, newr, c) == '.' ; --newr)
               std::swap(d(*this, newr,c), d(*this, newr+1, c));
   return *this;
}

long Day::tally() const noexcept {
   long weight = grid.size(), result = 0;
   for (auto row : grid)
      result += weight-- * std::count(row.begin(), row.end(), 'O');
   return result;
}

Day & Day::cycle() noexcept {
   return shuffle(North{}).shuffle(West{}).shuffle(South{}).shuffle(East{});
}

}

void part1(std::istream &is, std::ostream &os) {
   os << "part1: " << Day{is}.shuffle(North{}).tally() << "\n";
}

void part2(std::istream &is, std::ostream &os) {
   Day day { is }, old { day };
   long count, period = 0;
   constexpr long target = 1000000000;

   // First find a cycle by advancing day twice as fast as old
   for (count = 1, day.cycle(); day != old;++count) {
      day.cycle();
      if (count % 2 == 0)
         old.cycle();
   }
   // Keep counting until we find the pattern repeats again, to get the cycle length.
   do {
      day.cycle();
      period++;
   } while (day != old);
   // Now run the remaining iterations until it's its as if we ran to the target.
   for (int i = 0; i < (target - count) % period; ++i)
      day.cycle();
   os << "part2: " << day.tally() << "\n";
}
