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

void part1(const std::string& directions, const std::map<std::string, std::pair<std::string, std::string>>& paths) {

    int64_t count = 0;
    std::string current = "AAA";
    while(current!="ZZZ") {
        char dir = directions[count%directions.size()];
        ++count;
        if(dir=='L') {
            current = paths.find(current)->second.first;
        } else if(dir=='R') {
            current = paths.find(current)->second.second;
        }
    }
    std::cout << "Part1: " << count << std::endl;
}

void part2(const std::string& directions, const std::map<std::string, std::pair<std::string, std::string>>& paths) {
    std::vector<std::string> currents;
    for(const auto& elem: paths) {
        if(elem.first.back()=='A'){
            currents.push_back(elem.first);
        }
    }

    std::vector<int64_t> cycles;
    for(auto current: currents) {
        int64_t count = 0;
        while(current.back()!='Z') {
            char dir = directions[count%directions.size()];
            ++count;
            if(dir=='L') {
                current = paths.find(current)->second.first;
            } else if(dir=='R') {
                current = paths.find(current)->second.second;
            }
        }
        cycles.push_back(count);
    }

    int64_t result = 1;
    for(auto cycle: cycles) {
        result = (result*cycle)/std::gcd(result, cycle);
    }

    std::cout << "Part2: " << result << std::endl;
}




int main() {

    std::fstream ifs(std::string(SRC_DIR)+"/input.txt");
    std::string directions;
    std::getline(ifs, directions);

    std::string line;
    std::map<std::string, std::pair<std::string, std::string>> paths;
    while(std::getline(ifs, line))
    {
        // GXT = (MQM, CHN)
        static const std::regex base_regex("([0-9A-Z]{3}) = \\(([0-9A-Z]{3}), ([0-9A-Z]{3})\\)");

        std::vector<int64_t> result;
        std::smatch match;
        if(std::regex_match(line,match, base_regex)) {
            paths[match[1].str()] = {match[2].str(), match[3].str()};
        }

    }

    part1(directions, paths);
    part2(directions, paths);
    return 0;
}
