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

std::set<int64_t> find_integers(const std::string& str) {
    static std::regex base_regex("(\\d+)");

    std::set<int64_t> result;
    for (auto it = std::sregex_iterator(str.begin(), str.end(), base_regex);
         it != std::sregex_iterator(); ++it) {

        auto value = std::stoi(it->operator[](1).str());
        result.insert(value);
    }
    return result;
}

void part1() {

    std::fstream ifs(std::string(SRC_DIR)+"/input.txt");
    std::string line;

    int64_t total = 0;
    while(std::getline(ifs, line)) {

        auto colon = line.find(":");
        auto sep = line.find("|");

        auto winning_numbers = find_integers(line.substr(colon+1, sep-colon));
        auto card_numbers = find_integers(line.substr(sep+1));

        std::vector<int64_t> common;
        std::set_intersection(winning_numbers.begin(), winning_numbers.end(), card_numbers.begin(), card_numbers.end(), std::back_inserter(common));
        if(!common.empty()) {
            total += (1 << (common.size() - 1));
        }
    }

    std::cout << "part1: " << total << std::endl;
}

void part2() {

    std::fstream ifs(std::string(SRC_DIR)+"/input.txt");
    std::string line;

    std::vector<int64_t> matching_numbers;

    while(std::getline(ifs, line)) {

        auto colon = line.find(':');
        auto sep = line.find('|');

        auto winning_numbers = find_integers(line.substr(colon+1, sep-colon));
        auto card_numbers = find_integers(line.substr(sep+1));

        std::vector<int64_t> common;
        std::set_intersection(winning_numbers.begin(), winning_numbers.end(), card_numbers.begin(), card_numbers.end(), std::back_inserter(common));
        matching_numbers.push_back(common.size());
    }



    std::vector<int64_t> total_cards(matching_numbers.size(), 1); // original card

    for(int i=0; i < matching_numbers.size(); ++i) {
        auto count = total_cards[i];
        auto matching = matching_numbers[i];
        for(int j=i+1; j < i+1+matching; ++j) {
            total_cards[j]+=count;
        }
    }

    int64_t total = std::accumulate(total_cards.begin(), total_cards.end(), 0ll);

    std::cout << "part2: " << total << std::endl;
}

int main() {
    part1();
    part2();
    return 0;
}
