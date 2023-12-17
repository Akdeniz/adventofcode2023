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

#[derive(Debug, PartialEq, Eq, PartialOrd, Ord, Copy, Clone, Hash)]
enum Dir {
    U,
    D,
    L,
    R,
}

#[derive(Debug, PartialEq, Eq)]
struct PosDirType(i64, i64, Dir);
#[derive(Debug, PartialEq, Eq)]
struct PqType(i64, PosDirType);

impl PartialOrd for PqType {
    fn partial_cmp(&self, other: &Self) -> Option<Ordering> {
        Some(other.0.cmp(&self.0))
    }
}

impl Ord for PqType {
    fn cmp(&self, other: &Self) -> Ordering {
        other.0.cmp(&self.0)
    }
}

fn bfs(table: &Vec<Vec<i64>>, min_move: i64, max_move: i64) -> Option<i64> {
    let row_len = table.len() as i64;
    let col_len = table[0].len() as i64;

    let mut visited = HashSet::new();
    let mut pq: BinaryHeap<PqType> = BinaryHeap::new();

    pq.push(PqType(0, PosDirType(0, 0, Dir::D)));
    pq.push(PqType(0, PosDirType(0, 0, Dir::R)));

    while let Some(PqType(value, PosDirType(x, y, dir))) = pq.pop() {
        if x == row_len - 1 && y == col_len - 1 {
            return Some(value);
        }

        if !visited.insert(((x, y), dir)) {
            continue;
        }

        match dir {
            Dir::D => {
                if x + min_move < row_len {
                    let mut path = 0;
                    for i in (x + 1)..=(x + max_move).min(row_len - 1) {
                        path += table[i as usize][y as usize];
                        if i - x >= min_move {
                            pq.push(PqType(value + path, PosDirType(i, y, Dir::L)));
                            pq.push(PqType(value + path, PosDirType(i, y, Dir::R)));
                        }
                    }
                }
            }
            Dir::U => {
                if x - min_move >= 0 {
                    let mut path = 0;
                    for i in ((x - max_move).max(0)..x).rev() {
                        path += table[i as usize][y as usize];
                        if x - i >= min_move {
                            pq.push(PqType(value + path, PosDirType(i, y, Dir::L)));
                            pq.push(PqType(value + path, PosDirType(i, y, Dir::R)));
                        }
                    }
                }
            }
            Dir::R => {
                if y + min_move < col_len {
                    let mut path = 0;
                    for i in (y + 1)..=(y + max_move).min(col_len - 1) {
                        path += table[x as usize][i as usize];
                        if i - y >= min_move {
                            pq.push(PqType(value + path, PosDirType(x, i, Dir::U)));
                            pq.push(PqType(value + path, PosDirType(x, i, Dir::D)));
                        }
                    }
                }
            }
            Dir::L => {
                if y - min_move >= 0 {
                    let mut path = 0;
                    for i in ((y - max_move).max(0)..y).rev() {
                        path += table[x as usize][i as usize];
                        if y - i >= min_move {
                            pq.push(PqType(value + path, PosDirType(x, i, Dir::U)));
                            pq.push(PqType(value + path, PosDirType(x, i, Dir::D)));
                        }
                    }
                }
            }
        }
    }

    None
}

fn main() {
    let table = read()
        .into_iter()
        .map(|x| {
            x.chars()
                .map(|x| x.to_digit(10).unwrap() as i64)
                .collect::<Vec<_>>()
        })
        .collect::<Vec<_>>();

    println!("part1 {:?}", bfs(&table, 0, 3).unwrap());
    println!("part2 {:?}", bfs(&table, 4, 10).unwrap());
}
