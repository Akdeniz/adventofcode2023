use std::collections::HashSet;
use lazy_static::lazy_static;
use regex::{Match, Regex};
use std::fs::File;
use std::io::{self, BufRead};
use std::iter::Map;
use std::path::Path;
use std::slice::Iter;

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

fn find_integers(input: &str) -> HashSet<i64> {

    lazy_static! {
        static ref NUMBER_RE: Regex = Regex::new(r#"(?P<number>[0-9]+)"#).unwrap();
    }

    let mut result: HashSet<i64> = HashSet::new();
    for capture in NUMBER_RE.captures_iter(input) {
        let number = capture.name("number").unwrap().as_str().parse::<i64>().unwrap();
        result.insert(number);
    }
    result
}

fn part1() {

    let mut total = 0i64;
    for line in read() {

        let start = line.find(':').unwrap();
        let delimiter = line.find('|').unwrap();

        let winning_numbers = find_integers(&line[start+1..delimiter]);
        let card_numbers = find_integers(&line[delimiter+1..]);

        let matched = winning_numbers.intersection(&card_numbers).count();
        if matched > 0 {
            total += 1 << (matched - 1);
        }
    }

    println!("part1 {}", total);
}

fn part2() {

    let mut matching_numbers: Vec<i64> = vec![];
    for line in read() {

        let start = line.find(':').unwrap();
        let delimiter = line.find('|').unwrap();

        let winning_numbers = find_integers(&line[start+1..delimiter]);
        let card_numbers = find_integers(&line[delimiter+1..]);

        let matched = winning_numbers.intersection(&card_numbers).count();
        matching_numbers.push(matched as i64);
    }

    let mut total_cards: Vec<i64> = vec![1; matching_numbers.len()];

    for idx in 0..total_cards.len() {
        let count = total_cards[idx];
        let matched = matching_numbers[idx] as usize;
        for j in idx+1..idx+1+matched {
            total_cards[j]+=count;
        }
    }

    let total: i64 = total_cards.iter().sum();
    println!("part2 {}", total);
}

fn main() {
    part1();
    part2();
}
