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

int64_t hash(const std::string& line) {
    int64_t result = 0;
    for (char c: line) {
        result += static_cast<int64_t>(c);
        result *= 17;
        result %= 256;
    }
    return result;
}

void part1(const std::vector<std::string>& lines) {
    int64_t result = 0;
    for (const auto& line: lines) {
        result += hash(line);
    }

    std::cout << "part1: " << result << std::endl;
}

void part2(const std::vector<std::string>& lines) {
    std::vector<std::vector<std::pair<std::string, int64_t>>> boxes(256);
    for (const auto& line: lines) {
        auto pos = line.find('-');
        if (pos != std::string::npos) {
            auto label = line.substr(0, pos);
            std::erase_if(boxes[hash(label)], [&label](const auto& elem) { return elem.first == label; });
        } else {
            auto pos = line.find('=');
            auto label = line.substr(0, pos);
            int64_t focal = std::stoll(line.substr(pos + 1));
            auto& box = boxes[hash(label)];
            auto it = std::find_if(box.begin(), box.end(), [&label](const auto& elem) { return elem.first == label; });
            if (it == box.end()) {
                box.push_back({label, focal});
            } else {
                *it = {label, focal};
            }
        }
    }

    int64_t result = 0;
    for (int64_t box_idx = 0; box_idx < boxes.size(); ++box_idx) {
        const auto& box = boxes[box_idx];
        for (int64_t slot_idx = 0; slot_idx < box.size(); ++slot_idx) {
            int64_t power = (box_idx + 1) * (slot_idx + 1) * box[slot_idx].second;
            result += power;
        }
    }

    std::cout << "part2: " << result << std::endl;
}

int main() {
    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");

    std::string line;
    std::vector<std::string> lines;
    while (std::getline(ifs, line, ',')) {
        std::erase_if(line, [](char c) { return c == '\n'; });
        lines.push_back(line);
    }

    part1(lines);
    part2(lines);

    return 0;
}
