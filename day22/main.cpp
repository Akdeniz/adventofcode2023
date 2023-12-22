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

struct Point {
    int64_t x, y, z;
};

bool operator<(const Point& a, const Point& b) {
    if (a.x != b.x)
        return a.x < b.x;
    if (a.y != b.y)
        return a.y < b.y;
    return a.z < b.z;
}

struct Brick {
    Point start, end;
    std::vector<Point> cubes;
};

std::vector<Point> cubes(const Brick& brick) {
    std::vector<Point> result;
    if (brick.start.x != brick.end.x) {
        assert(brick.start.y==brick.end.y && brick.start.z==brick.end.z);
        for (int t = std::min(brick.start.x, brick.end.x); t <= std::max(brick.start.x, brick.end.x); ++t)
            result.push_back({t, brick.start.y, brick.start.z});
    } else if (brick.start.y != brick.end.y) {
        assert(brick.start.x==brick.end.x && brick.start.z==brick.end.z);
        for (int t = std::min(brick.start.y, brick.end.y); t <= std::max(brick.start.y, brick.end.y); ++t)
            result.push_back({brick.start.x, t, brick.start.z});
    } else {
        assert(brick.start.x==brick.end.x && brick.start.y==brick.end.y);
        for (int t = std::min(brick.start.z, brick.end.z); t <= std::max(brick.start.z, brick.end.z); ++t)
            result.push_back({brick.start.x, brick.start.y, t});
    }
    return result;
}

std::vector<Point> adjust_z_pos(std::vector<Point> input, int64_t base_z) {
    int64_t z_offset = input[0].z;
    for (auto& pos: input)
        z_offset = std::min(z_offset, pos.z);
    for (auto& pos: input) {
        pos.z += (base_z - z_offset);
    }
    return input;
}

int64_t find_dependents(const std::vector<Brick>& bricks, const std::map<Point, int64_t> point_to_idx, int start) {
    std::set<int> dependents = {start};
    std::set<int> moved = {start};

    while (!moved.empty()) {
        std::set<int> possible_dependents;
        for (int idx: moved) {
            for (const auto& pos: bricks[idx].cubes) {
                auto it = point_to_idx.find({pos.x, pos.y, pos.z + 1});
                if (it != point_to_idx.end() && dependents.count(it->second) == 0)
                    possible_dependents.insert(it->second);
            }
        }
        moved.clear();

        for (int dependent: possible_dependents) {
            std::set<int> supporting;
            for (const auto& pos: bricks[dependent].cubes) {
                auto it = point_to_idx.find({pos.x, pos.y, pos.z - 1});
                if (it != point_to_idx.end() && dependents.count(it->second) == 0 && it->second != dependent)
                    supporting.insert(it->second);
            }
            if (supporting.empty()) {
                moved.insert(dependent);
            }
        }

        dependents.insert(moved.begin(), moved.end());
    }
    return dependents.size() - 1;
}


int main() {
    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");

    static std::regex base_regex("(\\d+),(\\d+),(\\d+)~(\\d+),(\\d+),(\\d+)");
    std::smatch match;

    std::vector<Brick> bricks;
    std::string line;
    while (std::getline(ifs, line) && !line.empty()) {
        if (std::regex_match(line, match, base_regex)) {
            bricks.push_back({
                {std::stoll(match[1].str()), std::stoll(match[2].str()), std::stoll(match[3].str())},
                {std::stoll(match[4].str()), std::stoll(match[5].str()), std::stoll(match[6].str())}
            });
        } else
            assert(false);
    }

    std::sort(bricks.begin(), bricks.end(), [](auto a, auto b) {
        return std::min(a.start.z, a.end.z) < std::min(b.start.z, b.end.z);
    });

    std::map<Point, int64_t> point_to_idx;
    std::map<std::pair<int64_t, int64_t>, int64_t> max_z_in_xy_plane;
    // if no point, that means 0

    for (int i = 0; i < bricks.size(); ++i) {
        auto& brick = bricks[i];

        auto cube_positions = cubes(brick);
        int64_t max_z = -1;
        for (const auto& pos: cube_positions) {
            auto it = max_z_in_xy_plane.find({pos.x, pos.y});
            if (it != max_z_in_xy_plane.end())
                max_z = std::max(max_z, it->second);
        }
        brick.cubes = adjust_z_pos(cube_positions, max_z + 1);
        for (const auto& pos: brick.cubes) {
            max_z_in_xy_plane[{pos.x, pos.y}] = std::max(max_z_in_xy_plane[{pos.x, pos.y}], pos.z);
            assert(point_to_idx.count(pos)==0);
            point_to_idx[pos] = i;
        }
    }

    int64_t can_remove_count = 0;
    for (int i = 0; i < bricks.size(); ++i) {
        std::set<int> dependents;
        for (const auto& pos: bricks[i].cubes) {
            auto it = point_to_idx.find({pos.x, pos.y, pos.z + 1});
            if (it != point_to_idx.end() && it->second != i)
                dependents.insert(it->second);
        }

        bool can_remove = true;
        for (int dependent: dependents) {
            std::set<int> supporting;
            for (const auto& pos: bricks[dependent].cubes) {
                auto it = point_to_idx.find({pos.x, pos.y, pos.z - 1});
                if (it != point_to_idx.end() && it->second != i && it->second != dependent)
                    supporting.insert(it->second);
            }
            can_remove &= !supporting.empty();
        }

        if (can_remove) {
            ++can_remove_count;
        }
    }

    std::cout << "part1: " << can_remove_count << std::endl;


    int64_t result = 0;
    for (int i = 0; i < bricks.size(); ++i) {
        result += find_dependents(bricks, point_to_idx, i);
    }

    std::cout << "part2: " << result << std::endl;

    return 0;
}
