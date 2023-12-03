#include <iostream>
#include <vector>
#include <unistd.h>

struct Num {
   // Data we record about a number - the value itself, and the column
   // it starts and ends on.
   int value;
   int colStart;
   int colEnd;
};

struct Sym {
   // For a symbol, record the column its on, the number of adjacent numbers,
   // and the product of those adjacent numbers
   int col;
   int adjCount;
   int adjProduct;
   char symbol;
};

// Reach row is a sequence of symbols and numbers.
struct Row {
   std::vector<Sym> symbols;
   std::vector<Num> nums;
};

using Data = std::vector<Row>;

Data parse(std::istream &is) {
   Data data;
   int rowNum = 0;
   int col = 0, startcol, endcol;
   char c;
   int val = 0;
   data.emplace_back();
   for (bool done = false; !done;) {
      Row &row = data.back();
      switch (is.peek()) {
         case '.':
            is.get(); col++;
            continue;
         case '0'...'9':
            startcol = col;
            for (val = 0; isdigit(is.peek()); ) {
               val = val * 10 + is.get() - '0';
               col++;
            }
            endcol = col;
            row.nums.emplace_back( val, startcol, endcol );
            break;
         case '\n':
            is.get();
            col = 0;
            rowNum++;
            data.emplace_back();
            break;
         case -1:
            done = true;
            break;
         default:
            c = is.get();
            row.symbols.emplace_back(col, 0, 1, c);
            col++;
            break;
      }
   }
   return data;
}

void part1/* and 2*/(std::istream &is, std::ostream &os) {
   Data data = parse(is);

   static std::vector<Sym> empty;
   unsigned long total = 0;
   for (size_t rowNum = 0; rowNum < data.size(); ++rowNum) {

      // We iterate over the numbers in each row primarily, but maintain
      // cursors into the current row, and the row above and below that drag
      // along with the numbers. Store those cursors in symrows.

      struct SymState { std::vector<Sym>::iterator cur, end; };

      std::vector<SymState> symrows;
      if (rowNum > 0)
         symrows.emplace_back(data[rowNum-1].symbols.begin(), data[rowNum-1].symbols.end());
      symrows.emplace_back(data[rowNum].symbols.begin(), data[rowNum].symbols.end());
      if (rowNum < data.size() - 1)
         symrows.emplace_back(data[rowNum+1].symbols.begin(), data[rowNum+1].symbols.end());

      for (auto &num : data[rowNum].nums) {
         bool present = false;
         // Pull the cursors to the symbols forward to the start column of the
         // string for the number, then iterate over each symbol that is
         // adjacent to the number.  If any symbols are found, we know we've to
         // increment our total. Also, record the product of all adjacent
         // numbers to the symbol as we go, so we can sum them and make part 2
         // trivial

         for (SymState &p : symrows) {
            while (p.cur != p.end && p.cur->col < num.colStart - 1)
               ++p.cur;
            auto i = p.cur;
            while (i != p.end && i->col <= num.colEnd) {
               ++i->adjCount;
               i->adjProduct *= num.value;
               ++i;
               present = true;
            }
         }
         if (present)
            total += num.value;
      }
   }
   os << total << "\n";

   // For part two, just find all the "*" symbols with two recorded adjacent
   // numbers, and add the recorded product-of-adjacent-numbers to the total.
   total = 0;
   for (size_t rowNum = 0; rowNum < data.size(); ++rowNum) {
      for (auto &sym : data[rowNum].symbols) {
         if (sym.symbol == '*' && sym.adjCount == 2)
            total += sym.adjProduct;
      }
   }
   os << total << "\n";
}

void part2(std::istream &is, std::ostream &os) { }
