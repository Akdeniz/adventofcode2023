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

std::vector<std::string> split(const std::string &str, char c = ' ') {
    std::istringstream split(str);
    std::vector<std::string> tokens;
    for (std::string each; std::getline(split, each, c); tokens.push_back(each));
    return tokens;
}

std::vector<int64_t> connected(const std::map<std::string, std::set<std::string>>& graph) {

    std::set<std::string> nodes;
    for(const auto&[key, value]: graph)
        nodes.insert(key);

    std::vector<int64_t> results;

    std::set<std::string> visited;
    for(const auto& node: nodes) {
        if(visited.count(node))
            continue;

        int64_t count = 0;
        std::list<std::string> q = {node};
        while(!q.empty()) {
            auto current = q.front();
            q.pop_front();
            if(visited.count(current)){
                continue;
            }
            visited.insert(current);
            count++;

            for(const auto& next: graph.find(current)->second) {
                if(visited.count(next)==0){
                    q.push_back(next);
                }
            }
        }
        results.push_back(count);
    }

    return results;
}

int main() {
    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");

    std::string line;

    std::map<std::string, std::set<std::string>> graph;
    while (std::getline(ifs, line)) {
        auto pos = line.find(":");
        std::string from = line.substr(0, pos);

        for(const auto& to: split(line.substr(pos+1))) {
            if(!to.empty()) {
                graph[from].insert(to);
                graph[to].insert(from);
            }
        }
    }

    {
        std::set<std::pair<std::string, std::string>> added;
        std::ofstream ofs("graph.dot");
        ofs << "digraph graphname {" << std::endl;
        for (const auto&[from, destinations]: graph) {
            for(const auto& to: destinations) {
                if(added.insert({std::min(from, to), std::max(from, to)}).second)
                    ofs << from << " -> " << to << ";" << std::endl;
            }
        }
        ofs << "}" << std::endl;
    }


    std::vector<std::pair<std::string, std::string>> tobedeleted = {{"fpg", "ldl"}, {"hcf", "lhn"}, {"dfk", "nxk"}};
    for(const auto elem: tobedeleted){
        graph[elem.first].erase(elem.second);
        graph[elem.second].erase(elem.first);
    }

    auto result = connected(graph);
    assert(result.size()==2);
    std::cout << "part1: " << result[0]*result[1] << std::endl;

    return 0;
}
