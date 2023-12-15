#include <iostream>
#include <functional>

void timeit(std::function<void(void)>);

namespace aoc {

using Executor = std::function<void(std::istream &, std::ostream &)>;
struct Case  {
   Case (std::string_view name, Executor func);
};

inline std::pair<std::string, std::string> token(const std::string &line, const std::string & sep = " ") {
    auto split = line.find(sep);
    if (split == std::string::npos)
        return { std::string(line), "" };
    else
        return { std::string(line.substr(0, split)), std::string(line.substr(split + sep.size())) };
}

}
