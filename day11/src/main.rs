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

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where
    P: AsRef<Path>,
{
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}

fn read() -> Vec<String> {
    let Ok(lines) = read_lines("input.txt") else {
        panic!("file is not found");
    };

    return lines.into_iter().map(|x| x.unwrap()).collect();
}

fn solve(step: i64) -> i64 {
    let table = read().into_iter().map(|row| row.chars().collect()).collect::<Vec<Vec<char>>>();

    let mut rows :Vec<i64> = vec![];
    for row in table.iter() {
        if row.iter().find(|&ch| *ch=='#').is_some() {
            rows.push(1 + rows.last().unwrap_or(&0));
        } else {
            rows.push(step + rows.last().unwrap_or(&0));
        }
    }

    let mut cols: Vec<i64> = vec![];
    for j in 0..(table[0].len()) {
        if (0..table.len()).map(|i| table[i][j]).find(|&c| c=='#').is_some() {
            cols.push(1 + cols.last().unwrap_or(&0));
        } else {
            cols.push(step + cols.last().unwrap_or(&0));
        }
    }

    let mut points = vec![];
    for (i, row) in table.iter().enumerate() {
        for (j, ch) in row.iter().enumerate() {
            if *ch=='#' {
                points.push((i, j));
            }
        }
    }

    let mut result = 0;
    for i in 0..points.len() {
        for j in (i+1)..points.len() {
            result += (rows[points[i].0]-rows[points[j].0]).abs() + (cols[points[i].1]-cols[points[j].1]).abs();
        }
    }

    result
}

fn main() {

    println!("part1 {}", solve(2));
    println!("part2 {}", solve(1000000));
}
