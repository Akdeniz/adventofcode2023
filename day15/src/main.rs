use gcd::binary_u64;
use lazy_static::lazy_static;
use regex::{Match, Regex};
use std::collections::{HashMap, HashSet};
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

fn hash(input: &str) -> usize {
    let mut result = 0usize;
    for ch in input.chars() {
        result += ch as usize;
        result *= 17;
        result %= 256;
    }
    result
}

fn main() {
    let line = read().join("");
    let table = line.split(",").collect::<Vec<_>>();

    println!("part1 {:?}", table.iter().map(|&x| hash(x)).sum::<usize>());

    let mut boxes: Vec<Vec<(&str, i64)>> = vec![vec![]; 256];

    for elem in table {
        if let Some(pos) = elem.find(|x| x == '-') {
            let key = &elem[0..pos];
            let box_idx = hash(key);
            if let Some(idx) = boxes[box_idx].iter().position(|&(x, _)| x == key) {
                boxes[box_idx].remove(idx);
            }
        } else if let Some(pos) = elem.find(|x| x == '=') {
            let key = &elem[0..pos];
            let focal = elem[pos + 1..].parse::<i64>().unwrap();
            let box_idx = hash(key);
            if let Some(idx) = boxes[box_idx].iter().position(|&(x, _)| x == key) {
                boxes[box_idx][idx].1 = focal;
            } else {
                boxes[box_idx].push((key, focal));
            }
        }
    }

    let mut result = 0i64;
    for (box_idx, boxx) in boxes.iter().enumerate() {
        for (slot_idx, &(_, focal)) in boxx.iter().enumerate() {
            result += (box_idx + 1) as i64 * (slot_idx + 1) as i64 * focal;
        }
    }

    println!("part2 {:?}", result);
}
