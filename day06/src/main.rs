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

fn part1() {

    let lines = read();
    let times = find_integers(&lines[0]);
    let distances = find_integers(&lines[1]);

    let mut result = 1i64;
    for (idx, &time) in times.iter().enumerate() {

        let mut count = 0i64;
        for i in 1..time {
            if i * (time - i) > distances[idx] {
                count+=1;
            }
        }
        result *= count;
    }

    println!("part1 {}", result);
}

fn part2() {

    let lines = read();
    let times = find_integers(&lines[0].replace(" ", ""));
    let distances = find_integers(&lines[1].replace(" ", ""));

    let mut result = 1i64;
    for (idx, &time) in times.iter().enumerate() {

        let mut count = 0i64;
        for i in 1..time {
            if i * (time - i) > distances[idx] {
                count+=1;
            }
        }
        result *= count;
    }

    println!("part2 {}", result);
}

fn main() {
    part1();
    part2();
}
