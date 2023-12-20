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

struct Module {
    enum Type {
        Conjuction,
        FlipFlop,
        None
    } type;

    std::string name;
    std::vector<std::string> destinations;
    bool is_on = false;
    std::map<std::string, bool> input_pulses; // true is high

    bool all_inputs_are_high() {
        for (const auto& [_, is_high]: input_pulses) {
            if (!is_high)
                return false;
        }
        return true;
    }
};

struct Pulse {
    std::string from, to;
    bool is_high;
};


void execute(std::map<std::string, Module>& modules, int64_t& low_pulse_cnt, int64_t& high_pulse_cnt,
             bool part2 = false) {
    std::list<Pulse> pulses = {Pulse{.from = "button", .to = "broadcaster", .is_high = false}};
    while (!pulses.empty()) {
        const auto pulse = pulses.front();
        pulses.pop_front();

        if (pulse.is_high)
            ++high_pulse_cnt;
        else
            ++low_pulse_cnt;

        assert(modules.count(pulse.to));
        auto& module = modules[pulse.to];

        switch (module.type) {
            case Module::None: {
                for (const auto& dest: module.destinations) {
                    pulses.push_back(Pulse{.from = pulse.to, .to = dest, .is_high = pulse.is_high});
                }
                break;
            }
            case Module::FlipFlop: {
                // flip flops are off at the beginning.
                // high pulses are ignored.
                // if low pulse, invert the on/off and off->on sends high pulse, on->off sends low pulse
                if (!pulse.is_high) {
                    module.is_on = !module.is_on;

                    for (const auto& dest: module.destinations) {
                        pulses.push_back(Pulse{.from = pulse.to, .to = dest, .is_high = module.is_on});
                    }
                }
                break;
            }
            case Module::Conjuction: {
                // conjuction modules are connected from its input modules
                // all input modules are remembered as low pulse at start
                // when pulse is received, update the remembered pulse for that input
                // then if all remembered are high pulse, it sends a low pulse
                // otherwise it send high pulse

                module.input_pulses[pulse.from] = pulse.is_high;
                bool all_high = module.all_inputs_are_high();

                for (const auto& dest: module.destinations) {
                    pulses.push_back(Pulse{.from = pulse.to, .to = dest, .is_high = !all_high});
                }
                break;
            }
        }
    }
}


bool perform(std::map<std::string, Module>& modules, const std::string& start, const std::string& end) {
    std::list<Pulse> pulses = {Pulse{.from = "broadcaster", .to = start, .is_high = false}};
    static int64_t count = 0;
    ++count;

    bool result = false;
    while (!pulses.empty()) {
        const auto pulse = pulses.front();
        pulses.pop_front();

        if (pulse.to == end) {
            result |= !pulse.is_high;
            continue;
        }

        assert(modules.count(pulse.to));
        auto& module = modules[pulse.to];

        switch (module.type) {
            case Module::None: {
                for (const auto& dest: module.destinations) {
                    pulses.push_back(Pulse{.from = pulse.to, .to = dest, .is_high = pulse.is_high});
                }
                break;
            }
            case Module::FlipFlop: {
                // flip flops are off at the beginning.
                // high pulses are ignored.
                // if low pulse, invert the on/off and off->on sends high pulse, on->off sends low pulse
                if (!pulse.is_high) {
                    module.is_on = !module.is_on;

                    for (const auto& dest: module.destinations) {
                        pulses.push_back(Pulse{.from = pulse.to, .to = dest, .is_high = module.is_on});
                    }
                }
                break;
            }
            case Module::Conjuction: {
                // conjuction modules are connected from its input modules
                // all input modules are remembered as low pulse at start
                // when pulse is received, update the remembered pulse for that input
                // then if all remembered are high pulse, it sends a low pulse
                // otherwise it send high pulse


                module.input_pulses[pulse.from] = pulse.is_high;
                bool all_high = module.all_inputs_are_high();

                // if(module.name=="nl" && all_high) {
                //     std::cout << pulse.from << " : " << count << std::endl;
                // }

                for (const auto& dest: module.destinations) {
                    pulses.push_back(Pulse{.from = pulse.to, .to = dest, .is_high = !all_high});
                }
                break;
            }
        }
    }
    return result;
}

int main() {
    std::fstream ifs(std::string(SRC_DIR) + "/input.txt");

    /*
    broadcaster -> a, b, c
    %a -> b
    %b -> c
    %c -> inv
    &inv -> a
     */
    static std::regex base_regex("((%|&)?)([a-z]+) -> ([a-z ,]+)");
    static std::regex name_regex("([a-z]+)");
    std::smatch match, name_match;

    std::map<std::string, Module> modules;
    std::string line;
    while (std::getline(ifs, line) && !line.empty()) {
        if (std::regex_match(line, match, base_regex)) {
            auto type_str = match[2].str();
            auto name_str = match[3].str();
            auto dest_str = match[4].str();

            std::vector<std::string> destinations;
            for (auto it = std::sregex_iterator(dest_str.begin(), dest_str.end(), name_regex);
                 it != std::sregex_iterator(); ++it) {
                destinations.push_back(it->operator[](1).str());
            }

            modules[name_str] = Module{
                .name = name_str, .destinations = destinations,
                .type = (type_str == "%" ? Module::FlipFlop : (type_str == "&" ? Module::Conjuction : Module::None))
            };
        } else
            assert(false);
    }

    modules["button"] = Module{.name = "button", .destinations = {"broadcaster"}, .type = Module::None};

    {
        std::ofstream ofs("graph.dot");
        ofs << "digraph graphname {" << std::endl;
        for (const auto& [_, module]: modules) {
            if (module.type == Module::Conjuction) {
                ofs << module.name << " [shape=box];" << std::endl;
            } else if (module.type == Module::FlipFlop) {
                ofs << module.name << " [shape=triangle];" << std::endl;
            }

            for (const auto& d: module.destinations)
                ofs << module.name << " -> " << d << ";" << std::endl;
        }
        ofs << "}" << std::endl;
    }

    for (const auto& [_, module]: modules) {
        for (const auto& d: module.destinations) {
            modules[d].input_pulses[module.name] = false;
        }
    } {
        auto modules_cpy = modules;
        int64_t low_pulse_cnt = 0;
        int64_t high_pulse_cnt = 0;

        for (int i = 0; i < 1000; ++i) {
            execute(modules_cpy, low_pulse_cnt, high_pulse_cnt);
        }
        std::cout << "part1: " << low_pulse_cnt * high_pulse_cnt << std::endl;
    }

    auto find = [](auto modules, auto start, auto end) {
        int64_t result = 1;
        while (!perform(modules, start, end)) {
            ++result;
        }
        return result;
    };

    int64_t result = find(modules, "db", "mr");
    result *= find(modules, "cm", "bb");
    result *= find(modules, "hd", "gl");
    result *= find(modules, "xf", "kk");
    std::cout << "part2: " << result << std::endl;

    return 0;
}
