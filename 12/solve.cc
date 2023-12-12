#include "../aoc.h"
#include <vector>
#include <sstream>
#include <iterator>
#include <cassert>

struct Scenario {
   using Pattern = std::string;
   Pattern pattern;
   using Constraints = std::vector<int>;
   Constraints constraints;
};

struct Matcher {
   const Scenario &scenario;
   int counter = -1;
   Scenario::Pattern::const_iterator curChar;
   Scenario::Constraints::const_iterator curConstraint;
   int accept(char c);
   Matcher(const Scenario &s_) : scenario(s_), counter(-1), curChar(s_.pattern.cbegin()), curConstraint(s_.constraints.cbegin()) {}
   Matcher(const Matcher &) = default;
};

int
Matcher::accept(char c) {
   if (curChar == scenario.pattern.cend()) {
      // If we've hit the end of the string, we should have consumed all the constraints.
      return curConstraint == scenario.constraints.cend() && counter <= 0;
   }

   switch (c) {
      case '.':
         if (counter != -1) {
            if (counter != 0)
               return 0;
            counter = -1;
         }
         return accept(*++curChar);

      case '#':
         if (counter == 0)
            return 0; // too many in a row.
         if (counter == -1) {
            // can start current counter.
            if (curConstraint == scenario.constraints.cend())
               return 0;
            counter = *curConstraint++; // start consuming the current constraint.
         }
         counter--;
         return accept(*++curChar);

      case '?': {
         auto copy = *this;
         return copy.accept('.') + accept('#');
      }
      default:
         abort();
   }
}

using Day = std::vector<Scenario>;

template <bool multiply>
Day parse(std::istream &is) {
   Day day;
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

template <typename T> std::ostream &operator << (std::ostream &os, const std::vector<T> &container) {
   const char *sep = "";
   for (const auto &a : container) {
      os << sep << a;
      sep = ", ";
   }
   return os;
}

template <bool multiply>
long solve(std::istream &is, std::ostream &os) {
   auto day = parse<multiply>(is);
   long tot = 0;
   for (auto &s : day) {
      Matcher acc(s);
      os << s.pattern << "\n";
      tot += acc.accept(s.pattern[0]);
      os.flush();
   }
   return tot;
}

void part1(std::istream &is, std::ostream &os) {
   std::cout << "part 1 " << solve<false>(is, os) << "\n";
}

void part2(std::istream &is, std::ostream &os) {
   std::cout << "part 1 " << solve<true>(is, os) << "\n";
}
