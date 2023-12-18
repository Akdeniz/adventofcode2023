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

struct Elem {
    char dir;
    int64_t len;
    std::string color;
    int64_t x, y;
};

std::pair<int64_t,int64_t> get_dir(char c) {
    switch (c) {
        case 'L': return {0, -1};
        case 'R': return {0, 1};
        case 'D': return {1, 0};
        case 'U': return {-1, 0};
    }
    assert(false);
}

int64_t shoe_lace(const std::vector<Elem>& table) {
    int64_t result = 0;
    int64_t length = 0;
    // https://www.101computing.net/the-shoelace-algorithm/
    const auto sz = table.size();
    for(int i=0; i < sz; ++i) {
        length+=table[i].len;
        result+=table[i].x*table[(i+1)%sz].y - table[(i+1)%sz].x*table[i].y;
    }
    return std::abs(result)/2+length/2+1;
}

int main() {
    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");

    // D 4 (#90bc33)
    static std::regex base_regex("(D|U|L|R) (\\d+) \\(#([a-f0-9#]+)\\)");
    std::smatch match;

    std::string line;
    std::vector<Elem> table;
    while (std::getline(ifs, line)) {
        if(std::regex_match(line,match, base_regex)) {
            table.push_back(Elem{.dir = match[1].str().front(), .len = std::stoll(match[2].str()), .color = match[3].str()});
        } else assert(false);
    }

    {
        std::pair<int64_t, int64_t> position = {0, 0};
        for(auto& elem: table) {
            elem.x = position.first;
            elem.y = position.second;

            auto dir_off = get_dir(elem.dir);
            position.first+=dir_off.first*(elem.len);
            position.second+=dir_off.second*(elem.len);
        }

        std::cout << "part1: " << shoe_lace(table) << std::endl;
    }

    {
        std::pair<int64_t, int64_t> position = {0, 0};
        for(auto& elem: table) {
            elem.x = position.first;
            elem.y = position.second;


            elem.len = std::stoll(elem.color.substr(0, 5), nullptr, 16);

            //0 means R, 1 means D, 2 means L, and 3 means U.
            switch (elem.color.back()) {
                case '0': {
                    elem.dir = 'R';
                    break;
                }
                case '1': {
                    elem.dir = 'D';
                    break;
                }
                case '2': {
                    elem.dir = 'L';
                    break;
                }
                case '3': {
                    elem.dir = 'U';
                    break;
                }
                default:
                    assert(false);

            }

            auto dir_off = get_dir(elem.dir);
            position.first+=dir_off.first*(elem.len);
            position.second+=dir_off.second*(elem.len);
        }

        std::cout << "part2: " << shoe_lace(table) << std::endl;
    }

    return 0;
}
