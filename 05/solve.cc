#include "../aoc.h"
#include <map>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <limits>

using ulong = unsigned long;
using Range = std::pair<ulong, ulong>;
using RangeSet = std::vector<Range>;

struct Projection {
   ulong source;
   ulong destoff;
};

struct Mapping {
   std::string dest;
   std::map<ulong, Projection> projections;
};

struct Almanac {
   std::vector<ulong> seeds;
   std::map<std::string, Mapping> maps;

   ulong chase(ulong seed) {
      for (std::string cur = "seed"; cur != "location"; ) {
         auto &mapping = maps[cur];
         cur = mapping.dest;
         auto it = mapping.projections.upper_bound(seed);
         if (it != mapping.projections.end() && it->second.source <= seed)
            seed += it->second.destoff;
      }
      return seed;
   }

   RangeSet chase(RangeSet ranges) {
      RangeSet outRanges;
      for (std::string cur = "seed"; cur != "location"; ranges = std::move(outRanges) ) {
         auto &mapping = maps[cur];
         cur = mapping.dest;
         for (auto range : ranges) {
            for (auto it = mapping.projections.upper_bound(range.first); range.first < range.second; ++it) {
               if (it == mapping.projections.end() || range.second < it->second.source) {
                  outRanges.push_back(range);
                  break;
               }
               if (range.first < it->second.source) {
                  outRanges.emplace_back(range.first, it->second.source);
                  range.first = it->second.source;
               }
               auto end = std::min(range.second, it->first);
               outRanges.emplace_back(range.first + it->second.destoff, end + it->second.destoff);
               range.first = end;
            }
         }
      }
      return ranges;
   }

   Almanac(std::istream &is) {
      std::string line;
      std::getline(is, line, ' '); // skip 'seeds: '
      std::getline(is, line);
      std::istringstream seedStream(line);
      std::copy(std::istream_iterator<ulong>(seedStream), std::istream_iterator<ulong>(), std::back_inserter(seeds));
      std::getline(is, line); // skip blank line after seeds.
      for (std::string s; std::getline(is, s, '-');) {
         auto &map = maps[s];
         std::getline(is, s, '-'); // "to"
         std::getline(is, map.dest, ' ');
         std::getline(is, s); // "map:"

         while (std::getline(is, line) && line != "") {
            std::istringstream values(line);
            ulong to, from, count;
            values >> to >> from >> count;
            map.projections[from + count] = { from, to - from };
         }
      }
   }
};

void part1(std::istream &in, std::ostream &out) {
   Almanac almanac(in);
   ulong lowest = std::numeric_limits<ulong>::max();
   for (auto seed : almanac.seeds)
      lowest = std::min(lowest, almanac.chase(seed));
   out << lowest << "\n";
}

void part2(std::istream &in, std::ostream &out) {
   Almanac almanac(in);
   RangeSet ranges;
   for (size_t i = 0; i < almanac.seeds.size(); i += 2)
      ranges.emplace_back(almanac.seeds[i], almanac.seeds[i]+almanac.seeds[i+1]);
   ranges = almanac.chase(ranges);
   out << std::min_element(ranges.begin(), ranges.end())->first << "\n";
}
