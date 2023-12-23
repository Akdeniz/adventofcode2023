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

struct Game {
    const std::vector<std::pair<int64_t, int64_t>> NEIGHS = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};

    Game(std::vector<std::string> table) : table(table),
                                           visited(table.size(), std::vector<bool>(table.front().size(), false)) {
    }

    int64_t longest_path(int64_t x, int64_t y, bool part2 = false) {
        if (x < 0 || x >= table.size() || y < 0 || y >= table.front().size() || table[x][y] == '#')
            return -1;

        if (x + 1 == table.size() && y + 2 == table.back().size()) {
            return 0;
        }

        if (visited[x][y])
            return -1;

        visited[x][y] = true;

        int64_t result = -1;
        const char c = table[x][y];
        if (c == '.' || part2) {
            for (const auto& neigh: NEIGHS) {
                auto res = longest_path(x + neigh.first, y + neigh.second, part2);
                if (res >= 0) result = std::max(result, res + 1);
            }
        } else if (c == '>') {
            auto res = longest_path(x, y + 1);
            if (res >= 0) result = std::max(result, res + 1);
        } else if (c == 'v') {
            auto res = longest_path(x + 1, y);
            if (res >= 0) result = std::max(result, res + 1);
        }

        visited[x][y] = false;
        return result;
    }

private:
    const std::vector<std::string> table;
    std::vector<std::vector<bool>> visited;
};


int main() {
    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");

    std::vector<std::string> table;
    std::string line;
    while (std::getline(ifs, line) && !line.empty()) {
        table.push_back(line);
    }

    Game game(table);
    std::cout << "part1: " << game.longest_path(0, 1, false) << std::endl;
    std::cout << "part2: " << game.longest_path(0, 1, true) << std::endl;

    return 0;
}
