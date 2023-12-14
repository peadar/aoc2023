#include "../aoc.h"
#include <vector>
#include <algorithm>


struct Day {
   std::vector<std::vector<char>> grid;
   Day(std::istream &is);
 
   template <typename Direction> void shuffle(const Direction &d);
   long tally();
   void cycle();
   auto operator <=>(const Day &) const = default;
};

struct North {
   Day &d;
   size_t majorsize() const noexcept { return d.grid.size(); }
   size_t minorsize() const noexcept { return d.grid[0].size(); }
   char &operator ()(size_t maj, size_t min) const noexcept { return d.grid[maj][min]; }
   North(Day &d_) : d(d_) {}
};

struct South {
   Day &d;
   size_t majorsize() const noexcept { return d.grid.size(); }
   size_t minorsize() const noexcept { return d.grid[0].size(); }
   char &operator ()(size_t maj, size_t min) const noexcept { return d.grid[d.grid.size() - 1 - maj][min]; }
   South(Day &d_) : d(d_) {}
};

struct West {
   Day &d;
   size_t majorsize() const noexcept { return d.grid[0].size(); }
   size_t minorsize() const noexcept { return d.grid.size(); }
   char &operator ()(size_t maj, size_t min) const noexcept { return d.grid[min][maj]; }
   West(Day &d_) : d(d_) {}
};

struct East {
   Day &d;
   size_t majorsize() const noexcept { return d.grid[0].size(); }
   size_t minorsize() const noexcept { return d.grid.size(); }
   char &operator ()(size_t maj, size_t min) const noexcept { return d.grid[min][d.grid[0].size() - 1 - maj]; }
   East(Day &d_) : d(d_) {}
};



Day::Day(std::istream &is) {
   for (std::string line; std::getline(is, line) && is; ) {
      grid.emplace_back();
      std::copy(line.begin(), line.end(), std::back_inserter(grid.back()));
   }
}

template <typename Direction>
void
Day::shuffle(const Direction &d) {
   for (size_t r = 1; r < d.majorsize(); ++r) {
      for (size_t c = 0; c < d.minorsize(); ++c) {
         if (d(r, c) == 'O') {
            for (size_t newr = r - 1; newr < d.majorsize() && d(newr, c) == '.' ; --newr) {
               std::swap(d(newr,c), d(newr+1, c));
            }
         }
      }
   }
}

long
Day::tally() {
   long weight = 1;
   long result = 0;
   for (auto row = grid.rbegin(); row != grid.rend(); ++row, ++weight) {
      result += weight * std::count(row->begin(), row->end(), 'O');
   }

   for (size_t r = 1; r < grid.size(); ++r) {
      for (size_t c = 0; c < grid[r].size(); ++c) {
         if (grid[r][c] == 'O') {
            for (size_t newr = r - 1; newr < grid.size() && grid[newr][c] == '.' ; --newr) {
               std::swap(grid[newr][c], grid[newr+1][c]);
            }
         }
      }
   }
   return result;
}

void Day::cycle() {
   North n(*this);
   South s(*this);
   East e(*this);
   West w(*this);
   shuffle(n);
   shuffle(w);
   shuffle(s);
   shuffle(e);
}

std::ostream &operator <<(std::ostream &os, const Day &day) {
   for (auto &row : day.grid) {
      for (auto &col : row) {
         os << col;
      }
      os << "\n";
   }
   return os << "\n";
}


void part1(std::istream &is, std::ostream &os) {
   Day day { is };

   North n(day);
   day.shuffle(n);
   day.tally();

   os << "part1: " << day.tally() << "\n";
}

void part2(std::istream &is, std::ostream &os) {

   Day day { is };
   Day start = day;
   for (int i = 0; i < 1000000000 ; ++i) {
      day.cycle();
      if (day == start) {
         os << "period: " << i << "\n";
         break;
      }
   }
}
