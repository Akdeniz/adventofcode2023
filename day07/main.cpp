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

void part1(std::vector<std::pair<std::string, int64_t>> hands) {

    auto hand_type = [](const std::string& hand) {
        std::map<char, int> counts;
        for(char c: hand) ++counts[c];
        std::vector<int> type;
        for(const auto& elem: counts)
            type.push_back(elem.second);
        std::sort(type.begin(), type.end(), std::greater<int>());
        return type;
    };

    auto hand_level = [](const std::string& hand) {
        static const std::string order = "23456789TJQKA";
        std::vector<int> result;
        for(char c: hand) {
            auto pos = order.find(c);
            assert(pos!=std::string::npos);
            result.push_back(pos);
        }
        return result;
    };


    std::sort(hands.begin(), hands.end(), [&hand_type, &hand_level](const std::pair<std::string, int64_t>& a, const std::pair<std::string, int64_t>& b) {

        auto a_type = hand_type(a.first);
        auto b_type = hand_type(b.first);

        if(a_type==b_type) {
            return hand_level(a.first) < hand_level(b.first);
        }

        return a_type < b_type;
    });

    int64_t total = 0;
    for(int i=0; i < hands.size(); ++i){
        total+=(i+1)*hands[i].second;
    }

    std::cout << "Part1: " << total << std::endl;
}

void part2(std::vector<std::pair<std::string, int64_t>> hands) {

    auto hand_type = [](const std::string& hand) {
        std::map<char, int> counts;
        int joker_count = 0;
        for(char c: hand) {
            if(c=='J') {
                ++joker_count;
                continue;
            }
            ++counts[c];
        }
        std::vector<int> type;
        for(const auto& elem: counts) {
            type.push_back(elem.second);
        }
        std::sort(type.begin(), type.end(), std::greater<int>());
        if(!type.empty()) {
            type[0]+=joker_count;
        } else {
            type.push_back(joker_count);
        }
        return type;
    };

    auto hand_level = [](const std::string& hand) {
        static const std::string order = "J23456789TQKA";
        std::vector<int> result;
        for(char c: hand) {
            auto pos = order.find(c);
            assert(pos!=std::string::npos);
            result.push_back(pos);
        }
        return result;
    };


    std::sort(hands.begin(), hands.end(), [&hand_level, &hand_type](const std::pair<std::string, int64_t>& a, const std::pair<std::string, int64_t>& b) {

        auto a_type = hand_type(a.first);
        auto b_type = hand_type(b.first);

        if(a_type==b_type) {

            auto a_level = hand_level(a.first);
            auto b_level = hand_level(b.first);
            return a_level < b_level;
        }

        return a_type < b_type;
    });

    int64_t total = 0;
    for(int i=0; i < hands.size(); ++i){
        total+=(i+1)*hands[i].second;
    }

    std::cout << "Part2: " << total << std::endl;
}


int main() {

    std::fstream ifs(std::string(SRC_DIR)+"/input.txt");
    std::string line;

    std::vector<std::pair<std::string, int64_t>> hands;
    while(std::getline(ifs, line))
    {
        auto pos = line.find(" ");
        auto hand = line.substr(0, pos);
        int64_t bid = std::stoll(line.substr(pos+1));
        hands.push_back({hand, bid});
    }

    part1(hands);
    part2(hands);
    return 0;
}
