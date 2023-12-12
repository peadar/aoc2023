#include "../aoc.h"
#include <vector>
#include <iterator>
#include <cassert>
#include <unordered_map>
#include <optional>
namespace {
struct Scenario {
   using Pattern = std::string;
   Pattern pattern;
   using Constraints = std::vector<int>;
   Constraints constraints;
};

struct State {
   unsigned char curChar;
   unsigned char curConstraint;

   // the remaining number of acceptable contigous damaged springs from here.
   // "-1" indicates we are not in the middle of a span
   signed char damagedSpan;
   auto operator <=> (const State &) const = default;
};

}
template <> struct std::hash<State> {
   std::size_t operator() (const State &s) const {
      // If these hold, the hash is unique.
      assert(s.curChar < 128);
      assert(s.curConstraint < 32);
      assert(s.damagedSpan + 1 < 32);
      assert(s.damagedSpan + 1 >= 0);
      return size_t(s.curChar) | size_t(s.curConstraint)  << 7 | size_t(s.damagedSpan + 1) << 12;
   }
};

namespace {

class Matcher {
public:
   using Cache = std::unordered_map<State, std::optional<long>>;
   long accept() const noexcept { return accept({0, 0, -1}, scenario.pattern[0]); }
   Matcher(Cache &cache_, const Scenario &s_)
      : scenario(s_)
      , allStates(cache_)
   { }
   Matcher(const Matcher &) = delete;

private:
   const Scenario &scenario;
   Cache &allStates;
   long acceptNocache(const State &curstate, char c) const noexcept;
   long accept(const State &state, char c) const noexcept;
};

long Matcher::accept(const State &curstate, char c) const noexcept {
   auto &cache = allStates[curstate];
   if (cache)
      return *cache;
   auto rv = acceptNocache(curstate, c);
   cache = rv;
   return rv;
}

long Matcher::acceptNocache(const State &curstate, char c) const noexcept {
   if (curstate.curChar == scenario.pattern.size())
      // If we've hit the end of the string, we should have consumed all the constraints.
      return ( curstate.curConstraint == scenario.constraints.size() && curstate.damagedSpan <= 0) ? 1 : 0;

   switch (c) {
      case '.': {
         long damagedSpan = curstate.damagedSpan;
         if (damagedSpan != -1) {
            if (damagedSpan != 0)
               return 0;
            damagedSpan = -1;
         }
         State next = curstate;
         next.damagedSpan = damagedSpan;
         return accept(next, scenario.pattern[++next.curChar]);
                }
      case '#': {
         if (curstate.damagedSpan == 0 || (curstate.damagedSpan == -1 && curstate.curConstraint == scenario.constraints.size())) {
            return 0; // too many in a row, or we've run out of constraints with a new '#'
         }
         State next = curstate;
         if (curstate.damagedSpan == -1) // start consuming the current constraint.
            next.damagedSpan = scenario.constraints[next.curConstraint++];
         next.damagedSpan--;
         return accept(next,  scenario.pattern[++next.curChar]);
                }
      case '?':
         return acceptNocache(curstate, '.') + acceptNocache(curstate, '#');

      default:
         __builtin_unreachable();
   }
}

using Scenarios = std::vector<Scenario>;

template <bool multiply>
Scenarios parse(std::istream &is) {
   Scenarios day;
   for (;;) {
      std::string pattern;
      getline(is, pattern, ' ');
      if (pattern == "")
         break;
      Scenario s;
      s.pattern = pattern;
      if (multiply) {
         for (int i = 0; i < 4; ++i)
            s.pattern += "?" + pattern;
      }
      std::string tmp;
      std::vector<int> constraints;
      for (getline(is, tmp); tmp != ""; ) {
         auto [istr, rest] = aoc::token(tmp, ",");
         constraints.push_back(std::stoi(istr, nullptr, 0));
         tmp = rest;
      }
      for (int i = 0; i < (multiply ? 5 : 1); ++i) {
         std::copy(constraints.begin(), constraints.end(), std::back_inserter(s.constraints));
      }
      day.push_back(std::move(s));
   }
   return day;
}

template <bool multiply>
long solve(std::istream &is) {
   auto day = parse<multiply>(is);
   long tot = 0;
   Matcher::Cache cache(4000);
   for (auto &s : day) {
      cache.clear();
      Matcher acc(cache, s);
      tot += acc.accept();
   }
   return tot;
}
}
void part1(std::istream &is, std::ostream &os) {
   os << "part 1: " << solve<false>(is) << "\n";
}

void part2(std::istream &is, std::ostream &os) {
   os << "part 2: " << solve<true>(is) << "\n";
}
