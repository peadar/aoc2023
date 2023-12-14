#include "../aoc.h"
#include <vector>
#include <algorithm>

struct Day {
   std::vector<std::vector<char>> grid;
   Day(std::istream &is);
   
   void northify();
   long tally();

};

Day::Day(std::istream &is) {
   for (std::string line; std::getline(is, line) && is; ) {
      grid.emplace_back();
      std::copy(line.begin(), line.end(), std::back_inserter(grid.back()));
   }
}

void
Day::northify() {
   for (size_t r = 1; r < grid.size(); ++r) {
      for (size_t c = 0; c < grid[r].size(); ++c) {
         if (grid[r][c] == 'O') {
            for (size_t newr = r - 1; newr < grid.size() && grid[newr][c] == '.' ; --newr) {
               std::swap(grid[newr][c], grid[newr+1][c]);
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

void part1(std::istream &is, std::ostream &os) {
   Day day { is };
   day.northify();
   day.tally();

   os << "part1: " << day.tally() << "\n";
}

void part2(std::istream &is, std::ostream &os) {
}
