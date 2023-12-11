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

int64_t solve(int64_t step) {

    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");
    std::vector<std::string> table;
    std::string line;
    while (std::getline(ifs, line)) {
        table.push_back(line);
    }

    std::vector<int64_t> rows;
    for(int i=0; i < table.size(); ++i) {
        int64_t prev = i==0 ? 0 : rows[i-1];
        if(table[i].find('#')==std::string::npos) {
            rows.push_back(prev+step);
        } else {
            rows.push_back(prev+1);
        }
    }

    std::vector<int64_t> cols;
    for(int j=0; j < table[0].size(); ++j) {
        int64_t prev = j==0 ? 0 : cols[j-1];
        bool found = false;
        for(int i=0; i < table.size() && !found; ++i) {
            if(table[i][j]=='#') found = true;
        }
        if(!found) {
            cols.push_back(prev+step);
        } else {
            cols.push_back(prev+1);
        }
    }

    std::vector<std::pair<int,int>> points;
    for(int i=0; i < table.size(); ++i) {
        for (int j = 0; j < table[i].size(); ++j) {
            if (table[i][j] == '#') {
                points.push_back({i, j});
            }
        }
    }

    int64_t result = 0;
    for(int i=0; i < points.size(); ++i) {
        for(int j=i+1; j < points.size(); ++j) {
            auto a = points[i];
            auto b = points[j];

            result+=(std::abs(rows[a.first]-rows[b.first])+std::abs(cols[a.second]-cols[b.second]));
        }
    }

    return result;
}

int main() {
    std::cout << "part1: " << solve(2) << std::endl;
    std::cout << "part2: " << solve(1000000) << std::endl;
}
