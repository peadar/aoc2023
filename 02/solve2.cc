#include <unistd.h>
#include <cassert>
#include <sstream>
#include <algorithm>
#include "../aoc.h"

enum Color { RED, GREEN, BLUE, COLORS };

struct Draw {
   int colors[COLORS];
   bool possibleWith(const Draw &max) const {
      for (int i = 0; i < COLORS; ++i)
         if  (max.colors[i] < colors[i])
            return false;
      return true;
   }
};

struct Game {
   int number;
   std::vector<Draw> draws;
   Game(const std::string &in);
   bool possibleWith(const Draw &max)  const {
      return std::all_of(draws.begin(), draws.end(),
            [&max](const Draw &d) { return d.possibleWith(max); });
   }
};

Game::Game(const std::string &in) {
   std::string game, colon;
   std::istringstream is(in);
   is >> game >> number >> colon;
   for (;;) {
      draws.push_back({});
      auto &draw = draws.back();
      for  (;;) {
         int count;
         std::string colorname;
         is >> count >> colorname;
         Color color;
         if (colorname.starts_with("blue"))
            color = BLUE;
         else if (colorname.starts_with("green"))
            color = GREEN;
         else {
            assert(colorname.starts_with("red"));
            color = RED;
         }
         draw.colors[color] = count;
         char last = colorname[colorname.size() - 1];
         if (last == ';')
            break;
         else if (last == ',')
            ;
         else
            return;
      }
   }
}

std::vector<Game>
parseGames(std::istream &is) {
   std::vector<Game> games;
   games.reserve(100);
   for (;;) {
      std::string line;
      std::getline(is, line);
      if (!is.good())
         return games;
      games.emplace_back(line);
   }
}

void
part1(std::istream &is, std::ostream &os) {
   auto games = parseGames(is);
   Draw max { .colors { 12, 13, 14 } };
   int total = 0;
   for (auto &g : games)
      if (g.possibleWith(max))
         total += g.number;
   os << total << "\n";
}

void
part2(std::istream &is, std::ostream &os) {
   auto games = parseGames(is);
   unsigned long int sums = 0;
   for (auto &g : games) {
      Draw min{};
      for (auto &draw : g.draws)
         for (int i = 0; i < COLORS; ++i)
            min.colors[i] = std::max(min.colors[i], draw.colors[i]);
      int power = 1;
      for (int i = 0; i < COLORS; ++i)
         power *= min.colors[i];
      sums += power;
   }
   os << sums << "\n";
}
