#include "../aoc.h"

using namespace std::literals;

using Target = std::string;

struct Conditional {
   uint8_t variable;
   char op;
   int value;
   Target target;
};
using Workflow = std::vector<Conditional>;

using Workflows = std::vector<Workflow>;

using Part = std::array<int, 4>;

std::ostream &operator <<(std::ostream &os, const Part &part) {
   return os
      << "{ x=" << part[0]
      << ", m=" << part[1]
      << ", a=" << part[2]
      << ", s=" << part[3]
      << " }";
}

using Parts = std::vector<Part>;

struct Data {
   std::map<Target, Workflow> workflows;
   Parts parts;
   Data(std::istream &is);
};

Data::Data(std::istream &is) {
   for (std::string sline; std::getline(is, sline) && sline != ""; ) {
      std::string_view line = sline;
      std::string_view label;
      std::tie(label, line) = aoc::token(line, "{"sv);
      Workflow &wf = workflows[std::string(label)];
      while (line != "") {
         std::string_view target;
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
            { 'x', 0 },
            { 'm', 1 },
            { 'a', 2 },
            { 's', 3 },
            { '_', 255 },
         };
         wf.emplace_back(idxes.at(ch), op, val, std::string(target));
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
         std::tie(var, p[i], line) = varval(line, i == 3 ? "}" : ",");
      }
      parts.push_back(p);
   }
}

void part1(std::istream &in, std::ostream &out) {
   Data data(in);

   long tot = 0;
   for (const auto &part : data.parts) {
      std::string label = "in";
      while (label != "A" && label != "R") {
         Workflow &wf = data.workflows[label];
         for (auto &cond : wf) {
            bool truth;
            switch (cond.op) {
               case '<': truth = part[cond.variable] < cond.value; break;
               case '>': truth = part[cond.variable] > cond.value; break;
               case 'T': truth = true; break;
               default: abort();
            }
            if (truth) {
               label = cond.target;
               break;
            }
         }
      }
      if (label == "A")
         tot += std::accumulate(part.begin(), part.end(), 0);
   }
   out << tot;
}
aoc::Case part1_("part1", part1);
