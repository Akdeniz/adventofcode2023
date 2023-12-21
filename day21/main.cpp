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
#include <queue>

const std::vector<std::pair<int64_t, int64_t>> neighs = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

int64_t bfs(const std::vector<std::string>& table, std::pair<int64_t, int64_t> start) {
    std::set<std::pair<int64_t, int64_t>> current = {start};
    for (int i = 0; i < 64; ++i) {
        std::set<std::pair<int64_t, int64_t>> next;
        for (const auto& elem: current) {
            for (const auto& neigh: neighs) {
                int64_t newx = elem.first + neigh.first;
                int64_t newy = elem.second + neigh.second;

                if (newx < 0 || newx >= table.size() || newy < 0 || newy >= table[0].size() || table[newx][newy] != '.')
                    continue;
                next.insert({newx, newy});
            }
        }
        std::swap(current, next);
    }
    return current.size();
}

std::pair<int64_t, int64_t> starting_point(const std::vector<std::string>& table) {
    for (int i = 0; i < table.size(); ++i) {
        for (int j = 0; j < table[i].size(); ++j) {
            if (table[i][j] == 'S')
                return {i, j};
        }
    }
    assert(false);
}

int64_t mod(int64_t k, int64_t n) {
    return ((k % n) + n) % n;
}

int main() {
    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");


    std::vector<std::string> table;
    std::string line;
    while (std::getline(ifs, line)) {
        table.push_back(line);
    }

    auto start = starting_point(table);
    table[start.first][start.second] = '.';

    std::cout << "part1: " << bfs(table, start) << std::endl;

    std::map<std::pair<int64_t, int64_t>, int64_t> visited; {
        std::list<std::pair<int64_t, int64_t>> q = {start};
        int64_t distance = 0;
        while (!q.empty()) {
            int64_t qsize = q.size();
            while (qsize-- > 0) {
                auto current = q.front();
                q.pop_front();

                if (visited.count(current))
                    continue;

                visited[current] = distance;

                for (const auto& neigh: neighs) {
                    int64_t newx = current.first + neigh.first;
                    int64_t newy = current.second + neigh.second;

                    if (newx < 0 || newx >= table.size() || newy < 0 || newy >= table[0].size() || table[newx][newy] !=
                        '.')
                        continue;
                    q.push_back({newx, newy});
                }
            }
            ++distance;
        }
    }

    const int64_t sz = table.size();
    const int64_t half_sz = sz / 2;
    assert(static_cast<int64_t>(table.front().size()/2)==half_sz);

    int64_t even_corners = 0;
    int64_t odd_corners = 0;
    int64_t even_points = 0;
    int64_t odd_points = 0;
    for (const auto& [_,distance]: visited) {
        if (distance > half_sz) {
            if (distance % 2 == 0) ++even_corners;
            if (distance % 2 != 0) ++odd_corners;
        }
        if (distance % 2 == 0) ++even_points;
        if (distance % 2 != 0) ++odd_points;
    }

    const int64_t n = (26501365 - half_sz) / sz;
    assert(n==202300);

    int64_t even = n * n;
    int64_t odd = (n + 1) * (n + 1);

    int64_t result = odd * odd_points
                     + even * even_points
                     - (n + 1) * odd_corners
                     + n * even_corners;

    std::cout << "part2: " << result << std::endl;

    return 0;
}
