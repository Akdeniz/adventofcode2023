#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <sstream>
#include  <regex>

std::vector<std::string> split(const std::string &str, char c = ' ') {
    std::istringstream split(str);
    std::vector<std::string> tokens;
    for (std::string each; std::getline(split, each, c); tokens.push_back(each));
    return tokens;
}


struct Result{
    int game_number = 0;
    int red = 0;
    int green = 0;
    int blue = 0;
};

auto find(const std::string& line) {

    auto pos = line.find(':');
    assert(pos!=std::string::npos);

    Result result;

    result.game_number = std::stoi(line.substr(line.find_first_of("0123456789"), pos));

    for(const auto& picks: split(line.substr(pos+1), ';')) {

        static std::regex base_regex("(\\d+) (red|green|blue)");

        for (auto it = std::sregex_iterator(picks.begin(), picks.end(), base_regex);
             it != std::sregex_iterator(); ++it) {

            auto count = std::stoi(it->operator[](1).str());
            auto color = it->operator[](2).str();
            if(color=="red") result.red = std::max(result.red, count);
            if(color=="blue") result.blue = std::max(result.blue, count);
            if(color=="green") result.green = std::max(result.green, count);
        }

    }

    return result;
}

int main() {

    {
        std::fstream ifs(std::string(SRC_DIR)+"/input.txt");
        std::string line;

        int64_t total = 0;
        while(std::getline(ifs, line))
        {
            auto result = find(line);
            if(result.red <= 12 && result.green <= 13 && result.blue <= 14)
                total+=result.game_number;
        }

        std::cout << "Part1: " << total << std::endl;
    }

    {
        std::fstream ifs(std::string(SRC_DIR)+"/input.txt");
        std::string line;

        int64_t total = 0;
        while(std::getline(ifs, line))
        {
            auto result = find(line);
            int64_t power = 1;
            if(result.red> 0) power*=result.red;
            if(result.green> 0) power*=result.green;
            if(result.blue> 0) power*=result.blue;
            total+=power;
        }

        std::cout << "Part2: " << total << std::endl;
    }

    return 0;
}
