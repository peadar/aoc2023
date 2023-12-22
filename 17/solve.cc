#include <bits/stdc++.h>

namespace {
struct Location {
   int row = 0, col = 0;
   Location operator + ( const Location &rhs) { return { row + rhs.row, col + rhs.col } ; }
   auto operator <=> (const Location &rhs) const = default;
};

auto constexpr INFINITE = std::numeric_limits<unsigned long>::max();

enum Cardinal : uint8_t {
   NORTH, WEST, SOUTH, EAST
};

const Location cardinal[] {
   { -1, 0 },
   { 0, -1 },
   { 1, 0 },
   { 0, 1 },
};

struct Node {
   Location location;
   Cardinal direction;
   uint8_t remaining;
   long distance;
   Node(const Location &location_, Cardinal direction_, uint8_t remaining_)
      : location(location_)
      , direction(direction_)
      , remaining(remaining_)
      , distance(INFINITE)
   { }
};

std::ostream &operator <<(std::ostream &os, const Location &l) {
   return os << "{ " << l.row << ", " << l.col << " }";
}

struct Grid {
   std::vector<std::vector<uint8_t>> costs;
   std::vector<Node *> allNodes;
   Grid(std::istream &);
   void dijkstra();
};

Grid::Grid(std::istream &is) {
   for (std::string line; std::getline(is, line) && is; ) {
      costs.emplace_back();
      for (char c: line)
         costs.back().push_back(uint8_t(c - '0'));
   }
}

void Grid::dijkstra() {

   struct LTNode {
      Grid &g;
      bool operator()(const Node *l, const Node *r) {
         return g.costs[r->location.row][r->location.col] < g.costs[l->location.row][l->location.col];
      };
      LTNode(Grid &g_) : g(g_) {}
   };
   using Q = std::priority_queue<Node *, std::vector<Node *>, LTNode>;
   LTNode compare(*this);
   Q q(compare);
   auto start = new Node({0, 0}, EAST, 2);
   start->distance = 0;
   q.push(start);
}

}

void part1(std::istream &is, std::ostream &os) {
   Grid g{is};
   long tot = 0;
   g.dijkstra();
   os << "part 1: " << g.end->cost << "\n";
}

void part2(std::istream &, std::ostream &) {
}
