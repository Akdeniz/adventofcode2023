#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <sstream>
#include  <regex>
#include <optional>
#include <functional>
#include <set>
#include <unordered_map>
#include <map>

std::vector<int64_t> find_integers(const std::string &str) {
    static std::regex base_regex("(\\d+)");

    std::vector<int64_t> result;
    for (auto it = std::sregex_iterator(str.begin(), str.end(), base_regex);
         it != std::sregex_iterator(); ++it) {

        auto value = std::stoll(it->operator[](1).str());
        result.push_back(value);
    }
    return result;
}

void part1() {

    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");
    std::string line;

    std::getline(ifs, line);
    auto times = find_integers(line);
    std::getline(ifs, line);
    auto distances = find_integers(line);

    int64_t result = 1;
    for (int idx = 0; idx < times.size(); ++idx) {

        const auto time = times[idx];
        int64_t count = 0;
        for (int64_t i = 0; i <= time; ++i) {
            if (i * (time - i) > distances[idx]) {
                ++count;
            }
        }
        result *= count;
    }
    std::cout << "part1: " << result << std::endl;
}


void part2() {

    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");
    std::string line;

    std::getline(ifs, line);
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    auto times = find_integers(line);
    std::getline(ifs, line);
    line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
    auto distances = find_integers(line);


    const auto time = times[0];
    const auto distance = distances[0];
    int64_t count = 0;
    for (int64_t i = 0; i <= time; ++i) {
        if (i * (time - i) > distance) {
            ++count;
        }
    }

    std::cout << "part2: " << count << std::endl;
}


int main() {

    part1();
    part2();
    return 0;
}
