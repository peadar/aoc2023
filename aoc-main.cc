#include "aoc.h"
#include <fstream>
#include <iterator>
#include <unistd.h>

namespace aoc {
std::vector<std::pair<std::string, Executor>> functions [[gnu::init_priority(1000)]];

Case::Case(std::string_view name, Executor callable) {
   functions.emplace_back(name, callable);
}

}

struct SB : std::streambuf {
   std::vector<char> vec;
   SB(std::vector<char> &&vec) : vec(vec) { }
   void reset() { setg(vec.data(), vec.data(), vec.data() + vec.size()); }
};

SB bufferis(std::istream &is) {
      // read the content of the file into memory, and issue it from there.
      // copy content into vector of char.
      std::vector<char> buf;
      is.seekg(0);
      std::copy( std::istreambuf_iterator<char>(is.rdbuf()),
            std::istreambuf_iterator<char>(), std::back_inserter(buf));

      return SB{ std::move(buf) };
}


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

   std::ifstream in( argv[optind], std::ifstream::binary);

   if (do_timeit) {
      std::fstream null("/dev/null");
      auto sb = bufferis(in);
      for (auto [name, func] : aoc::functions) {
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
      std::cout << "running " << aoc::functions.size() << " parts \n";
      for (auto &[ name, func ] : aoc::functions) {
         std::cout << name << ": ";
         func(in, std::cout);
         std::cout << "\n";
         in.clear();
         in.seekg(0);
      }
   }
}
