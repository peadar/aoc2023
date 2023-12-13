#include "../aoc.h"
#include <vector>
#include <iterator>
#include <unistd.h>
#include <optional>
#include <cassert>

using BITS = uint64_t;

struct GRID {
   std::vector<BITS> data;
   size_t cols = 0;
   GRID rot90() const;

   template <typename It>
   std::optional<size_t> palindromeAt(It begin, It end) {
      for (auto s = begin; s < end - 1; ++s) {
         for (auto p = s, e = end;; ++p, --e) {
            if (p == e) {
               return p - begin;
            }
            if (*p != e[-1])
               break;
         }
      }
      return std::nullopt;
   }

   std::optional<size_t> palindromeAtNR() {
      auto p = palindromeAt(data.begin(), data.end());
      if (p)
         return *p;
      p = palindromeAt(data.rbegin(), data.rend());
      if (p)
         return data.size() - *p;
      return std::nullopt;
   }

   size_t palindromeAt() {
      auto p = palindromeAtNR();
      if (p)
         return *p * 100;
      auto rot = this->rot90();
      p = rot.palindromeAtNR();
      assert(p);
      return *p;
   }
};

GRID
GRID::rot90() const {
   GRID g;
   g.cols = data.size();
   g.data.resize(cols);
   for (BITS fromrow = 0; fromrow < data.size(); ++fromrow)
      for (BITS fromcol = 0; fromcol < cols; ++fromcol) {
         if (data[fromrow] & (1 << fromcol))
            g.data[fromcol] |= (1 << (g.cols - 1 - fromrow));
   }
   return g;
}


using GRIDS = std::vector<GRID>;

GRIDS parse(std::istream &is) {
   GRIDS result;
   GRID cur;
   for (std::string line;;) {
      std::getline(is, line);
      if (line == "") {
         result.push_back(std::move(cur));
         cur = {};
         if (!is)
            break;
         continue;
      }
      BITS b = 0;
      cur.cols = 0;
      for (auto c : line) {
         if (c == '#')
            b |= 1 << cur.cols;
         cur.cols++;
      }
      cur.data.push_back(b);
   }

   return result;
}

std::ostream &operator <<(std::ostream &os, const GRID &grid) {
   for (auto datum : grid.data) {
      for (int i = 0; i < grid.cols; ++i)
         os << (datum & (1 << i )? '#':'.');
      os << "\n";
   }
   return os << "\n";
}

void part1(std::istream &is, std::ostream &os) {
   auto day = parse(is);
   unsigned long sum = 0;
   for (auto &orig : day) {
      auto val = orig.palindromeAt();
      sum += val;
   }
   os << "part1: " << sum << "\n";
}

void part2(std::istream &is, std::ostream &os) {
   auto day = parse(is);
   /*
   for (auto &d : day) {
      os << d;
      os << "\n";
      os << d.rot90();
      os << "\n";
      os << "\n";
   }
   */
}
