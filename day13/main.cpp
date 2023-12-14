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

std::vector<int64_t> find_integers(const std::string &str) {
    static std::regex base_regex("(-?\\d+)");

    std::vector<int64_t> result;
    for (auto it = std::sregex_iterator(str.begin(), str.end(), base_regex);
         it != std::sregex_iterator(); ++it) {

        auto value = std::stoll(it->operator[](1).str());
        result.push_back(value);
         }
    return result;
}

bool is_col_reflection(const std::vector<std::string>& table, const int64_t idx, const bool with_smudge=false) {

    bool smudged = false;
    for(int i=0; i < table.size(); ++i) {
        for(int j=1; idx+j < table[i].size(); ++j) {
            if(idx-(j-1) < 0)
                break;
            if(table[i][idx+j]!=table[i][idx-(j-1)]) {
                if(with_smudge) {
                    if(smudged)
                        return false;
                    smudged = true;
                } else {
                    return false;
                }
            }
        }
    }
    return !with_smudge || smudged;
}

bool is_row_reflection(const std::vector<std::string>& table, int64_t idx, const bool with_smudge=false) {
    bool smudged = false;
    for(int j=0; j < table[0].size(); ++j) {
        for(int i=1; idx+i < table.size(); ++i) {
            if(idx-(i-1) < 0)
                break;
            if(table[idx+i][j]!=table[idx-(i-1)][j]) {
                if(with_smudge) {
                    if(smudged)
                        return false;
                    smudged = true;
                } else {
                    return false;
                }
            }
        }
    }
    return !with_smudge || smudged;
}

std::vector<int64_t> find_row_reflactions(const std::vector<std::string>& table, const bool with_smudge=false) {
    std::vector<int64_t> results;
    for(int i=0; i+1 < table.size(); ++i) {
        if(is_row_reflection(table, i, with_smudge)) {
            results.push_back(i);
        }
    }
    return results;
}

std::vector<int64_t> find_col_reflactions(const std::vector<std::string>& table, const bool with_smudge=false) {
    std::vector<int64_t> results;
    for(int j=0; j+1 < table[0].size(); ++j) {
        if(is_col_reflection(table, j, with_smudge)) {
            results.push_back(j);
        }
    }
    return results;
}

int64_t part1(const std::vector<std::vector<std::string>>& tables) {


    int64_t result = 0;
    for(const auto& table: tables) {

        auto col_reflect = find_col_reflactions(table);
        if(!col_reflect.empty()) {
            assert(col_reflect.size()==1);
            result+=(col_reflect.front()+1);
            continue;
        }
        auto row_reflect = find_row_reflactions(table);
        if(!row_reflect.empty()) {
            assert(row_reflect.size()==1);
            result+=((row_reflect.front()+1)*100);
            continue;
        }

        assert(false);

    }
    return result;
}

int64_t part2(const std::vector<std::vector<std::string>>& tables) {


    int64_t result = 0;
    for(const auto& table: tables) {

        {
            auto reflect = find_col_reflactions(table, true);
            auto original = find_col_reflactions(table);
            for(auto elem: original) {
                auto it = std::find(reflect.begin(), reflect.end(), elem);
                if(it !=reflect.end())
                    reflect.erase(it);
            }
            if(!reflect.empty()) {
                assert(reflect.size()==1);
                result+=(reflect.front()+1);
                continue;
            }
        }

        {
            auto reflect = find_row_reflactions(table, true);
            auto original = find_row_reflactions(table);
            for(auto elem: original) {
                auto it = std::find(reflect.begin(), reflect.end(), elem);
                if(it !=reflect.end())
                    reflect.erase(it);
            }
            if(!reflect.empty()) {
                assert(reflect.size()==1);
                result+=((reflect.front()+1)*100);
                continue;
            }
        }

        assert(false);

    }
    return result;
}


int main() {

    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");
    std::vector<std::vector<std::string>> table;
    table.push_back(std::vector<std::string>{});
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) {
            table.push_back(std::vector<std::string>{});
            continue;
        }
        table.back().push_back(line);
    }



    std::cout << "part1: " << part1(table) << std::endl;
    std::cout << "part2: " << part2(table) << std::endl;
}
