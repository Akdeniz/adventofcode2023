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
    static std::regex base_regex("(-?\\d+)");

    std::vector<int64_t> result;
    for (auto it = std::sregex_iterator(str.begin(), str.end(), base_regex);
         it != std::sregex_iterator(); ++it) {

        auto value = std::stoll(it->operator[](1).str());
        result.push_back(value);
    }
    return result;
}

int64_t solve(bool backwards) {

    std::fstream ifs(std::string(SRC_DIR)+"/input.txt");

    int64_t result = 0;
    std::string line;
    while(std::getline(ifs, line))
    {
        std::vector<std::vector<int64_t>> history = {find_integers(line)};
        while(!std::all_of(history.back().begin(), history.back().end(), [](int64_t value){ return value==0;})) {
            const auto& last = history.back();
            std::vector<int64_t> newline;
            for(int i=1; i < last.size(); ++i) {
                newline.push_back(last[i]-last[i-1]);
            }
            history.emplace_back(std::move(newline));
        }

        int64_t predicted = 0;
        for(int i=history.size()-2; i >= 0; --i) {
            if(backwards)
                predicted = history[i].front()-predicted;
            else
                predicted = history[i].back()+predicted;
        }
        result+=predicted;
    }

    return result;
}


int main() {

    std::cout << "Part1: " << solve(false) << std::endl;
    std::cout << "Part2: " << solve(true) << std::endl;
    return 0;
}
