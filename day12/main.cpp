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
#include <list>

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

bool match(const std::string& input, int input_idx, int64_t group_size) {
    while(input_idx < input.size() && (input[input_idx]=='#' || input[input_idx]=='?') && group_size > 0) {
        group_size--;
        input_idx++;
    }

    if(group_size!=0)
        return false;

    return input_idx == input.size() || input[input_idx]!='#';
}

int64_t dfs(const std::string& input, int input_idx, const std::vector<int64_t>& groups, int group_idx) {

    if(group_idx==groups.size() && input_idx==input.size())
        return 1;

    if(group_idx>=groups.size())
        return 0;

    if(input_idx>=input.size())
        return 0;

    if(input[input_idx]=='#') {
        if(match(input, input_idx, groups[group_idx])) {
            return dfs(input, input_idx+groups[group_idx]+1, groups, group_idx+1);
        }
    } else if(input[input_idx]=='?') {
        int64_t result = 0;
        if(match(input, input_idx, groups[group_idx])) {
            result+=dfs(input, input_idx+groups[group_idx]+1, groups, group_idx+1);
        }
        result+=dfs(input, input_idx+1, groups, group_idx);
        return result;
    } else {
        return dfs(input, input_idx+1, groups, group_idx);
    }
    return 0;
}

int64_t solve(const std::string& input, const std::vector<int64_t>& groups) {
    return dfs(input, 0, groups, 0);
}

int main() {

    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");
    std::vector<std::string> table;
    std::string line;
    while (std::getline(ifs, line)) {
        table.push_back(line);
    }

    int64_t result = 0;
    for(const auto& line: table) {
        auto pos = line.find(' ');
        result+=solve(line.substr(0, pos), find_integers(line.substr(pos+1)));
    }

    std::cout << "part1: " << result << std::endl;
}
