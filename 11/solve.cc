#include "../aoc.h"
#include <vector>
#include <string>
namespace {
struct Row {
   long idx;
   std::vector<long> cols;
   Row(long rownum) : idx(rownum) {}
};
std::vector<Row> parse(std::istream &input, long gap) {
   std::vector<Row> data;
   long rownum = 0;
   std::vector<bool> allcols;
   for (std::string text; std::getline(input, text); ) {
      if (allcols.size() < text.size())
         allcols.resize(text.size());
      Row row{rownum};
      for (size_t i = 0; i < text.size(); ++i)
         if (text[i] == '#') {
            row.cols.push_back(i);
            allcols[i] = true;
         }
      rownum += row.cols.size() == 0 ? gap : 1;
      data.push_back(std::move(row));
   }
   for (long prev = allcols.size(), cur = prev - 1; cur >= 0; --cur) {
      if (!allcols[cur])
         continue;
      size_t emptycols = prev - cur - 1;
      prev = cur;
      for (auto & row : data)
         for (auto & col : row.cols)
            if (col > cur)
               col += (gap - 1) * emptycols;
   }
   return data;
}
}
long solve(std::istream &is, long gap) {
   auto input = parse(is, gap);
   long output = 0;
   for (size_t outrow = 0; outrow < input.size(); ++outrow)
      for (size_t outcol = 0;  outcol < input[outrow].cols.size(); ++outcol)
         for (size_t inrow = outrow; inrow < input.size(); ++inrow)
            for (size_t incol = outrow == inrow ? outcol + 1 : 0; incol < input[inrow].cols.size(); ++incol)
               output += input[inrow].idx - input[outrow].idx + std::abs(input[outrow].cols[outcol] - input[inrow].cols[incol]);
   return output;
}

aoc::Case part1("part1", [](std::istream &is, std::ostream &os) {
   os << solve(is, 2); });

aoc::Case part2("part2", [](std::istream &is, std::ostream &os) {
   os << solve(is, 1000000); });
