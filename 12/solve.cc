#include "../aoc.h"
#include <vector>
#include <iterator>
#include <map>
#include <optional>

struct Scenario {
   using Pattern = std::string;
   Pattern pattern;
   using Constraints = std::vector<int>;
   Constraints constraints;
};

struct State {
   Scenario::Pattern::const_iterator curChar;
   Scenario::Constraints::const_iterator curConstraint;
   char c;
   int counter;
   auto operator <=> (const State &) const = default;
};

struct Matcher {
   const Scenario &scenario;
   std::map<State, std::optional<long>> allStates;
   long accept() {
      State initial = State{ scenario.pattern.cbegin(), scenario.constraints.cbegin(), scenario.pattern[0], -1, };
      return accept(initial);
   }
   long accept(const State &state);
   Matcher(const Scenario &s_) : scenario(s_){}
   Matcher(const Matcher &) = default;
};

long Matcher::accept(const State &curstate) {
   auto &cache = allStates[curstate];
   if (cache)
      return *cache;
   if (curstate.curChar == scenario.pattern.cend()) {
      // If we've hit the end of the string, we should have consumed all the constraints.
      cache = ( curstate.curConstraint == scenario.constraints.cend() && curstate.counter <= 0) ? 1 : 0;
      return *cache;
   }
   switch (curstate.c) {

      case '.': {
         long counter = curstate.counter;
         if (counter != -1) {
            if (counter != 0) {
               cache = 0;
               return *cache;
            }
            counter = -1;
         }
         State next = curstate;
         next.counter = counter;
         next.c = *++next.curChar;
         cache = accept(next);
         return *cache;
                }

      case '#': {
         if (curstate.counter == 0 || curstate.counter == -1 && curstate.curConstraint == scenario.constraints.cend()) {
            cache = 0; // too many in a row, or we've run out of constraints with a new '#'
            return *cache;
         }

         State next = curstate;
         if (curstate.counter == -1) // start consuming the current constraint.
            next.counter = *next.curConstraint++;
         next.counter--;
         next.c = *++next.curChar;
         cache = accept(next);
         return *cache;
                }

      case '?': {
         State a = curstate;
         State b = curstate;
         a.c = '.';
         b.c = '#';
         cache = accept(a) + accept(b);
         return *cache;
                }
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
long solve(std::istream &is, std::ostream &os) {
   auto day = parse<multiply>(is);
   long tot = 0;
   for (auto &s : day) {
      Matcher acc(s);
      tot += acc.accept();
      os.flush();
      os << acc.allStates.size() << " cachesize\n";
   }
   return tot;
}

void part1(std::istream &is, std::ostream &os) {
   os << "part 1: " << solve<false>(is, os) << "\n";
}

void part2(std::istream &is, std::ostream &os) {
   os << "part 2: " << solve<true>(is, os) << "\n";
}
