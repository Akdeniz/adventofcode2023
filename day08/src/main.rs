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
    let file_path = "input.txt";
    let Ok(lines) = read_lines(file_path) else {
        return vec!();
    };

    return lines.into_iter().map(|x| x.unwrap()).collect();
}


fn part1(directions: &str, paths: &HashMap<&str,(&str,&str)>) {

    let mut current = "AAA";
    let mut count = 0i64;

    while current!="ZZZ" {
        let dir = directions.chars().nth(count as usize%directions.len()).unwrap();
        match dir {
            'L' => current = paths[current].0,
            'R' => current = paths[current].1,
            _ => { panic!(); }
        }
        count+=1;
    }

    println!("part1 {}", count);
}

fn part2(directions: &str, paths: &HashMap<&str,(&str,&str)>) {

    let starting_points = paths.iter().map(|(&x, _)| x).filter(|&x| x.chars().nth_back(0).unwrap()=='A').collect::<Vec<_>>();

    let cycles = starting_points.iter().map(|&x| {
        let mut count = 0u64;
        let mut current= x;
        while current.chars().nth_back(0).unwrap()!='Z' {
            let dir = directions.chars().nth(count as usize%directions.len()).unwrap();
            count+=1;
            match dir {
                'L' => current = paths[current].0,
                'R' => current = paths[current].1,
                _ => { panic!(); }
            }
        }
        count
    }).collect::<Vec<_>>();

    let mut result = 1u64;
    for cycle in cycles {
        result = (result*cycle)/binary_u64(result, cycle);
    }

    println!("part2 {}", result);
}


fn main() {

    let lines = read();

    let mut lines_iter = lines.iter();
    let directions = lines_iter.next().unwrap();


    // GXT = (MQM, CHN)
    let path_re = Regex::new(r#"(?P<from>[0-9A-Z]{3}) = \((?P<left>[0-9A-Z]{3}), (?P<right>[0-9A-Z]{3})\)"#).unwrap();

    let mut paths = HashMap::new();
    for line in lines_iter {
        if let Some(cap) = path_re.captures(line) {
            let (_, [from, left, right]) = cap.extract();
            paths.insert(from, (left, right));
        }
    }





    part1(&directions, &paths);
    part2(&directions, &paths);
}
