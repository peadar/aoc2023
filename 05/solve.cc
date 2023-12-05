#include "../aoc.h"

#include <map>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <limits>
#include <cassert>
#include <unistd.h>

using ulong = unsigned long;
using Range = std::pair<ulong, ulong>;
using RangeSet = std::vector<Range>;

struct Projection {
   // these are the start of the source and destination.
   // The size of the range is implied by using the end of the source range as
   // the key to the map this is stored in.
   ulong source;
   ulong dest;
};

struct Mapping {
   std::string source;
   std::string dest;
   std::map<ulong, Projection> projections;
};

struct Almanac {
   std::vector<unsigned long> seeds;
   std::map<std::string, Mapping> maps;
   Almanac(std::istream &is);
   void parseSeeds(const std::string &);

   ulong chase(ulong seed) {
      for (std::string cur = "seed"; cur != "location"; ) {
         auto &mapping = maps[cur];
         cur = mapping.dest;
         auto it = mapping.projections.upper_bound(seed);
         if (it == mapping.projections.end()  || it->second.source > seed)
            continue;
         assert(seed < it->first);
         auto &mapent = it->second;
         seed -= mapent.source;
         seed += mapent.dest;
      }
      return seed;
   }

   RangeSet chase(RangeSet ranges) {
      RangeSet outRanges;
      for (std::string cur = "seed"; cur != "location"; ) {
         auto &mapping = maps[cur];
         cur = mapping.dest;
         for (auto range : ranges) {
            for (;range.first < range.second;) {
               auto it = mapping.projections.upper_bound(range.first);

               if (it == mapping.projections.end() || range.second < it->second.source) {
                  outRanges.push_back(range);
                  break;
               }

               // we'll offset the range by this much to project into the next type.
               auto diff = it->second.dest - it->second.source;

               // if there's a prefix in the range that isn't coered by the
               // projection, then add that first to the output as an identity
               // mapping.
               if (range.first < it->second.source) {
                  outRanges.push_back({range.first, it->second.source});
                  range.first = it->second.source;
               }
               Range subrange { range.first, std::min(range.second, it->first) };
               range.first = subrange.second;
               subrange.first += diff;
               subrange.second += diff;
               outRanges.push_back(subrange);
            }
         }
         ranges = std::move(outRanges);
      }
      return ranges;
   }
};

void Almanac::parseSeeds(const std::string &line) {
   std::istringstream is(line);
   std::string seedscolon;
   is >> seedscolon;
   assert(seedscolon == "seeds:");
   std::copy(
         std::istream_iterator<ulong>(is),
         std::istream_iterator<ulong>(),
         std::back_inserter(seeds));
}

Almanac::Almanac(std::istream &is) {
   std::string line;

   std::getline(is, line);
   parseSeeds(line);
   std::getline(is, line);

   for (;;) {
      std::string from, sep, to;
      {
         std::getline(is, line);
         std::istringstream ss(line);
         if (!std::getline(ss, from, '-'))
            break;
         std::getline(ss, sep, '-');
         std::getline(ss, to, ' ');
         assert(sep == "to");
      }

      {
         std::istringstream ss(line);
         std::string mapname, text;
         ss >> mapname >> text;
         assert(text == "map:");
      }

      auto &forward = maps[from];
      forward.source = from;
      forward.dest = to;

      for (;;) {
         if (!std::getline(is, line) || line == "")
            break;
         std::istringstream values(line);
         ulong to, from, count;
         values >> to >> from >> count;
         forward.projections[from + count] = { from, to };
      }
   }
}

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
   for (size_t i = 0; i < almanac.seeds.size(); i += 2) {
      ranges.emplace_back(almanac.seeds[i], almanac.seeds[i]+almanac.seeds[i+1]);
   }
   auto outRanges = almanac.chase(ranges);

   ulong lowest = std::numeric_limits<ulong>::max();
   for (auto i : outRanges)
      lowest = std::min(i.first, lowest);
   out << lowest << "\n";
}
