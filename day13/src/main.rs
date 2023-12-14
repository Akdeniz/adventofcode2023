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

fn matching(input: &Vec<char>, mut input_idx: usize, mut group_size: usize) -> bool {
    while input_idx < input.len() && (input[input_idx]=='#'  || input[input_idx]=='?') && group_size > 0 {
        group_size-=1;
        input_idx+=1;
    }
    group_size==0 && (input_idx==input.len() || input[input_idx]!='#')
}


fn solve(input: &Vec<char>, groups: &Vec<i64>, input_idx: usize, groups_idx: usize, memo: &mut HashMap<(usize, usize), i64>) -> i64 {

    let key = (input_idx, groups_idx);
    if let Some((_, &result))  = memo.get_key_value(&key) {
        return result;
    }

    if groups_idx==groups.len() {
        for idx in input_idx..input.len() {
            if input[idx]=='#' {
                memo.insert(key, 0);
                return 0;
            }
        }
        memo.insert(key, 1);
        return 1;
    }

    if input_idx >= input.len() {
        memo.insert(key, 0);
        return 0;
    }

    let result = match input[input_idx] {
        '.' => solve(input, groups, input_idx+1, groups_idx, memo),
        '#' => {
            if matching(input, input_idx, groups[groups_idx] as usize) {
                return solve(input, groups, input_idx +groups[groups_idx] as usize+ 1, groups_idx+1, memo)
            }
            return 0;
        },
        '?' => {
            let mut total = 0;
            if matching(input, input_idx, groups[groups_idx] as usize) {
                total+= solve(input, groups, input_idx +groups[groups_idx] as usize+ 1, groups_idx+1, memo)
            }
            total += solve(input, groups, input_idx + 1, groups_idx, memo);
            return total;
        },
        _ => panic!()
    };
    memo.insert(key, result);
    return result;
}

fn main() {

    let table = read();
    let mut result = 0i64;
    for row in &table {
        let pos = row.find(|x|x==' ').unwrap();
        let mut memo: HashMap<(usize, usize), i64> = HashMap::new();
        result+=solve(&row[0..pos].chars().collect::<Vec<_>>(), &find_integers(&row[pos+1..]), 0, 0, &mut memo);
    }

    println!("part1 {}", result);

    let mut result = 0i64;
    for row in  &table {
        let pos = row.find(|x|x==' ').unwrap();

        let mut input: String = String::new();
        for _ in 0..5 {
            input+=&row[0..pos];
            input+="?";
        }

        let groups = vec![find_integers(&row[pos+1..]); 5].iter().flatten().map(|x|*x).collect::<Vec<_>>();
        let mut memo: HashMap<(usize, usize), i64> = HashMap::new();
        result+=solve(&input.chars().collect(), &groups, 0, 0, &mut memo);
    }


    println!("part2 {}", result);
}
