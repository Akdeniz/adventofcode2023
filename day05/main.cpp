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

std::vector<int64_t> find_integers(const std::string& str) {
    static std::regex base_regex("(\\d+)");

    std::vector<int64_t> result;
    for (auto it = std::sregex_iterator(str.begin(), str.end(), base_regex);
         it != std::sregex_iterator(); ++it) {

        auto value = std::stoll(it->operator[](1).str());
        result.push_back(value);
    }
    return result;
}

struct Range {
    int64_t destination_start;
    int64_t source_start;
    int64_t length;

    int64_t source_end() const { return source_start+length; }
    int64_t destination_end() const { return destination_start+length; }


};

std::vector<Range> read_ranges(std::fstream &ifs, const std::string& until){
    std::vector<Range> result;
    std::string line;
    while(std::getline(ifs, line) && line!=until){
        auto range = find_integers(line);
        assert(range.size()==0 || range.size()==3);
        if(range.size()==3) {
            result.push_back(Range{.destination_start=range[0], .source_start=range[1], .length=range[2]});
        }
    }
    return result;
}

void part1(const std::vector<int64_t>& seeds, const std::vector<std::vector<Range>>& conversions) {


    std::optional<int64_t> smallest_location;

    for(auto seed: seeds) {

        auto current = seed;
        for(const auto& conversion: conversions) {

            for(const auto& range: conversion) {
                if(range.source_start <= current && current < range.source_start+range.length) {
                    current = current-range.source_start+range.destination_start;
                    break;
                }
            }
        }

        smallest_location = std::min(smallest_location.value_or(current), current);
    }

    std::cout << "part1 : " << *smallest_location << std::endl;
}

int64_t smallest_mapping(const std::pair<int64_t, int64_t>& range, const std::vector<std::vector<Range>>& conversions, int conversion_idx){

    if(conversion_idx >= conversions.size()) {
        return range.first;
    }

    std::optional<int64_t> smallest_value;

    std::vector<std::pair<int64_t, int64_t>> not_mapped_ranges = {range};
    for (const auto &mapping: conversions[conversion_idx]) {

        // M |---------|           |-------|          |-----------|
        // R       |-------|   |-------|                  |----|

        std::vector<std::pair<int64_t, int64_t>> new_ranges;
        for(const auto& range: not_mapped_ranges) {

            if (range.second <= mapping.source_start || mapping.source_end() <= range.first) {
                new_ranges.push_back(range);
                continue;
            }

            if (range.first < mapping.source_start) {
                new_ranges.push_back({range.first, mapping.source_start});
            }

            if (range.second > mapping.source_end()) {
                new_ranges.push_back({mapping.source_end(), range.second});
            }

            int64_t overlap_start =
                    std::max(mapping.source_start, range.first) + (mapping.destination_start - mapping.source_start);
            int64_t overlap_end =
                    std::min(mapping.source_end(), range.second) + (mapping.destination_start - mapping.source_start);

            auto value = smallest_mapping({overlap_start, overlap_end}, conversions, conversion_idx + 1);
            smallest_value = std::min(smallest_value.value_or(value), value);
        }
        std::swap(not_mapped_ranges, new_ranges);
    }

    for(const auto& range: not_mapped_ranges) {
        auto value = smallest_mapping(range, conversions, conversion_idx + 1);
        smallest_value = std::min(smallest_value.value_or(value), value);
    }

    assert(smallest_value.has_value());
    return *smallest_value;
}

void part2(const std::vector<int64_t>& seeds, const std::vector<std::vector<Range>>& conversions) {

    std::optional<int64_t> smallest_value;

    for(int i=0; i < seeds.size(); i+=2) {
        auto value = smallest_mapping({seeds[i], seeds[i]+seeds[i+1]},conversions, 0);
        smallest_value = std::min(smallest_value.value_or(value), value);
    }

    std::cout << "part2 : " << *smallest_value << std::endl;
}


int main() {

    std::fstream ifs(std::string(SRC_DIR)+"/input.txt");
    std::string line;

    std::getline(ifs, line);
    auto seeds = find_integers(line);

    while(std::getline(ifs, line) && line!="seed-to-soil map:");

    std::vector<std::vector<Range>> conversions;
    conversions.push_back(read_ranges(ifs, "soil-to-fertilizer map:"));
    conversions.push_back(read_ranges(ifs, "fertilizer-to-water map:"));
    conversions.push_back(read_ranges(ifs, "water-to-light map:"));
    conversions.push_back(read_ranges(ifs, "light-to-temperature map:"));
    conversions.push_back(read_ranges(ifs, "temperature-to-humidity map:"));
    conversions.push_back(read_ranges(ifs, "humidity-to-location map:"));
    conversions.push_back(read_ranges(ifs, "end"));

    part1(seeds, conversions);
    part2(seeds, conversions);
    return 0;
}
