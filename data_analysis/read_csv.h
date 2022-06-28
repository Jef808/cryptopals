#ifndef READ_CSV_H_
#define READ_CSV_H_

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace data {


template<typename T>
inline std::pair<std::vector<std::vector<T>>, bool>
read_csv(const std::string& fn, bool header)
{
    std::vector<std::vector<T>> ret;
    std::string buf;

    std::ifstream ifs{ fn };

    if (not ifs) {
        std::cerr << "Failed to open input file " << fn << std::endl;
        return std::make_pair(ret, false);
    }

    std::getline(ifs, buf);

    size_t n_items = std::count(buf.begin(), buf.end(), ',') + 1;

    // Discard header line
    if (header) {
        std::getline(ifs, buf);
    }

    auto& vec = ret.emplace_back();
    vec.reserve(n_items);

    std::string_view sv{ buf };

    for (size_t i = 0; i < n_items; ++i) {

        size_t comma = sv.find(',', 0);
        std::stringstream ss { sv.substr(0, comma).data() };

        T& val = vec.emplace_back();
        ss >> val;

        sv.remove_prefix(comma + 1);
    }

    return std::make_pair(ret, true);
}

} // namespace data

#endif // READ_CSV_H_
