use std::collections::HashSet;
use lazy_static::lazy_static;
use regex::{Match, Regex};
use std::fs::File;
use std::io::{self, BufRead};
use std::iter::Map;
use std::path::Path;
use std::slice::Iter;
use std::vec;

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where
    P: AsRef<Path>,
{
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn read() -> Vec<String> {
    let file_path = "input.txt";
    let Ok(lines) = read_lines(file_path) else {
        return vec!();
    };

    return lines.into_iter().map(|x| x.unwrap()).collect();
}

fn find_integers(input: &str) -> Vec<i64> {

    lazy_static! {
        static ref NUMBER_RE: Regex = Regex::new(r#"(?P<number>[0-9]+)"#).unwrap();
    }

    let mut result: Vec<i64> = vec![];
    for capture in NUMBER_RE.captures_iter(input) {
        let number = capture.name("number").unwrap().as_str().parse::<i64>().unwrap();
        result.push(number);
    }
    result
}

fn part1(seeds: &Vec<i64>, conversions: &Vec<Vec<Mapping>>) {

    let mut smallest: Option<i64> = None;

    for seed in seeds {
        let mut current = *seed;

        for conversion in conversions {
            for mapping in conversion {
                if mapping.src <= current && current < mapping.src+mapping.len {
                    current = current-mapping.src+mapping.dst;
                    break;
                }
            }
        }

        smallest = Option::from(current.min(smallest.unwrap_or(current)));
    }

    println!("part1 {}", smallest.unwrap());
}

fn apply_conversion(ranges:&Vec<(i64, i64)>, conversion: &Vec<Mapping>) -> Vec<(i64, i64)> {


    let mut result: Vec<(i64, i64)> = vec![];

    let mut remaining_ranges = ranges.clone();
    for mapping in conversion {

        let mut new_ranges = vec![];
        for (r_start, r_end) in remaining_ranges {

            // M |---------|           |-------|          |-----------|
            // R       |-------|   |-------|                  |----|

            if r_end <= mapping.src || mapping.src+mapping.len <= r_start {
                new_ranges.push((r_start, r_end));
                continue;
            }

            if r_start < mapping.src {
                new_ranges.push((r_start, mapping.src));
            }

            if r_end > mapping.src+mapping.len {
                new_ranges.push((mapping.src+mapping.len, r_end));
            }

            let overlap_start = r_start.max(mapping.src) + (mapping.dst-mapping.src);
            let overlap_end = r_end.min(mapping.src+mapping.len) + (mapping.dst-mapping.src);
            result.push((overlap_start, overlap_end));
        }
        remaining_ranges = new_ranges;
    }
    result.append(&mut remaining_ranges);
    result
}

fn part2(seeds: &Vec<i64>, conversions: &Vec<Vec<Mapping>>) {

    let mut smallest: Option<i64> = None;

    for seed in seeds.chunks(2) {
        let mut ranges = vec![(seed[0], seed[0]+seed[1])];

        for conversion in conversions {
            ranges = apply_conversion(&ranges, conversion)
        }

        let value = ranges.iter().map(|x| x.0).min().unwrap();
        smallest = Option::from(value.min(smallest.unwrap_or(value)));
    }

    println!("part2 {}", smallest.unwrap());
}

struct Mapping {
    dst:i64,
    src:i64,
    len:i64
}

fn get_mappings(lines: &Vec<String>, header: &str) -> Vec<Mapping> {

    let mut result: Vec<Mapping> = vec![];
    let start = lines.iter().position(|x| x==header).unwrap() + 1;
    for idx in start..lines.len() {
        let values = find_integers(&lines[idx]);
        if values.is_empty() {
            break
        }
        result.push(Mapping{dst:values[0], src:values[1], len:values[2]});
    }
    result
}

fn main() {
    let lines = read();
    let seeds = find_integers(&lines[0]);

    let mut levels: Vec<Vec<Mapping>> = vec![];
    levels.push(get_mappings(&lines, "seed-to-soil map:"));
    levels.push(get_mappings(&lines, "soil-to-fertilizer map:"));
    levels.push(get_mappings(&lines, "fertilizer-to-water map:"));
    levels.push(get_mappings(&lines, "water-to-light map:"));
    levels.push(get_mappings(&lines, "light-to-temperature map:"));
    levels.push(get_mappings(&lines, "temperature-to-humidity map:"));
    levels.push(get_mappings(&lines, "humidity-to-location map:"));

    part1(&seeds, &levels);
    part2(&seeds, &levels);
}
