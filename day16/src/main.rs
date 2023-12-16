use gcd::binary_u64;
use lazy_static::lazy_static;
use regex::{Match, Regex};
use std::collections::{HashMap, HashSet, VecDeque};
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

#[derive(Debug, Copy, Clone, PartialEq, Eq, Hash)]
enum Dir {
    U,
    D,
    L,
    R,
}
type PosDirType = ((i64, i64), Dir);

fn new_dir(dir: Dir, mirror: char) -> Vec<Dir> {
    return match (dir, mirror) {
        (Dir::R, '.') | (Dir::R, '-') => vec![Dir::R],
        (Dir::R, '/') => vec![Dir::U],
        (Dir::R, '\\') => vec![Dir::D],
        (Dir::R, '|') => vec![Dir::U, Dir::D],

        (Dir::L, '.') | (Dir::L, '-') => vec![Dir::L],
        (Dir::L, '/') => vec![Dir::D],
        (Dir::L, '\\') => vec![Dir::U],
        (Dir::L, '|') => vec![Dir::U, Dir::D],

        (Dir::U, '.') | (Dir::U, '|') => vec![Dir::U],
        (Dir::U, '/') => vec![Dir::R],
        (Dir::U, '\\') => vec![Dir::L],
        (Dir::U, '-') => vec![Dir::R, Dir::L],

        (Dir::D, '.') | (Dir::D, '|') => vec![Dir::D],
        (Dir::D, '/') => vec![Dir::L],
        (Dir::D, '\\') => vec![Dir::R],
        (Dir::D, '-') => vec![Dir::R, Dir::L],
        _ => panic!(),
    };
}

fn bfs(table: &Vec<Vec<char>>, start: PosDirType) -> usize {
    let row_len = table.len() as i64;
    let col_len = table[0].len() as i64;

    let mut visited = HashSet::new();
    let mut q = VecDeque::from([start]);

    while !q.is_empty() {
        let ((x, y), dir) = q.pop_front().unwrap();

        if x < 0 || x >= row_len || y < 0 || y >= col_len {
            continue;
        }

        if !visited.insert(((x, y), dir)) {
            continue;
        }

        // empty space (.), mirrors (/ and \), and splitters (| and -).
        for d in new_dir(dir, table[x as usize][y as usize]) {
            let (newx, newy) = match d {
                Dir::U => (x - 1, y),
                Dir::D => (x + 1, y),
                Dir::L => (x, y - 1),
                Dir::R => (x, y + 1),
            };

            if newx < 0 || newx >= row_len || newy < 0 || newy >= col_len {
                continue;
            }

            q.push_back(((newx, newy), d));
        }
    }

    visited
        .iter()
        .map(|&(x, _)| x)
        .collect::<HashSet<_>>()
        .len()
}

fn main() {
    let table = read()
        .into_iter()
        .map(|x| x.chars().collect::<Vec<_>>())
        .collect::<Vec<_>>();

    println!("part1 {:?}", bfs(&table, ((0, 0), Dir::R)));

    let mut result = 0;
    for i in 0..table.len() as i64 {
        result = result.max(bfs(&table, ((i, 0), Dir::R)));
        result = result.max(bfs(&table, ((i, (table[0].len() - 1) as i64), Dir::L)));
    }

    for i in 0..table[0].len() as i64 {
        result = result.max(bfs(&table, ((0, i), Dir::D)));
        result = result.max(bfs(&table, (((table.len() - 1) as i64, i), Dir::L)));
    }

    println!("part2 {:?}", result);
}
