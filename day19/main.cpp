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

std::vector<std::string> split(const std::string& str, char c = ' ') {
    std::istringstream split(str);
    std::vector<std::string> tokens;
    for (std::string each; std::getline(split, each, c); tokens.push_back(each));
    return tokens;
}

struct Pred {
    bool always_true = false;
    char variable;
    char cmp;
    int64_t value;
    std::string output;
};

struct Register {
    int64_t x, m, a, s;

    int64_t value(char c) const {
        switch (c) {
            case 'x': return x;
            case 'm': return m;
            case 'a': return a;
            case 's': return s;
            default: assert(false);
        }
    }
};

struct Ranges {
    std::pair<int64_t, int64_t> x, m, a, s;

    bool empty() const {
        return x.first > x.second || m.first > m.second || a.first > a.second || s.first > s.second;
    }

    int64_t count() const {
        int64_t x_cnt = std::max<int64_t>(0, x.second - x.first + 1);
        int64_t m_cnt = std::max<int64_t>(0, m.second - m.first + 1);
        int64_t a_cnt = std::max<int64_t>(0, a.second - a.first + 1);
        int64_t s_cnt = std::max<int64_t>(0, s.second - s.first + 1);
        return x_cnt * m_cnt * a_cnt * s_cnt;
    }

    std::pair<int64_t, int64_t>& field(char variable) {
        switch (variable) {
            case 'x': return x;
            case 'm': return m;
            case 'a': return a;
            case 's': return s;
        }
    }

    Ranges accept(char variable, char cmp, int64_t value) const {
        assert(cmp=='<' || cmp=='>');

        Ranges result(*this);
        auto& f = result.field(variable);
        if (cmp == '<') f.second = std::min(f.second, value - 1);
        else f.first = std::max(f.first, value + 1);
        return result;
    }

    Ranges reject(char variable, char cmp, int64_t value) const {
        assert(cmp=='<' || cmp=='>');

        Ranges result(*this);
        auto& f = result.field(variable);
        if (cmp == '<') f.first = std::max(f.first, value);
        else f.second = std::min(f.second, value);
        return result;
    }
};

struct Counter {
private:
    std::map<std::string, std::vector<Pred>> predicates;

public:
    Counter(std::map<std::string, std::vector<Pred>> predicates): predicates(predicates) {
    }

    bool is_accepted(const Register& reg) const {
        std::string current = "in";

        while (true) {
            assert(predicates.count(current));
            for (const auto& pred: predicates.find(current)->second) {
                std::string output;
                if (pred.always_true) {
                    output = pred.output;
                } else {
                    assert(pred.cmp=='<' || pred.cmp=='>');
                    if (((pred.cmp == '<' && reg.value(pred.variable) < pred.value)) || ((
                            pred.cmp == '>' && reg.value(pred.variable) > pred.value)))
                        output = pred.output;
                }

                if (!output.empty()) {
                    if (pred.output == "R" || pred.output == "A") {
                        return pred.output == "A";
                    } else {
                        current = pred.output;
                        break;
                    }
                }
            }
        }
    };

    int64_t count_predicate(const std::vector<Pred>& conditions, int64_t idx, const Ranges& ranges) const {
        const auto& pred = conditions[idx];
        if (pred.always_true) {
            return count_accepted(pred.output, ranges);
        }

        int64_t result = 0;
        // accept;
        result += count_accepted(pred.output, ranges.accept(pred.variable, pred.cmp, pred.value));

        // reject
        result += count_predicate(conditions, idx + 1, ranges.reject(pred.variable, pred.cmp, pred.value));

        return result;
    }

    int64_t count_accepted(std::string current, const Ranges& ranges) const {
        if (ranges.empty() || current == "R")
            return 0;

        if (current == "A")
            return ranges.count();

        assert(predicates.count(current));
        return count_predicate(predicates.find(current)->second, 0, ranges);
    }
};


int main() {
    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");

    /*
        rq{a<3448:rdq,A}
        nsx{a>318:fvb,jk}
        dqd{m<3010:jzr,x>3543:hrh,s<2149:qmn,cl}
     */
    static std::regex base_regex("([a-z]+)\\{([^\\}]+)\\}");
    std::smatch match;

    std::map<std::string, std::vector<Pred>> predicates;
    std::string line;
    while (std::getline(ifs, line) && !line.empty()) {
        if (std::regex_match(line, match, base_regex)) {
            // std::cout << match[1].str() << " : ";
            std::vector<Pred> preds;
            for (const auto& elem: split(match[2].str(), ',')) {
                auto cmp = elem.find_first_of("<>");
                if (cmp != std::string::npos) {
                    auto colon_pos = elem.find(":");
                    preds.push_back(Pred{
                        .always_true = false, .variable = elem.front(), .cmp = elem[cmp],
                        .value = std::stoll(elem.substr(cmp + 1, colon_pos - cmp - 1)),
                        .output = elem.substr(colon_pos + 1)
                    });

                    // std::cout << "(" << preds.back().variable << preds.back().cmp << preds.back().value << " -- " <<
                    //         preds.back().output << "), ";
                } else {
                    preds.push_back(Pred{.always_true = true, .output = elem});
                    // std::cout << "(" << preds.back().output << "), ";
                }
            }
            // std::cout << std::endl;
            predicates[match[1].str()] = preds;
        } else
            assert(false);
    }


    std::vector<Register> registers;
    static std::regex reg_regex("\\{x=(\\d+),m=(\\d+),a=(\\d+),s=(\\d+)\\}");
    std::smatch reg_match;
    // {x=472,m=796,a=2160,s=343}
    while (std::getline(ifs, line)) {
        if (std::regex_match(line, reg_match, reg_regex)) {
            registers.push_back(Register{
                .x = std::stoll(reg_match[1].str()), .m = std::stoll(reg_match[2].str()),
                .a = std::stoll(reg_match[3].str()), .s = std::stoll(reg_match[4].str())
            });
        } else
            assert(false);
    }

    Counter counter(predicates);
    int64_t result = 0;
    for (const auto& reg: registers) {
        if (counter.is_accepted(reg)) {
            result += reg.x + reg.m + reg.a + reg.s;
        }
    }

    std::cout << "part1: " << result << std::endl;


    Ranges range = {.x = {1, 4000}, .m = {1, 4000}, .a = {1, 4000}, .s = {1, 4000}};
    std::cout << "part2: " << counter.count_accepted("in", range) << std::endl;

    return 0;
}
