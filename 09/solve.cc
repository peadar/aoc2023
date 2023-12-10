#include "../aoc.h"
#include <iterator>
#include <algorithm>
#include <sstream>
#include <unistd.h>
#include <optional>

std::optional<std::vector<long>>
nextline(std::istream &is) {
   std::string line;
   std::getline(is, line);
   if (line == "")
      return std::nullopt;
   std::istringstream lis(line);
   std::vector<long> data;
   std::copy(
         std::istream_iterator<long>(lis),
         std::istream_iterator<long>(),
         std::back_inserter(data));
   return data;
}

void
part1(std::istream &is, std::ostream &os) {
   long total = 0;
   for (std::optional<std::vector<long>> maybedata = nextline(is); maybedata; maybedata = nextline(is)) {
      auto data = *maybedata;
      long exval = 0;
      for (int i = data.size() - 1; i > 0; --i) {
         long old = data[i];
         exval += old;
         bool allzeroes = true;
         for (int j = i - 1; j >= 0; --j) {
            long neu = old - data[j];
            if (neu != 0)
               allzeroes = false;
            old = data[j];
            data[j] = neu;
         }
         if (allzeroes)
            break;
      }
      total += exval;
   }
   os << "part1: " << total << "\n";
}

void
part2(std::istream &is, std::ostream &os) {
   long total = 0;
   for (std::optional<std::vector<long>> maybedata = nextline(is); maybedata; maybedata = nextline(is)) {
      size_t i;
      auto data = *maybedata;
      for (i = 0; i < data.size() - 1; ++i) {
         long old = data[i];
         bool allzeroes = true;
         for (size_t j = i + 1; j < data.size(); ++j) {
            auto neu = data[j] - old;
            if (neu != 0)
               allzeroes = false;
            old = data[j];
            data[j] = neu;
         }
         if (allzeroes)
            break;
      }
      long val = 0;
      for (; i < data.size(); --i) {
         val = data[i] - val;
      }
      total += val;
   }
   os << "part2: " << total << "\n";
}

