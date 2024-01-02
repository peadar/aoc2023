#include "../aoc.h"

namespace {

struct Symlink {
   constexpr static size_t END = 1 << 10;
   size_t workflow:11;
   size_t condition:11;
};

struct Symbol {
   size_t idx = -1;
   Symlink head = { Symlink::END, Symlink::END };
};

struct Conditional {
   uint8_t variable;
   char op;
   int value;
   union {
      size_t target;
      Symlink link;
   } u;
   Conditional(uint8_t var, char op, int val, Symlink link)
      : variable(var), op(op), value(val) {
      u.link = link;
   }
};

struct Workflow {
   std::vector<Conditional> conditions;
   bool visited = false;
};
using namespace std::literals;

struct Part {
   std::array<int, 4> min;
   std::array<int, 4> max;
};
using Parts = std::vector<Part>;

struct Data {
   std::vector<Workflow> workflows;
   size_t in, R, A; // indexes of the start and terminal states.
   Parts parts;
   Data(std::istream &is);
   size_t doPart1(size_t label, const Part &part) const;
   template <typename Contribute> long doPart2(size_t label, Part part, Contribute contribute);
};

Data::Data(std::istream &is) {
   std::map<std::string, Symbol> links;
   for (std::string sline; std::getline(is, sline) && sline != ""; ) {
      std::string_view line = sline;
      std::string label;
      std::tie(label, line) = aoc::token(line, "{"sv);
      Workflow wf;
      auto &symbol = links[label];
      symbol.idx = workflows.size();

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
            val = 0;
            std::tie(target, line) = aoc::token(line, "}"sv);
         }
         static const std::map<char, int> idxes {
            { 'x', 0 }, { 'm', 1 }, { 'a', 2 }, { 's', 3 }, { '_', 255 },
         };
         auto &targetSym = links[target];
         auto oldlink = targetSym.head;
         targetSym.head = { workflows.size(), wf.conditions.size() };
         wf.conditions.emplace_back(idxes.at(ch), op, val, oldlink);
      }
      workflows.push_back(wf);
   }

   in = links["in"].idx;
   A = links["A"].idx = workflows.size();
   R = links["R"].idx = workflows.size() + 1;

   // we should now have resolved all indexes. We can go back and fix up our links.
   for (const auto &sym : links) {
      for (auto link = sym.second.head; link.workflow != Symlink::END; ) {
         auto &cond = workflows[link.workflow].conditions[link.condition];
         link = cond.u.link;
         cond.u.target = sym.second.idx;
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
         std::tie(var, p.min[i], line) = varval(line, i == 3 ? "}" : ",");
         p.max[i] = p.min[i];
      }
      parts.push_back(p);
   }
}

template <typename Contribute> long Data::doPart2(size_t label, Part part, Contribute contribute) {
   if (label == R)
      return 0;
   if (label == A)
      return contribute(part);

   Workflow &wf = workflows[label];
   long tot = 0;
   for (auto &cond : wf.conditions) {
      Part copy = part;
      switch (cond.op) {
         case '<':
            copy.max[cond.variable] = std::min(copy.max[cond.variable], cond.value - 1);
            part.min[cond.variable] = std::max(part.min[cond.variable], cond.value);
            break;
         case '>':
            copy.min[cond.variable] = std::max(copy.min[cond.variable], cond.value + 1);
            part.max[cond.variable] = std::min(part.max[cond.variable], cond.value);
            break;
         case 'T':
            break;
         default:
            abort();
      }
      if (cond.op == 'T' || copy.max[cond.variable] >= copy.min[cond.variable])
         tot += doPart2(cond.u.target, copy, contribute);

      if (cond.op == 'T' || part.max[cond.variable] < part.min[cond.variable])
         break;
   }
   return tot;
}

long contributeSize(const Part &part) {
   long tot = 1;
   for (size_t i = 0; i < 4; ++i)
      tot *= part.max[i] - part.min[i] + 1;
   return tot;
}

long contributePosition(const Part &part) {
   long tot = 0;
   for (size_t i = 0; i < 4; ++i)
      tot += part.max[i];
   return tot;
}

void part1(std::istream &is, std::ostream &out) {
   Data data(is);
   long tot = 0;
   for (const auto &part : data.parts)
      tot += data.doPart2(data.in, part, contributePosition);
   out << tot;
}

void part2(std::istream &is, std::ostream &os) {
   Data data(is);
   Part p;
   for (int i = 0; i < 4; ++i) {
      p.max[i] = 4000;
      p.min[i] = 1;
   }
   os << data.doPart2(data.in, p, contributeSize);
}

aoc::Case part1_("part1", part1);
aoc::Case part2_("part2", part2);
aoc::Case parse_("parse", [](std::istream &is, std::ostream &) { Data d(is); } );

}
