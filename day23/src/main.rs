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

#[derive(Debug, PartialEq, Eq)]
struct Point {
    x: i64, y: i64,z: i64
}

impl PartialOrd for Point {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        self.x.partial_cmp(&other.x).and_then(self.y.partial_cmp(&other.y)).and_then(self.z.partial_cmp(&other.z))
    }
}

impl Ord for Point {
    fn cmp(&self, other: &Self) -> Ordering {
        self.x.cmp(&other.x).then(self.y.cmp(&other.y)).then_with(self.z.cmp(&other.z))
    }
}


struct Brick {
    s: Point, e: Point
}

fn main() {

    let LINE_RE = Regex::new(r#"(\d+),(\d+),(\d+)~(\d+),(\d+),(\d+)"#).unwrap();

    let mut bricks = vec![];
    for line in read() {
        println!("{}", line);
        let (_, [x1, y1, z1, x2, y2, z2]) = LINE_RE.captures(&line).unwrap().extract();
        let start = Point{x:x1.parse::<i64>().unwrap(),y:y1.parse::<i64>().unwrap(),z:z1.parse::<i64>().unwrap()};
        let end = Point{x:x2.parse::<i64>().unwrap(),y:y2.parse::<i64>().unwrap(),z:z2.parse::<i64>().unwrap()};
        bricks.push(Brick{s:start, e:end});
    }

    bricks.sort();

    let mut point_to_idx = HashMap::new();



    println!("part1 {:?}", bricks.len());
}
