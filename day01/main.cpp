#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <numeric>

auto find(const std::string& line, bool check_words=false) {

    static const std::vector<char> chars = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    static const std::vector<std::string> words = {"zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};

    int64_t first = line.size();
    int64_t first_value = 0;
    int64_t last = -1;
    int64_t last_value = 0;

    for(int idx=0; idx < chars.size(); ++idx) {
        int64_t pos = line.find(chars[idx]);
        if (pos!=std::string::npos && pos < first) {
            first = pos;
            first_value = idx;
        }

        pos = line.rfind(chars[idx]);
        if (pos!=std::string::npos && last < pos)
        {
            last = pos;
            last_value = idx;
        }

        if(check_words) {
            int64_t pos = line.find(words[idx]);
            if (pos!=std::string::npos && pos < first) {
                first = pos;
                first_value = idx;
            }

            pos = line.rfind(words[idx]);
            if (pos!=std::string::npos && last < pos)
            {
                last = pos;
                last_value = idx;
            }
        }
    }

    //std::cout << line << " -- " << first_value << ", " << last_value << std::endl;
    return std::make_pair<>(first_value, last_value);
}


int main() {

    {
        std::fstream ifs(std::string(SRC_DIR)+"/input.txt");
        std::string line;

        int64_t total = 0;
        while(std::getline(ifs, line))
        {
            auto first_last = find(line);
            total += (first_last.first*10 + first_last.second);
        }

        std::cout << "Part1: " << total << std::endl;
    }

    {
        std::fstream ifs(std::string(SRC_DIR)+"/input.txt");
        std::string line;

        int64_t total = 0;
        while(std::getline(ifs, line))
        {
            auto first_last = find(line, true);
            total += (first_last.first*10 + first_last.second);
        }


        std::cout << "Part2: " << total << std::endl;
    }

    return 0;
}
