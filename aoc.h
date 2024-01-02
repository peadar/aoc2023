#include <bits/stdc++.h>

void timeit(std::function<void(void)>);

namespace aoc {

using Executor = std::function<void(std::istream &, std::ostream &)>;
struct Case  {
   Case (std::string_view name, Executor func);
};

template <typename String>
inline std::pair<String, String> token(const String line, const String sep = " ") {
    auto split = line.find(sep);
    if (split == std::string::npos)
        return { line, "" };
    else
        return { line.substr(0, split), line.substr(split + sep.size()) };
}

}
