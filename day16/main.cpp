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

typedef std::pair<int, int> PosType;
typedef std::pair<PosType, char> PosDirType;

int64_t bfs(const std::vector<std::string>& table, PosDirType start) {
    std::set<PosDirType> visited;
    std::deque<PosDirType> q = {start};

    while (!q.empty()) {
        auto cur = q.front();
        q.pop_front();

        const auto x = cur.first.first;
        const auto y = cur.first.second;
        const auto dir = cur.second;

        if (x < 0 || x >= table.size() || y < 0 || y >= table.front().size())
            continue;

        if (!visited.insert(cur).second)
            continue;

        // empty space (.), mirrors (/ and \), and splitters (| and -).

        switch (dir) {
            case 'R': {
                switch (table[x][y]) {
                    case '/': {
                        q.push_back({{x - 1, y}, 'U'});
                        break;
                    }
                    case '\\': {
                        q.push_back({{x + 1, y}, 'D'});
                        break;
                    }
                    case '|': {
                        q.push_back({{x - 1, y}, 'U'});
                        q.push_back({{x + 1, y}, 'D'});
                        break;
                    }
                    case '.':
                    case '-': {
                        q.push_back({{x, y + 1}, 'R'});
                        break;
                    }
                }
                break;
            }
            case 'L': {
                switch (table[x][y]) {
                    case '/': {
                        q.push_back({{x + 1, y}, 'D'});
                        break;
                    }
                    case '\\': {
                        q.push_back({{x - 1, y}, 'U'});
                        break;
                    }
                    case '|': {
                        q.push_back({{x - 1, y}, 'U'});
                        q.push_back({{x + 1, y}, 'D'});
                        break;
                    }
                    case '.':
                    case '-': {
                        q.push_back({{x, y - 1}, 'L'});
                        break;
                    }
                }
                break;
            }
            case 'U': {
                switch (table[x][y]) {
                    case '/': {
                        q.push_back({{x, y + 1}, 'R'});
                        break;
                    }
                    case '\\': {
                        q.push_back({{x, y - 1}, 'L'});
                        break;
                    }
                    case '-': {
                        q.push_back({{x, y + 1}, 'R'});
                        q.push_back({{x, y - 1}, 'L'});
                        break;
                    }
                    case '.':
                    case '|': {
                        q.push_back({{x - 1, y}, 'U'});
                        break;
                    }
                }
                break;
            }
            case 'D': {
                switch (table[x][y]) {
                    case '/': {
                        q.push_back({{x, y - 1}, 'L'});
                        break;
                    }
                    case '\\': {
                        q.push_back({{x, y + 1}, 'R'});
                        break;
                    }
                    case '-': {
                        q.push_back({{x, y + 1}, 'R'});
                        q.push_back({{x, y - 1}, 'L'});
                        break;
                    }
                    case '.':
                    case '|': {
                        q.push_back({{x + 1, y}, 'D'});
                        break;
                    }
                }
                break;
            }
        }
    }

    std::set<PosType> unique_pos;
    for (const auto& elem: visited)
        unique_pos.insert(elem.first);
    return unique_pos.size();
}

int main() {
    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");

    std::string line;
    std::vector<std::string> table;
    while (std::getline(ifs, line)) {
        table.push_back(line);
    }

    std::cout << "part1: " << bfs(table, {{0, 0}, 'R'}) << std::endl;

    int64_t result = 0; {
        for (int i = 0; i < table.front().size(); ++i) {
            result = std::max<int64_t>(result, bfs(table, {{0, i}, 'D'}));
            result = std::max<int64_t>(result, bfs(table, {{table.size() - 1, i}, 'U'}));
        }

        for (int i = 0; i < table.size(); ++i) {
            result = std::max<int64_t>(result, bfs(table, {{i, 0}, 'R'}));
            result = std::max<int64_t>(result, bfs(table, {{i, table.front().size() - 1}, 'L'}));
        }
    }
    std::cout << "part2: " << result << std::endl;

    return 0;
}
