#include "aoc.h"
#include <fstream>
#include <set>
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
   SB() {}
   SB(std::vector<char> &&vec) : vec(vec) {
      reset();
   }
   void reset() {
      setg(vec.data(), vec.data(), vec.data() + vec.size());
   }
   SB &operator = (std::vector<char> &&rhs) {
      vec = std::move(rhs);
      reset();
      return *this;
   }
};

std::vector<char> bufferis(std::istream &is) {
      // read the content of the file into memory, and issue it from there.
      // copy content into vector of char.
      std::vector<char> buf;
      is.seekg(0);
      std::copy( std::istreambuf_iterator<char>(is.rdbuf()),
            std::istreambuf_iterator<char>(), std::back_inserter(buf));
      return buf;
}

int main(int argc, char *argv[]) {
   bool do_timeit { false };
   std::set<std::string> parts;
   bool quiet { false };

   for (int c; (c = getopt(argc, argv, "tp:q")) != -1; ) {
      switch (c) {
         case 't':
            do_timeit = true;
            break;
         case 'p':
            parts.insert(optarg);
            break;
         case 'q':
            quiet = true;
            break;
      }
   }

   std::ifstream in( argv[optind], std::ifstream::binary);
   std::fstream null;
   SB sb;
   std::function<aoc::Executor (aoc::Executor)> wrap;
   if (do_timeit) {
      null = std::fstream("/dev/null");
      sb = bufferis(in);
      wrap = [&] (aoc::Executor e) -> aoc::Executor {
         return [&, e](std::istream &, std::ostream &) {
            timeit([&, e] {
                  sb.reset();
                  std::istream memin(&sb);
                  e(memin, null);
                  });
         };
      };
   } else {
      wrap = [] (aoc::Executor e) { return e; };
   }

   for (auto &[ name, func ] : aoc::functions) {
      if (!parts.empty() && parts.find(name) == parts.end())
         continue;
      if (!quiet)
         std::cout << name << ": ";
      wrap(func)(in, std::cout);
      if (!quiet)
         std::cout << "\n";
      in.clear();
      in.seekg(0);
   }
}
