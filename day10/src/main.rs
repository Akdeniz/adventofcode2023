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

const LEFT: (i32, i32) = (0, -1);
const RIGHT: (i32, i32) = (0, 1);
const UP: (i32, i32) = (-1, 0);
const DOWN: (i32, i32) = (1, 0);

const NEIGHS: [(i32, i32);4] = [LEFT, RIGHT, UP, DOWN];

fn get_next_pos(pos: (i32, i32), direction: (i32, i32), ch: char) -> Option<(i32, i32)> {

    assert!("|-LJ7F".find(ch).is_some());
    match (ch, direction) {
        ('|', UP) | ('|', DOWN) => return Some((pos.0+direction.0, pos.1)),
        ('-', LEFT) | ('-', RIGHT) => return Some((pos.0, pos.1+direction.1)),
        ('L', LEFT) | ('L', DOWN) => return Some((pos.0+direction.1, pos.1+direction.0)),
        ('J', RIGHT) | ('J', DOWN) => return Some((pos.0-direction.1, pos.1-direction.0)),
        ('7', RIGHT) | ('7', UP) => return Some((pos.0+direction.1, pos.1+direction.0)),
        ('F', LEFT) | ('F', UP) => return Some((pos.0-direction.1, pos.1-direction.0)),
        _ => return None
    }
}

fn dfs(table: &Vec<Vec<char>>, pos: (i32, i32), prevpos: (i32, i32), end: (i32, i32), visited: &mut HashSet<(i32, i32)>) -> bool {

    if pos.0 < 0 || pos.0 as usize >= table.len() || pos.1 <0 || pos.1 as usize >= table[0].len() {
        return false;
    }

    if pos==end {
        visited.insert(end);
        return true;
    }

    let ch = table[pos.0 as usize][pos.1 as usize];

    if visited.contains(&pos) || ch=='.' {
        return false;
    }

    let direction  = (pos.0-prevpos.0, pos.1-prevpos.1);

    if let Some(nextpos) = get_next_pos(pos, direction, ch) {

        visited.insert(pos);

        if dfs(table, nextpos, pos, end, visited) {
            return true;
        }

        visited.remove(&pos);
    }

    return false;
}

fn part1() {
    let mut table = read().into_iter().map(|row| row.chars().collect()).collect::<Vec<Vec<char>>>();
    let start = table
        .iter()
        .enumerate()
        .filter(|(_, row)| row.contains(&'S'))
        .map(|(x, row)| (x as i32, row.iter().position(|&x| x=='S').unwrap() as i32))
        .last()
        .unwrap();


    let mut visited = HashSet::new();
    for (x, y) in NEIGHS {
        if dfs(&table, (start.0+x, start.1+y), start, start, &mut visited) {
            break;
        }
    }

    println!("part1 {}", (visited.len()+1)/2);
}

fn part2() {
    let mut table = read().into_iter().map(|row| row.chars().collect()).collect::<Vec<Vec<char>>>();
    let start = table
        .iter()
        .enumerate()
        .filter(|(_, row)| row.contains(&'S'))
        .map(|(x, row)| (x as i32, row.iter().position(|&x| x=='S').unwrap() as i32))
        .last()
        .unwrap();


    {
        let left = start.1 > 0 && "-LF".find(table[start.0 as usize][(start.1-1) as usize]).is_some();
        let right = ((start.1 + 1) as usize) < table[0].len() && "-J7".find(table[start.0 as usize][(start.1+1) as usize]).is_some();
        let up = start.0> 0 && "|7F".find(table[(start.0 -1) as usize][start.1 as usize]).is_some();
        let down = ((start.0+1) as usize) < table.len() && "|LJ".find(table[(start.0+1) as usize][start.1 as usize]).is_some();

        let replacing_ch = match (left,right, up, down) {
            (true, true, false, false) => '-',
            (false, false, true, true) => '|',
            (false, true, true, false) => 'L',
            (true, false, true, false) => 'J',
            (false, true, false, true) => 'F',
            (true, false, false, true) => '7',
            (_, _, _, _) => panic!()
        };

        table[start.0 as usize][start.1 as usize] = replacing_ch;
    }

    let mut visited = HashSet::new();
    for (x, y) in NEIGHS {
        if dfs(&table, (start.0+x, start.1+y), start, start, &mut visited) {
            break;
        }
    }

    let mut inside_count = 0;
    for (i, row) in table.iter().enumerate() {
        let mut last_ch = '-';
        let mut inside = false;
        for (j, &ch) in row.iter().enumerate() {
            if !visited.contains(&(i as i32, j as i32)) {
                if inside {
                    inside_count += 1;
                }
            } else {
                match ch {
                    'F' | 'L' | '|' => {
                        inside = !inside;
                        last_ch = ch;
                    },
                    'J' => {
                        if last_ch == 'L' {
                            inside = !inside;
                        }
                        last_ch= ch;
                    },
                    '7' => {
                        if last_ch == 'F' {
                            inside = !inside
                        }
                        last_ch = ch;
                    }
                    _ => {}
                }
            }
        }
    }
    println!("part2 {}", inside_count);
}

fn main() {
    part1();
    part2();
}
