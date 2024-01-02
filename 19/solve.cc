#include "../aoc.h"

namespace {
struct Conditional {
   uint8_t variable;
   char op;
   int value;
   std::string target;
   Conditional(uint8_t variable, char op, int value, std::string target)
      : variable(variable), op(op), value(value), target(target) { }
};

using Workflow = std::vector<Conditional>;
using namespace std::literals;

struct MinMax { int min; int max; };
using Part = std::array<MinMax, 4>;
using Parts = std::vector<Part>;

struct Data {
   std::map<std::string, Workflow> workflows;
   Parts parts;
   Data(std::istream &is);
   template <typename Contribute> long solve(const std::string &label, Part part, Contribute contribute);
};

Data::Data(std::istream &is) {
   for (std::string sline; std::getline(is, sline) && sline != ""; ) {
      std::string_view line = sline;
      std::string label;
      std::tie(label, line) = aoc::token(line, "{"sv);
      Workflow &wf = workflows[label];

      while (line != "") {
         std::string target;
         char ch = line[0];
         char op = line[1];
         int val;
         if (op == '<' || op == '>') {
            line = line.substr(2);
            std::string_view num;
            std::tie(num, line) = aoc::token(line, ":"sv);
            val = strtol(num.data(), 0, 0);
            std::tie(target, line) = aoc::token(line, ","sv);
         } else {
            ch = '_';
            op = 'T';
            val = 0; // unused.
            std::tie(target, line) = aoc::token(line, "}"sv);
         }
         static const std::map<char, int> idxes {
            { 'x', 0 }, { 'm', 1 }, { 'a', 2 }, { 's', 3 }, { '_', 255 },
         };
         wf.emplace_back(idxes.at(ch), op, val, target);
      }
   }

   for (std::string sline; std::getline(is, sline) && sline != ""; ) {
      sline = sline.substr(1);
      std::string_view line = sline;
      auto varval = [] (std::string_view line, std::string_view sep) {
         std::string_view var, val;
         std::tie(var, line) = aoc::token(line, "="sv);
         std::tie(val, line) = aoc::token(line, sep);
         return std::tuple(var, strtol(val.data(), 0, 0), line);
      };
      Part p;
      for (int i = 0; i < 4; ++i) {
         std::string_view var, val;
         std::tie(var, p[i].min, line) = varval(line, i == 3 ? "}" : ",");
         p[i].max = p[i].min;
      }
      parts.push_back(p);
   }
}

template <typename Contribute> long Data::solve(const std::string &label, Part part, Contribute contribute) {
   if (label == "R")
      return 0;
   if (label == "A")
      return contribute(part);
   Workflow &wf = workflows[label];
   long tot = 0;
   for (auto &cond : wf) {
      Part copy = part;
      auto &range = part[cond.variable];
      auto &copyrange = copy[cond.variable];
      switch (cond.op) {
         case '<':
            copyrange.max = std::min(range.max, cond.value - 1);
            range.min = std::max(range.min, cond.value);
            break;
         case '>':
            copyrange.min = std::max(copyrange.min, cond.value + 1);
            range.max = std::min(range.max, cond.value);
            break;
         case 'T':
            break;
         default:
            abort();
      }
      if (cond.op == 'T' || copyrange.max >= copyrange.min)
         tot += solve(cond.target, copy, contribute);
      if (cond.op == 'T' || range.max < range.min)
         break;
   }
   return tot;
}

void part1(std::istream &is, std::ostream &out) {
   Data data(is);
   long tot = 0;
   for (const auto &part : data.parts)
      tot += data.solve("in", part,
            [](const Part &part) {
            return std::accumulate( part.begin(), part.end(), 0L,
                  [](long v, const MinMax &p) { return v + p.max; }); });
   out << tot;
}

void part2(std::istream &is, std::ostream &os) {
   Data data(is);
   Part p;
   for (int i = 0; i < 4; ++i)
      p[i] = { 1,4000 };
   os << data.solve("in", p,
         [](const Part &part) {
         return std::accumulate(part.begin(), part.end(), 1L,
               [](long v, const MinMax &p) { return v * (p.max - p.min + 1); });
         });
}

aoc::Case part1_("part1", part1);
aoc::Case part2_("part2", part2);
}
