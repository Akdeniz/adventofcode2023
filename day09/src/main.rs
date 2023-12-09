use std::collections::{HashMap, HashSet};
use lazy_static::lazy_static;
use regex::{Match, Regex};
use std::fs::File;
use std::io::{self, BufRead};
use std::iter::Map;
use std::path::Path;
use std::slice::Iter;
use std::vec;
use gcd::binary_u64;

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where
    P: AsRef<Path>,
{
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn read() -> Vec<String> {
    let path = Path::new(file!());
    let Ok(lines) = read_lines("input.txt") else {
        panic!("file is not found");
    };

    return lines.into_iter().map(|x| x.unwrap()).collect();
}

fn find_integers(input: &str) -> Vec<i64> {

    lazy_static! {
        static ref NUMBER_RE: Regex = Regex::new(r#"(?P<number>-?[0-9]+)"#).unwrap();
    }

    let mut result: Vec<i64> = vec![];
    for capture in NUMBER_RE.captures_iter(input) {
        let number = capture.name("number").unwrap().as_str().parse::<i64>().unwrap();
        result.push(number);
    }
    result
}


fn solve(backwards: bool) -> i64 {

    let mut result = 0i64;
    for line in read() {
        let mut history = vec![find_integers(&line)];

        while !history.last().unwrap().iter().all(|&x| x==0) {
            let last = history.last().unwrap();
            let mut newstep: Vec<i64> = vec![];
            for i in 1..last.len() {
                newstep.push(last[i]-last[i-1]);
            }
            history.push(newstep);
        }

        let mut prediction = 0i64;
        for i in (0..history.len()-1).rev() {
            if backwards {
                prediction = history[i].first().unwrap() - prediction;
            } else {
                prediction = history[i].last().unwrap() + prediction;
            }
        }
        result+=prediction;
    }
    result
}

fn main() {

    println!("part1 {}", solve(false));
    println!("part2 {}", solve(true));
}
