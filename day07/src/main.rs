use std::collections::{HashMap, HashSet};
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

fn hand_type(hand: &str) -> Vec<i64> {
    let mut counts : HashMap<char, i64> = HashMap::new();
    for c in hand.chars() { *counts.entry(c).or_insert(0)+=1; }
    let mut counts = counts.iter().map(|x| *x.1).collect::<Vec<_>>();
    counts.sort_by(|a,b| b.cmp(a));
    counts
}

fn hand_level(hand: &str) -> Vec<usize> {
    let order = "23456789TJQKA";
    let mut result= vec![];
    for c in hand.chars() { result.push(order.find(c).unwrap()); }
    result
}

fn part1(mut hands: Vec<(&str, i64)>) {

    hands.sort_by(|&a, &b| {
        let a_type = hand_type(a.0);
        let b_type = hand_type(b.0);

        if a_type==b_type {
            return hand_level(a.0).cmp(&hand_level(b.0));
        }

        return a_type.cmp(&b_type);
    });

    let total: i64 = hands.iter().enumerate().map(|(idx, &hand)| ((idx+1) as i64) * hand.1).sum();

    println!("part1 {}", total);
}

fn hand_type_part2(hand: &str) -> Vec<i64> {
    let mut counts : HashMap<char, i64> = HashMap::new();
    let mut joker_count = 0i64;
    for c in hand.chars() {
        if c == 'J' {
            joker_count+=1;
            continue
        }
        *counts.entry(c).or_insert(0)+=1;
    }
    let mut counts = counts.iter().map(|x| *x.1).collect::<Vec<_>>();
    counts.sort_by(|a,b| b.cmp(a));
    if counts.is_empty() {
        counts.push(joker_count)
    } else {
        counts[0] += joker_count
    }
    counts
}

fn hand_level_part2(hand: &str) -> Vec<usize> {
    let order = "J23456789TQKA";
    let mut result= vec![];
    for c in hand.chars() { result.push(order.find(c).unwrap()); }
    result
}

fn part2(mut hands: Vec<(&str, i64)>) {

    hands.sort_by(|&a, &b| {
        let a_type = crate::hand_type_part2(a.0);
        let b_type = crate::hand_type_part2(b.0);

        if a_type==b_type {
            return crate::hand_level_part2(a.0).cmp(&crate::hand_level_part2(b.0));
        }

        return a_type.cmp(&b_type);
    });

    let total: i64 = hands.iter().enumerate().map(|(idx, &hand)| ((idx+1) as i64) * hand.1).sum();

    println!("part1 {}", total);
}

fn main() {

    let lines = read();
    let data = lines.iter().map(|line| {
        let parts = line.split(' ').collect::<Vec<_>>();
        (parts[0], parts[1].parse::<i64>().unwrap())
    }).collect::<Vec<_>>();

    part1(data.clone());
    part2(data.clone());
}
