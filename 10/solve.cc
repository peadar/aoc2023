#include "../aoc.h"
namespace {
enum Cardinal {
   INVALID = 0, NORTH = 1 << 0, SOUTH = 1 << 1, EAST = 1 << 2,
   WEST = 1 << 3, START = 1 << 4, MARK = 1 << 5, EMPTY = 1 << 6
};
using Cell = unsigned char;
struct Location {
   size_t row, col;
   constexpr Location operator + (Cardinal c) noexcept {
      switch (c) {
         case NORTH: return { row - 1, col };
         case SOUTH: return { row + 1, col };
         case EAST: return { row, col + 1 };
         case WEST: return { row, col - 1 };
         default: __builtin_unreachable();
      }
   }
};
struct Maze {
   std::vector<std::vector<Cell>> matrix;
   Cell &operator[](Location l) {
      return matrix[l.row][l.col];
   }
   bool contains(const Location &l) {
      return l.row < matrix.size() && l.col < matrix[0].size();
   }
};
constexpr Cardinal opposite(Cardinal c) noexcept {
   switch (c) {
      case NORTH: return SOUTH;
      case SOUTH: return NORTH;
      case EAST: return WEST;
      case WEST: return EAST;
      default: __builtin_unreachable();
   }
}
constexpr Cell makecell(char ch) noexcept {
   switch (ch) {
   case 'F': return EAST|SOUTH;
   case '7': return WEST|SOUTH;
   case 'L': return NORTH|EAST;
   case 'J': return NORTH|WEST;
   case '-': return EAST|WEST;
   case '|': return NORTH|SOUTH;
   case 'S': return START|MARK;
   case '.': return EMPTY;
   default: __builtin_unreachable();
   }
}
std::pair<Maze, Location> parse(std::istream &is) {
   std::pair<Maze, Location> result;
   std::vector<Cell> current;
   for (int c; (c = is.get()) != -1; ) {
      switch (c) {
         case '\n':
            result.first.matrix.push_back(std::move(current));
            current = {};
            break;
         case 'S':
            result.second.row = result.first.matrix.size();
            result.second.col = current.size();
            /* fallthrough */
         default:
            current.push_back(makecell(c));
            break;
      }
   }
   return result;
}
}
void parts( std::istream &is, std::ostream &os) {
   auto [maze, start] = parse(is);
   Cardinal dir;
   for (auto d : { NORTH, SOUTH, EAST, WEST } ) {
      Location candidate = start + d;
      if (maze.contains(candidate) && (maze[candidate] & opposite(d)))
         maze[start] |= dir = d;
   }
   int steps = 1;
   for (Location pos = start + dir; (maze[pos] & START) == 0; pos = pos + dir, steps++) {
      Cell &cell = maze[pos];
      dir = Cardinal(cell & (NORTH|SOUTH|EAST|WEST) & ~opposite(dir) );
      cell |= MARK;
   }
   int count = 0, inside = 0;
   for (auto &row : maze.matrix)
      for (auto dirs : row)
         if (dirs & MARK)
            inside ^= (dirs & (NORTH|SOUTH));
         else if (inside)
            ++count;
   os << "part1: " << steps/2 << "\n" << "part2: " << count << "\n";
}

aoc::Case both("both", parts);
