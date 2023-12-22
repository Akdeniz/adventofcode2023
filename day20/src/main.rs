use gcd::binary_u64;
use lazy_static::lazy_static;
use regex::{Match, Regex};
use std::cmp::Ordering;
use std::collections::{BinaryHeap, HashMap, HashSet, VecDeque};
use std::fs::File;
use std::io::{self, BufRead};
use std::iter::Map;
use std::path::Path;
use std::slice::Iter;
use std::vec;

fn read_file<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where
    P: AsRef<Path>,
{
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn read() -> Vec<String> {
    let Ok(lines) = read_file("input.txt") else {
        panic!("file is not found");
    };

    return lines.into_iter().map(|x| x.unwrap()).collect();
}

enum ModuleType{Conjuction, FlipFlop, None}
struct Module<'a> {
    typ: ModuleType,
    destinations: Vec<&'a str>,
    is_on: bool,
    input_pulses: HashMap<&'a str, bool>
}

impl Module<'_> {
    fn all_inputs_are_high(&self) -> bool {
        self.input_pulses.iter().all(|(_, &x)| x)
    }
}

struct Pulse<'a> {
    from: &'a str,
    to: &'a str,
    is_high: bool
}

fn execute(modules: &mut HashMap<&str, Module>) -> (i64, i64) {
    let mut pulses = VecDeque::new();
    pulses.push_back(Pulse{from:"button", to:"broadcaster", is_high: false});

    let mut low_pulse_cnt = 0i64;
    let mut high_pulse_cnt = 0i64;

    while let Some(pulse) = pulses.pop_front() {

        //println!("{} -{}-> {}", pulse.from, if pulse.is_high {"high"} else {"low"}, pulse.to);
        if pulse.is_high {
            high_pulse_cnt+=1;
        } else {
            low_pulse_cnt+=1;
        }

        let module = modules.get_mut(pulse.to).unwrap();

        match module.typ {
            ModuleType::None => {
                for &dest in module.destinations.iter() {
                    pulses.push_back(Pulse{from: pulse.to, to: dest, is_high: pulse.is_high});
                }
            }
            ModuleType::FlipFlop => {
                if !pulse.is_high {
                    module.is_on = !module.is_on;

                    for &dest in module.destinations.iter() {
                        pulses.push_back(Pulse{from: pulse.to, to : dest, is_high: module.is_on});
                    }
                }
            }
            ModuleType::Conjuction => {
                module.input_pulses.insert(pulse.from, pulse.is_high);
                let all_high = module.all_inputs_are_high();

                for &dest in module.destinations.iter() {
                    pulses.push_back(Pulse{from: pulse.to, to: dest, is_high: !all_high});
                }
            }
        }
    }

    (low_pulse_cnt, high_pulse_cnt)
}

fn main() {
    let table = read();

    lazy_static! {
        static ref LINE_RE: Regex = Regex::new(r#"(?P<type>((%|&)?))(?P<name>[a-z]+) -> (?P<destionations>[a-z ,]+)"#).unwrap();
        static ref NAME_RE: Regex = Regex::new(r#"(?P<name>[a-z]+)"#).unwrap();
    }

    let mut modules = HashMap::new();
    for line in table.iter() {
        let captures = LINE_RE.captures(&*line).unwrap();
        let typ = captures.name("type").unwrap().as_str();
        let name = captures.name("name").unwrap().as_str();
        let destionations_str = captures.name("destionations").unwrap().as_str();

        let mut module = Module{
            typ: if typ.is_empty() { ModuleType::None } else if typ.eq("%") { ModuleType::FlipFlop } else { ModuleType::Conjuction } ,
            destinations: vec![],
            is_on: false,
            input_pulses: HashMap::new(),
        };
        for capture in NAME_RE.captures_iter(destionations_str) {
            let (_, [name]) = capture.extract();
            module.destinations.push(name);
        }

        modules.insert(name, module);
    }

    modules.insert("button", Module{
        typ: ModuleType::None,
        destinations: vec!["broadcaster"],
        is_on: false,
        input_pulses: HashMap::new(),
    });

    let mut only_dest = vec![];
    let mut input_pulses = vec![];
    for (&name, module) in modules.iter() {
        for &dest in module.destinations.iter() {
            only_dest.push(dest);
            input_pulses.push((dest, name));
        }
    }

    for name in only_dest {
        if !modules.contains_key(name) {
            modules.insert(name, Module{
                typ: ModuleType::None,
                destinations: vec![],
                is_on: false,
                input_pulses: HashMap::new(),
            });
        }
    }

    for (to, from) in input_pulses {
        let module = modules.get_mut(to).unwrap();
        module.input_pulses.insert(from, false);
    }

    let mut low_total = 0;
    let mut high_total = 0;
    for _ in 0..1000 {
        let (low, high) = execute(&mut modules);
        low_total+=low;
        high_total+=high;
    }

    println!("part1 {:?}", low_total*high_total);
}
