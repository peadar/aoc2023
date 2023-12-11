#include "../aoc.h"
#include <vector>
#include <string>
#include <set>

namespace {
struct Row {
   long idx;
   std::vector<long> cols;
   Row(long rownum) : idx(rownum) {}
};

std::vector<Row> parse(std::istream &input, long gap) {
   std::vector<Row> data;
   long rownum = 0;
   std::set<long> allcols;
   for (;;) {
      std::string text;
      std::getline(input, text);
      if (text == "")
         break;

      Row row{rownum};
      for (size_t i = 0; i < text.size(); ++i) {
         if (text[i] == '#') {
            row.cols.push_back(i);
            allcols.insert(i);
         }
      }
      rownum += row.cols.size() == 0 ? gap : 1;
      data.push_back(std::move(row));
   }
   if (!allcols.empty()) {
      auto cur = allcols.rbegin();
      long prev = *cur + 1;

      while (cur != allcols.rend()) {
         long notempty = *cur++;
         long emptycols = prev - notempty - 1;
         prev = notempty;
         for (auto & row : data) {
            for (auto & col : row.cols) {
               if (col > notempty)
                  col += (gap - 1) * emptycols;
            }
         }
      }
   }
   return data;
}
}

long
solve(std::istream &is, std::ostream &os, long gap) {
   auto input = parse(is, gap);
   long output = 0;
   for (size_t outrow = 0; outrow < input.size(); ++outrow)
      for (size_t outcol = 0;  outcol < input[outrow].cols.size(); ++outcol)
         for (size_t inrow = outrow; inrow < input.size(); ++inrow)
            for (size_t incol = outrow == inrow ? outcol + 1 : 0; incol < input[inrow].cols.size(); ++incol)
               output += std::abs(input[outrow].idx - input[inrow].idx) + std::abs(input[outrow].cols[outcol] - input[inrow].cols[incol]);
   return output;
}

void
part1(std::istream &is, std::ostream &os) {
   os << "part1: " << solve(is, os, 2) << "\n";
}

void
part2(std::istream &is, std::ostream &os) {
   os << "part2: " << solve(is, os, 1000000) << "\n";
}
