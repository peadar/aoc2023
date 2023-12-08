#include "aoc.h"
#include <fstream>
#include <iterator>

#include <unistd.h>

int main(int argc, char *argv[]) {
   bool do_timeit { false };
   std::string parts = "*";
   for (int c; (c = getopt(argc, argv, "tp:")) != -1; ) {
      switch (c) {
         case 't':
            do_timeit = true;
            break;
         case 'p':
            parts = optarg;
            break;
      }
   }

   struct {
      const char *name;
      void (*func)(std::istream &, std::ostream &);
   } funcs[] = {
      { "null", [](std::istream &, std::ostream &) -> void {} },
      { "part1", part1 },
      { "part2", part2 },
   };

   std::ifstream in( argv[optind], std::ifstream::binary);

   if (do_timeit) {
      // read the content of the file into memory, and issue it from there.
      // copy content into vector of char.
      std::vector<char> buf;
      in.seekg(0);
      std::copy( std::istreambuf_iterator<char>(in.rdbuf()), std::istreambuf_iterator<char>(), std::back_inserter(buf));

      struct SB : std::streambuf {
         std::vector<char> &vec;
         SB(std::vector<char> &vec) : vec(vec) {
         }
         void reset() {
            setg(vec.data(), vec.data(), vec.data() + vec.size());
         }
      };

      std::fstream null("/dev/null");
      SB sb(buf);

      for (auto [name, func] : funcs) {
         if (parts != "*" && parts != name)
            continue;
         std::cout << "function: " << name << "\n";
         timeit([&] {
               sb.reset();
               std::istream memin(&sb);
               func(memin, null);
               });
      }
      return 0;
   } else {
      for (auto [ name, func] : funcs) {
         func(in, std::cout);
         in.clear();
         in.seekg(0);
      }
   }
}
