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
   ulong srcend;
   ulong destoff;
   bool operator < (const Projection &rhs) const { return srcend < rhs.srcend; }
};

struct Mapping {
   std::string dest;
   std::vector<Projection> projections;
   std::vector<Projection>::const_iterator find(ulong seed) {
      return std::upper_bound(
               projections.begin(), projections.end(), seed,
               [](ulong seed, const Projection &projection) { return seed < projection.srcend; });
   }
};

struct Almanac {
   std::vector<ulong> seeds;
   std::map<std::string, Mapping> maps;

   ulong chase(ulong seed) {
      for (std::string cur = "seed"; cur != "location"; ) {
         auto &mapping = maps[cur];
         cur = mapping.dest;
         auto it = mapping.find(seed);
         if (it != mapping.projections.end() && it->source <= seed)
            seed += it->destoff;
      }
      return seed;
   }

   RangeSet chase(RangeSet ranges) {
      RangeSet outRanges;
      for (std::string cur = "seed"; cur != "location"; ranges = std::move(outRanges) ) {
         auto &mapping = maps[cur];
         cur = mapping.dest;
         for (auto range : ranges) {
            for (auto it = mapping.find(range.first); range.first < range.second; ++it) {
               if (it == mapping.projections.end() || range.second < it->source) {
                  outRanges.push_back(range);
                  break;
               }
               if (range.first < it->source) {
                  outRanges.emplace_back(range.first, it->source);
                  range.first = it->source;
               }
               auto end = std::min(range.second, it->srcend);
               outRanges.emplace_back(range.first + it->destoff, end + it->destoff);
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
         while (isdigit(is.peek())) {
            getline(is, s, ' ');
            ulong to = stoul(s, 0, 0);
            getline(is, s, ' ');
            ulong from = stoul(s, 0, 0);
            getline(is, s);
            ulong count = stoul(s, 0, 0);
            map.projections.emplace_back( from, from + count, to - from );
         }
         std::sort(map.projections.begin(), map.projections.end());
         std::getline(is, s); // empty line.
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
