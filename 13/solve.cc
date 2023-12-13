#include "../aoc.h"
#include <vector>
#include <unistd.h>
#include <optional>
#include <cassert>
#include <numeric>

using BITS = uint32_t;

struct GRID {
   std::vector<BITS> data;
   size_t cols;
   GRID rot90() const noexcept;

   template <bool Smudge, typename It> std::optional<size_t> findPalindrome(It begin, It end) const noexcept {
      for (auto s = (end - begin)% 2 ? begin + 1 : begin; s < end - 1; s += 2) {
         int zeroes = 0, ones = 0;
         It p, e;
         for (p = s, e = end; p != e; ++p, --e)
            if (*p == e[-1])
               zeroes++;
            else if (__builtin_popcount(*p ^ e[-1]) == 1)
               ones++;
         if (zeroes == (end-s)/2 - Smudge && ones == Smudge)
            return p - begin;
      }
      return std::nullopt;
   }

   template <bool Smudge> std::optional<size_t> palindromeAtNR() const noexcept {
      auto p = findPalindrome<Smudge>(data.begin(), data.end());
      if (p)
         return p;
      p = findPalindrome<Smudge>(data.rbegin(), data.rend());
      if (p)
         return data.size() - *p;
      return std::nullopt;
   }

   template <bool Smudge> size_t palindromeAt() const noexcept {
      auto p = palindromeAtNR<Smudge>();
      if (p)
         return *p * 100;
      p = rot90().palindromeAtNR<Smudge>();
      assert(p);
      return *p;
   }
};

GRID GRID::rot90() const noexcept {
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

auto parse(std::istream &is) {
   std::vector<GRID> result;
   GRID cur;
   for (;;) {
      std::string line;
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

template <bool Smudge> unsigned long solve(std::istream &is) {
   auto day = parse(is);
   return std::accumulate(day.begin(), day.end(), 0L, [](long other, const GRID &g) -> long { return other + g.palindromeAt<Smudge>(); });
}

void part1(std::istream &is, std::ostream &os) { os << "part1: " <<  solve<false>(is) << "\n"; }
void part2(std::istream &is, std::ostream &os) { os << "part2: " <<  solve<true>(is) << "\n"; }
