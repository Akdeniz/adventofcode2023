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

void tilt_north(std::vector<std::string>& table) {
    for(int j=0; j < table[0].size(); ++j) {
        int empty_pos = 0;
        for(int i=0; i < table.size(); ++i) {
            if(table[i][j]=='#') {
                empty_pos = i+1;
            } else if(table[i][j]=='O') {
                std::swap(table[i][j], table[empty_pos][j]);
                ++empty_pos;
            }
        }
    }
}

void tilt_south(std::vector<std::string>& table) {
    for(int j=0; j < table[0].size(); ++j) {
        int empty_pos = table.size()-1;
        for(int i=table.size()-1; i >= 0; --i) {
            if(table[i][j]=='#') {
                empty_pos = i-1;
            } else if(table[i][j]=='O') {
                std::swap(table[i][j], table[empty_pos][j]);
                --empty_pos;
            }
        }
    }
}

void tilt_west(std::vector<std::string>& table) {
    for(int i=0; i < table.size(); ++i) {
        int empty_pos = 0;
        for(int j=0; j < table[0].size(); ++j) {
            if(table[i][j]=='#') {
                empty_pos = j+1;
            } else if(table[i][j]=='O') {
                std::swap(table[i][j], table[i][empty_pos]);
                ++empty_pos;
            }
        }
    }
}

void tilt_east(std::vector<std::string>& table) {
    for(int i=0; i < table.size(); ++i) {
        int empty_pos = table[0].size()-1;
        for(int j=table[0].size()-1; j >= 0; --j) {
            if(table[i][j]=='#') {
                empty_pos = j-1;
            } else if(table[i][j]=='O') {
                std::swap(table[i][j], table[i][empty_pos]);
                --empty_pos;
            }
        }
    }
}

std::pair<int64_t, int64_t> find_cycle(std::vector<std::string> table) {
    std::map<std::vector<std::string>, int64_t> seen;
    for(int64_t i=0; i < 1000000000; ++i) {
        tilt_north(table);
        tilt_west(table);
        tilt_south(table);
        tilt_east(table);

        if(seen.count(table)>0) {
            return {seen[table], i};
        }
        seen[table] = i;
    }
    assert(false);
}

void part1(std::vector<std::string> table) {

    tilt_north(table);

    int64_t result = 0;
    int64_t point = 1;
    for(int i=table.size()-1; i >= 0 ; --i) {
        for(int j=0; j < table[i].size(); ++j) {
            if(table[i][j]=='O')
                result+=point;
        }
        ++point;
    }
    std::cout << "part1: " << result << std::endl;
}

void part2(std::vector<std::string> table) {

    auto cycle = find_cycle(table);

    int64_t range = 1000000000;
    range-=cycle.first;
    range%=(cycle.second-cycle.first);
    range+=cycle.first;
    for(int64_t i=0; i < range; ++i) {
        tilt_north(table);
        tilt_west(table);
        tilt_south(table);
        tilt_east(table);
    }


    int64_t result = 0;
    int64_t point = 1;
    for(int i=table.size()-1; i >= 0 ; --i) {
        for(int j=0; j < table[i].size(); ++j) {
            if(table[i][j]=='O')
                result+=point;
        }
        ++point;
    }
    std::cout << "part2: " << result << std::endl;
}

int main() {

    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");
    std::vector<std::string> table;
    std::string line;
    while (std::getline(ifs, line)) {
        table.push_back(line);
    }

    part1(table);
    part2(table);

    return 0;
}
