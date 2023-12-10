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

const auto LEFT = std::make_pair<int, int>(0, -1);
const auto RIGHT = std::make_pair<int, int>(0, 1);
const auto UP = std::make_pair<int, int>(-1, 0);
const auto DOWN = std::make_pair<int, int>(1, 0);

const std::vector<std::pair<int, int>> NEIGHS = {LEFT,
                                                 RIGHT,
                                                 UP,
                                                 DOWN};

std::optional<std::pair<int, int>> get_next_pos(std::pair<int, int> pos, std::pair<int, int> dir, const char c) {

    assert(std::string("|-LJ7F").find(c) != std::string::npos);
    std::pair<int, int> result;
    switch (c) {
        case '|': {
            if (dir != UP && dir != DOWN) return std::nullopt;
            result = {pos.first + dir.first, pos.second};
            break;
        }
        case '-': {
            if (dir != LEFT && dir != RIGHT) return std::nullopt;
            result = {pos.first, pos.second + dir.second};
            break;
        }
        case 'L': {
            if (dir != LEFT and dir != DOWN) return std::nullopt;
            result = {pos.first + dir.second, pos.second + dir.first};
            break;
        }
        case 'J': {
            if (dir != RIGHT && dir != DOWN) return std::nullopt;
            result = {pos.first - dir.second, pos.second - dir.first};
            break;
        }
        case '7': {
            if (dir != RIGHT && dir != UP) return std::nullopt;
            result = {pos.first + dir.second, pos.second + dir.first};
            break;
        }
        case 'F': {
            if (dir != LEFT && dir != UP) return std::nullopt;
            result = {pos.first - dir.second, pos.second - dir.first};
            break;
        }
    }
    return result;
}

bool dfs(std::vector<std::string> &table, const std::pair<int, int> &prevpos, const std::pair<int, int> &pos,
         const std::pair<int, int> &endpos, std::set<std::pair<int, int>> &visited) {

    if (pos.first < 0 || pos.first >= table.size() || pos.second < 0 || pos.second >= table[0].size())
        return false;

    if (table[pos.first][pos.second] == '.')
        return false;

    const std::pair<int, int> dir = {pos.first - prevpos.first, pos.second - prevpos.second};

    if (pos == endpos) {
        visited.insert(pos);
        return true;
    }

    auto nextpos = get_next_pos(pos, dir, table[pos.first][pos.second]);
    if (!nextpos.has_value())
        return false;

    char current_char = '.';
    std::swap(table[pos.first][pos.second], current_char);

    if (!dfs(table, pos, *nextpos, endpos, visited)) {
        return false;
    }
    std::swap(table[pos.first][pos.second], current_char);

    visited.insert(pos);
    return true;
}

void part1(std::vector<std::string> table, std::pair<int, int> start) {


    std::set<std::pair<int, int>> visited;
    for (const auto &neigh: NEIGHS) {
        std::pair<int, int> nextpos = {start.first + neigh.first, start.second + neigh.second};
        if (dfs(table, start, nextpos, start, visited))
            break;
    }

    std::cout << "Part1: " << (visited.size() + 1) / 2 << std::endl;
}

int part2(std::vector<std::string> table, std::pair<int, int> start) {

    std::set<std::pair<int, int>> visited;

    for (const auto &neigh: NEIGHS) {
        std::pair<int, int> nextpos = {start.first + neigh.first, start.second + neigh.second};
        if (dfs(table, start, nextpos, start, visited))
            break;
    }

    int64_t result = 0;
    for (int i = 0; i < table.size(); ++i) {
        char last_ch = '-';
        bool inside = false;
        for (int j = 0; j < table[i].size(); ++j) {

            if (visited.count({i, j}) == 0) {
                if (inside) {
                    ++result;
                }
            } else {
                const auto ch = table[i][j];
                switch (ch) {
                    case '|':
                    case 'F':
                    case 'L': {
                        inside = !inside;
                        last_ch = ch;
                        break;
                    }
                    case 'J': {
                        if (last_ch == 'L') inside = !inside;
                        last_ch = ch;
                        break;
                    }
                    case '7': {
                        if (last_ch == 'F') inside = !inside;
                        last_ch = ch;
                        break;
                    }
                }
            }
        }
    }

    std::cout << "Part2: " << result << std::endl;
    return 0;
}

int main() {

    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");
    std::vector<std::string> table;
    std::string line;
    while (std::getline(ifs, line)) {
        table.push_back(line);
    }

    std::pair<int, int> start;
    for (int i = 0; i < table.size(); ++i) {
        auto pos = table[i].find('S');
        if (pos != std::string::npos) {
            start = {i, pos};
            break;
        }
    }

    {
        bool left =
                start.second > 0 && std::string("-LF").find(table[start.first][start.second - 1]) != std::string::npos;
        bool right = start.second + 1 < table[0].size() &&
                     std::string("-J7").find(table[start.first][start.second + 1]) != std::string::npos;
        bool up = start.first > 0 && std::string("|7F").find(table[start.first - 1][start.second]) != std::string::npos;
        bool down = start.first + 1 < table.size() &&
                    std::string("|LJ").find(table[start.first + 1][start.second]) != std::string::npos;

        std::vector<int> dir{left, right, up, down};
        if (dir == std::vector<int>{1, 1, 0, 0}) table[start.first][start.second] = '-';
        else if (dir == std::vector<int>{0, 0, 1, 1}) table[start.first][start.second] = '|';
        else if (dir == std::vector<int>{0, 1, 1, 0}) table[start.first][start.second] = 'L';
        else if (dir == std::vector<int>{1, 0, 1, 0}) table[start.first][start.second] = 'J';
        else if (dir == std::vector<int>{0, 1, 0, 1}) table[start.first][start.second] = 'F';
        else if (dir == std::vector<int>{1, 0, 0, 1}) table[start.first][start.second] = '7';
        else
            assert(false);
    }

    part1(table, start);
    part2(table, start);
}
