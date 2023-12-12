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
   std::string path;
   int counter = -1;
   Scenario::Pattern::const_iterator curChar;
   Scenario::Constraints::const_iterator curConstraint;
   int accept(char c);
   Matcher(const Scenario &s_) : scenario(s_), counter(-1), curChar(s_.pattern.cbegin()), curConstraint(s_.constraints.cbegin()) {}
   Matcher(const Matcher &) = default;
};

int
Matcher::accept(char c) {
   if (c != '?')
      path += c;
   if (curChar == scenario.pattern.cend()) {
      // If we've hit the end of the string, we should have consumed all the constraints.
      if (curConstraint == scenario.constraints.cend() && counter <= 0 ) {
         std::cout << path << "\n";
         return 1;
      }
      return 0;
   }

   switch (c) {
      case '.':
         if (counter != -1) {
            if (counter == 0)
               counter = -1;
            else
               return 0;
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
         assert(counter > 0);
         counter--;
         return accept(*++curChar);

      case '?': {
         auto copy = *this;
         return accept('.') + copy.accept('#');
      }
      default:
         abort();
   }
}

using Day = std::vector<Scenario>;

Day parse(std::istream &is) {
   Day day;
   for (;;) {
      Scenario s;
      getline(is, s.pattern, ' ');
      if (s.pattern == "")
         break;
      std::string tmp;
      for (getline(is, tmp); tmp != ""; ) {
         auto [istr, rest] = aoc::token(tmp, ",");
         s.constraints.push_back(std::stoi(istr, nullptr, 0));
         tmp = rest;

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

void part1(std::istream &is, std::ostream &os) {
   auto day = parse(is);
   long tot = 0;
   for (auto &s : day) {
      Matcher acc(s);
      std::cout << s.pattern <<  "  " << s.constraints << " = \n";
      long val = acc.accept(s.pattern[0]);
      std::cout << val << "\n\n";
      tot += val;
   }
   os << tot << "\n";
}

void part2(std::istream &is, std::ostream &os) {
   auto day = parse(is);
}
