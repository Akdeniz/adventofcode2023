use gcd::binary_u64;
use lazy_static::lazy_static;
use regex::{Match, Regex};
use std::cmp::Ordering;
use std::collections::{BinaryHeap, HashMap, HashSet, VecDeque};
use std::f64::EPSILON;
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


#[derive(Debug, Copy, Clone)]
struct Vec3 {
    x: f64,
    y: f64,
    z: f64,
}

fn intersection(p1: Vec3, v1: Vec3, p2: Vec3, v2: Vec3) -> Option<Vec3> {
    let slope1 = v1.y / v1.x;
    let slope2 = v2.y / v2.x;

    if slope1 != slope2 {
        let x = (-slope2 * p2.x - p1.y + p2.y + slope1 * p1.x) / (slope1 - slope2);
        let y = slope1 * (x - p1.x) + p1.y;
        return Some(Vec3 { x: x, y: y, z: 0f64 });
    }

    None
}

fn is_in_forward(found: Vec3, p: Vec3, v: Vec3) -> bool {
    return !(found.x < p.x && v.x > 0f64 || found.x > p.x && v.x < 0f64 || found.y < p.y && v.y > 0f64 || found.y > p.y && v.y < 0f64);
}

fn main() {
    let line_regex = Regex::new(r#"(\d+),[ ]*(\d+),[ ]*(\d+)[ ]*@[ ]*(-?\d+),[ ]*(-?\d+),[ ]*(-?\d+)"#).unwrap();

    let mut hailstorms = vec![];
    for line in read() {
        let (_, [px, py, pz, vx, vy, vz]) = line_regex.captures(&line).unwrap().extract();
        let pos = Vec3 { x: px.parse::<f64>().unwrap(), y: py.parse::<f64>().unwrap(), z: pz.parse::<f64>().unwrap() };
        let velociy = Vec3 { x: vx.parse::<f64>().unwrap(), y: vy.parse::<f64>().unwrap(), z: vz.parse::<f64>().unwrap() };
        hailstorms.push((pos, velociy));
    }

    let mut result = 0;
    for i in 0..hailstorms.len() {
        for j in (i + 1)..hailstorms.len() {
            let (p1, v1) = hailstorms[i];
            let (p2, v2) = hailstorms[j];

            if let Some(found) = intersection(p1, v1, p2, v2) {
                if [found.x, found.y].iter().all(|&x| x >= 200000000000000f64 && x <= 400000000000000f64) {
                    //if [found.x, found.y].iter().all(|&x| x >= 7f64 && x <= 27f64) {
                    if is_in_forward(found, p1, v1) && is_in_forward(found, p2, v2) {
                        result += 1;
                    }
                }
            }
        }
    }

    println!("part1 {:?}", result);
}
