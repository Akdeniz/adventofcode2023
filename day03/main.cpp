#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <sstream>
#include  <regex>
#include <optional>
#include <functional>

std::optional<int64_t> find_integer(std::vector<std::string> &table, int x, int y) {
    if(x < 0 || x >= table.size() || y < 0 || y >= table.front().size() || !std::isdigit(table[x][y])) {
        return std::nullopt;
    }

    while(y>0 && std::isdigit(table[x][y-1])) --y;
    int64_t value = 0;
    while(y < table.size() && std::isdigit(table[x][y])) {
        value*=10;
        value+=(table[x][y]-'0');
        table[x][y] = '.';
        ++y;
    }
    return value;
}

static const std::vector<std::pair<int,int>> neighs = {{-1,-1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

void part1(std::vector<std::string> table)
{
    int64_t total = 0;
    for(int i=0; i < table.size(); ++i) {
        for(int j=0; j < table[i].size(); ++j) {

            if(std::isdigit(table[i][j]) || table[i][j]=='.') {
                continue;
            }

            for(const auto& neigh : neighs) {
                if(auto value = find_integer(table, i + neigh.first, j + neigh.second)) {
                    total+=*value;
                }
            }
        }
    }

    std::cout << "Part1: " << total << std::endl;
}


void part2(std::vector<std::string> table)
{

    int64_t total = 0;

    for(int i=0; i < table.size(); ++i) {
        for(int j=0; j < table[i].size(); ++j) {

            if(table[i][j]!='*') {
                continue;
            }

            std::vector<int64_t> powers;
            for(const auto& neigh : neighs) {
                if(auto value = find_integer(table, i + neigh.first, j + neigh.second)) {
                    powers.push_back(*value);
                }
            }

            if(powers.size()>=2) {
                total += std::accumulate(powers.begin(), powers.end(), 1ll, std::multiplies());
            }
        }
    }

    std::cout << "Part2: " << total << std::endl;
}

int main() {

    std::fstream ifs(std::string(SRC_DIR)+"/input.txt");
    std::string line;

    std::vector<std::string> table;
    while(std::getline(ifs, line)) {
        table.push_back(line);
    }

    part1(table);
    part2(table);
    return 0;
}
