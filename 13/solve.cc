#include "../aoc.h"
#include <vector>
#include <unistd.h>
#include <optional>
#include <cassert>

using BITS = uint64_t;

template <bool Smudge> struct GRID {
   std::vector<BITS> data;
   size_t cols = 0;
   GRID rot90() const;

   template <typename It> std::optional<size_t> findPalindrome(It begin, It end) {
      auto evenbegin = (end - begin) % 2 == 0 ? begin : begin + 1;
      for (auto s = evenbegin; s < end - 1; s += 2) {
         auto matches = (end - s)/2;
         assert((end - s) % 2 == 0);
         int zeroes = 0;
         int ones = 0;
         for (auto p = s, e = end;; ++p, --e) {
            if (p == e) {
               if ( zeroes == matches - Smudge && ones == Smudge)
                  return p - begin;
               break;
            }
            if (*p == e[-1])
               zeroes++;
            if (__builtin_popcount(*p ^ e[-1]) == 1)
               ones++;
         }
      }
      return std::nullopt;
   }

   std::optional<size_t> palindromeAtNR() {
      auto p = findPalindrome(data.begin(), data.end());
      if (p)
         return p;
      p = findPalindrome(data.rbegin(), data.rend());
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

template <bool Smudge> GRID<Smudge> GRID<Smudge>::rot90() const {
   GRID<Smudge> g;
   g.cols = data.size();
   g.data.resize(cols);
   for (BITS fromrow = 0; fromrow < data.size(); ++fromrow)
      for (BITS fromcol = 0; fromcol < cols; ++fromcol) {
         if (data[fromrow] & (1 << fromcol))
            g.data[fromcol] |= (1 << (g.cols - 1 - fromrow));
   }
   return g;
}

template <bool Smudge> using GRIDS = std::vector<GRID<Smudge>>;
template <bool Smudge> GRIDS<Smudge> parse(std::istream &is) {
   GRIDS<Smudge> result;
   GRID<Smudge> cur;
   for (int lineNo = 1;; ++lineNo) {
      std::string line;;
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

template <bool Smudge> std::ostream &operator <<(std::ostream &os, const GRID<Smudge> &grid) {
   for (auto datum : grid.data) {
      for (int i = 0; i < grid.cols; ++i)
         os << (datum & (1 << i )? '#':'.');
      os << "\n";
   }
   return os << "\n";
}

template <bool Smudge> unsigned long solve(std::istream &is, std::ostream &os) {
   auto day = parse<Smudge>(is);
   unsigned long sum = 0;
   for (auto &orig : day)
      sum += orig.palindromeAt();
   return sum;
}

void part1(std::istream &is, std::ostream &os) {
   os << "part1: " <<  solve<false>(is, os) << "\n";
}

void part2(std::istream &is, std::ostream &os) {
   os << "part2: " <<  solve<true>(is, os) << "\n";
}
