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

typedef std::pair<int, int> PosType;
typedef std::pair<PosType, char> PosDirType;

int64_t bfs(const std::vector<std::vector<int64_t>>& table, const int min_move, const int max_move) {
    const int64_t row_size = table.size();
    const int64_t col_size = table[0].size();

    std::set<PosDirType> visited;

    typedef std::pair<int64_t, PosDirType> PQType;
    std::priority_queue<PQType, std::vector<PQType>, std::greater<>> pq;
    pq.push({0, {{0, 0}, 'D'}});
    pq.push({0, {{0, 0}, 'R'}});

    while (!pq.empty()) {
        const auto [value, pos_dir] = pq.top();
        pq.pop();

        const auto x = pos_dir.first.first;
        const auto y = pos_dir.first.second;
        const auto dir = pos_dir.second;

        if (x == row_size - 1 && y == col_size - 1)
            return value;

        if (x < 0 || x >= row_size || y < 0 || y >= col_size)
            continue;

        if (!visited.insert(pos_dir).second) {
            continue;
        }

        switch (dir) {
            case 'D': {
                if (x + min_move >= row_size) break;
                int64_t path = 0;
                for (int i = x + 1; i < row_size && i <= x + max_move; ++i) {
                    path += table[i][y];
                    if (i - x >= min_move) {
                        pq.push({value + path, {{i, y}, 'L'}});
                        pq.push({value + path, {{i, y}, 'R'}});
                    }
                }
                break;
            }
            case 'U': {
                if (x - min_move < 0) break;
                int64_t path = 0;
                for (int i = x - 1; i >= 0 && i >= x - max_move; --i) {
                    path += table[i][y];
                    if (x - i >= min_move) {
                        pq.push({value + path, {{i, y}, 'L'}});
                        pq.push({value + path, {{i, y}, 'R'}});
                    }
                }
                break;
            }
            case 'R': {
                if (y + min_move >= col_size) break;
                int64_t path = 0;
                for (int i = y + 1; i < col_size && i <= y + max_move; ++i) {
                    path += table[x][i];
                    if (i - y >= min_move) {
                        pq.push({value + path, {{x, i}, 'U'}});
                        pq.push({value + path, {{x, i}, 'D'}});
                    }
                }
                break;
            }

            case 'L': {
                if (y - min_move < 0) break;
                int64_t path = 0;
                for (int i = y - 1; i >= 0 && i >= y - max_move; --i) {
                    path += table[x][i];
                    if (y - i >= min_move) {
                        pq.push({value + path, {{x, i}, 'U'}});
                        pq.push({value + path, {{x, i}, 'D'}});
                    }
                }
                break;
            }
        }
    }
    return -1;
}

int main() {
    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");

    std::string line;
    std::vector<std::vector<int64_t>> table;
    while (std::getline(ifs, line)) {
        table.push_back({});
        for (char c: line)
            table.back().push_back(c - '0');
    }

    std::cout << "part1: " << bfs(table, 0, 3) << std::endl;
    std::cout << "part2: " << bfs(table, 4, 10) << std::endl;
    return 0;
}
