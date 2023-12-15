#include "../aoc.h"
#include <vector>
#include <algorithm>
#include <string_view>
#include <climits>
static_assert(CHAR_BIT == 8, "I am assuming unsigned chars do arithmetic mod 256");

struct Lens {
   std::string name;
   int focalLength;
};

struct Box {
   std::vector<Lens> lenses;
   void add(const std::string &name, int focalLength);
   void remove(const std::string &name);
};

void
Box::add(const std::string &label, int focalLength) {
   auto existing = std::find_if(lenses.begin(), lenses.end(),
         [&label](const Lens &lens) { return lens.name == label; } );
   if (existing != lenses.end())
      existing->focalLength = focalLength;
   else
      lenses.emplace_back(label, focalLength);
}

void
Box::remove(const std::string &label) {
   auto existing = std::find_if(lenses.begin(), lenses.end(),
         [&label](const Lens &lens) { return lens.name == label; } );
   if (existing != lenses.end())
      lenses.erase(existing);
}

size_t hashLabel(const std::string &label) {
   unsigned char cur = 0;
   for (auto c : label)
      cur = (cur + c) * 17;
   return cur;
}

using Boxes = std::array<Box, 256>;

long power(const Boxes &boxes) {
   unsigned long result = 0;
   for (size_t boxno = 0; boxno < 256; ++boxno) {
      auto &lenses = boxes[boxno].lenses;
      for (size_t lensno = 0; lensno < lenses.size(); ++lensno)
         result += (boxno+1) * (lensno + 1 ) * lenses[lensno].focalLength;
   }
   return result;
}

void part1(std::istream &is, std::ostream &os) {
   int c;
   long total = 0;
   unsigned char curhash = 0;
   while((c = is.get()) != -1) {
      switch (c) {
         case ',':
         case '\n':
            total += curhash;
            curhash = 0;
            break;
         default:
            curhash = (curhash + c) * 17;
            break;
      }
   }
   os << total << "\n";
}

void part2(std::istream &is, std::ostream &os) {
   char op;
   std::string label;
   size_t focalLength;
   Boxes boxes;
   for (int c; (c = is.get()) != -1; ) {
      switch (c) {
         case '=': case '-':
            op = c;
            break;
         case '0'...'9':
            focalLength = focalLength * 10 + c - '0';
            break;
         default:
            label += c;
            break;
         case ',': case '\n': {
            Box &box = boxes[hashLabel(label)];
            if (op == '=')
               box.add(label, focalLength);
            else
               box.remove(label);
            label = "";
            focalLength = 0;
            break;
                    }
      }
   }
   os << power(boxes) << "\n";
}
