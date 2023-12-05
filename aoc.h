#include <iostream>
#include <functional>

void timeit(std::function<void(void)>);
extern void part1(std::istream &, std::ostream &);
extern void part2(std::istream &, std::ostream &);
extern void parse(std::istream &, std::ostream &);
typedef void (*partfn)(std::istream &, std::ostream &);

namespace aoc {

inline std::pair<std::string, std::string> token(std::string_view line, const std::string_view sep = " ") {
    auto split = line.find(sep);
    if (split == std::string::npos)
        return { std::string(line), "" };
    else
        return { std::string(line.substr(0, split)), std::string(line.substr(split + sep.size())) };
}

}
