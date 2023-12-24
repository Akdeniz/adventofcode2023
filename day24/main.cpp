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
#include <array>

#include <z3++.h>

struct Vec3 {
    double x, y, z;
};

struct Hailstorm {
    Vec3 position, direction;
};

std::optional<Vec3> intersection(Vec3 p1, Vec3 v1, Vec3 p2, Vec3 v2) {

    // (y-p1.y)/(x-p1.x)==(v1.y/v1.x)
    // (y-p2.y)/(x-p2.x)==(v2.y/v2.x)

    const auto slope1 = v1.y / v1.x;
    const auto slope2 = v2.y / v2.x;

    // (y-p1.y)==slope1 * (x-p1.x)
    // (y-p2.y)==slope2 * (x-p2.x)

    // y==slope1 * (x-p1.x) + p1.y

    // slope1 * (x-p1.x)==slope2*x-slope2*p2.x-p1.y+p2.y

    // slope1*x-slope1*p1.x==slope2*x-slope2*p2.x-p1.y+p2.y

    // slope1*x-slope2*x==-slope2*p2.x-p1.y+p2.y+slope1*p1.x

    // x==(-slope2*p2.x-p1.y+p2.y+slope1*p1.x)/(slope1-slope2)

    if (slope1 != slope2) {
        const auto x = (-slope2 * p2.x - p1.y + p2.y + slope1 * p1.x) / (slope1 - slope2);
        const auto y = slope1 * (x - p1.x) + p1.y;
        return Vec3{.x = x, .y = y, .z = 0};
    }

    return std::nullopt;
}

bool is_in_forward(Vec3 found, Vec3 p, Vec3 v) {
    return !((found.x < p.x && v.x > 0) || (found.x > p.x && v.x < 0) || (found.y < p.y && v.y > 0) || (
            found.y > p.y && v.y < 0));
}

int main() {
    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");
    static std::regex base_regex("(\\d+),[ ]*(\\d+),[ ]*(\\d+)[ ]*@[ ]*(-?\\d+),[ ]*(-?\\d+),[ ]*(-?\\d+)");
    std::smatch match;

    std::vector<Hailstorm> hailstorms;
    std::string line;
    while (std::getline(ifs, line)) {
        if (std::regex_match(line, match, base_regex)) {
            hailstorms.push_back({
                                         {std::stod(match[1].str()), std::stod(match[2].str()), std::stod(
                                                 match[3].str())},
                                         {std::stod(match[4].str()), std::stod(match[5].str()), std::stod(
                                                 match[6].str())}
                                 });
        } else
            assert(false);
    }

    //const std::pair<double, double> RANGE = {7, 27};
    const std::pair<double, double> RANGE = {200000000000000, 400000000000000};
    int64_t result = 0;
    for (int i = 0; i < hailstorms.size(); ++i) {
        for (int j = i + 1; j < hailstorms.size(); ++j) {
            const auto &p1 = hailstorms[i].position;
            const auto &v1 = hailstorms[i].direction;
            const auto &p2 = hailstorms[j].position;
            const auto &v2 = hailstorms[j].direction;

            if (auto found = intersection(p1, v1, p2, v2)) {
                auto point = *found;
                if (is_in_forward(point, p1, v1) && is_in_forward(*found, p2, v2)) {
                    if (point.x >= RANGE.first && point.x <= RANGE.second && point.y >= RANGE.first && point.y <= RANGE.
                            second) {
                        ++result;
                    }
                }
            }
        }
    }
    std::cout << "part1: " << result << std::endl;


    z3::context ctx;
    auto px = ctx.real_const("px");
    auto py = ctx.real_const("py");
    auto pz = ctx.real_const("pz");
    auto vx = ctx.real_const("vx");
    auto vy = ctx.real_const("vy");
    auto vz = ctx.real_const("vz");

    std::vector<z3::expr> times;
    for (int i = 0; i < hailstorms.size(); ++i) {
        std::stringstream ss;
        ss << "t" << i;
        times.push_back(ctx.real_const(ss.str().c_str()));
    }

    z3::solver solver(ctx);

    auto to_real = [&ctx](double x) {
        return ctx.real_val(static_cast<int64_t>(x));
    };

    for (int i = 0; i < times.size(); ++i) {
        auto &t = times[i];
        auto &hail = hailstorms[i];
        solver.add(t * to_real(hail.direction.x) + to_real(hail.position.x) == px + vx * t);
        solver.add(py + vy * t == t * to_real(hail.direction.y) + to_real(hail.position.y));
        solver.add(pz + vz * t == t * to_real(hail.direction.z) + to_real(hail.position.z));
    }

    assert(solver.check() == z3::check_result::sat);

    auto model = solver.get_model();

    z3::expr_vector total(ctx);
    total.push_back(px);
    total.push_back(py);
    total.push_back(pz);

    std::cout << "part2: " << model.eval(z3::sum(total)).as_int64() << std::endl;

    return 0;
}
