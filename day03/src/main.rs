use lazy_static::lazy_static;
use regex::{Match, Regex};
use std::fs::File;
use std::io::{self, BufRead};
use std::iter::Map;
use std::path::Path;
use std::slice::Iter;

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

fn find_integer(table: &mut Vec<Vec<char>>, mut x: i32, mut y: i32) -> Option<i64> {
    if x < 0
        || x >= table.len() as i32
        || y < 0
        || y >= table.first().unwrap().len() as i32
        || !table[x as usize][y as usize].is_ascii_digit()
    {
        return Option::None;
    }

    while y > 0 && table[x as usize][(y - 1) as usize].is_ascii_digit() {
        y -= 1;
    }
    let mut value = 0i64;
    while y < table.len() as i32 && table[x as usize][y as usize].is_ascii_digit() {
        value *= 10;
        value += table[x as usize][y as usize].to_digit(10).unwrap() as i64;
        table[x as usize][y as usize] = '.';
        y += 1;
    }
    return Some(value);
}

fn part1() {
    let table: Vec<Vec<char>> = read()
        .iter()
        .map(|x| x.chars().into_iter().collect::<Vec<char>>())
        .collect();
    let mut table_copy = table.clone();

    let mut total = 0i64;
    for (x, row) in table.iter().enumerate() {
        for (y, ch) in row.iter().enumerate() {
            if ch.is_ascii_digit() || *ch == '.' {
                continue;
            }

            for (off_x, off_y) in vec![
                (-1, -1),
                (-1, 0),
                (-1, 1),
                (0, -1),
                (0, 1),
                (1, -1),
                (1, 0),
                (1, 1),
            ] {
                if let Some(value) =
                    find_integer(&mut table_copy, (x as i32) + off_x, (y as i32) + off_y)
                {
                    total += value;
                }
            }
        }
    }
    println!("part1 {}", total);
}

fn part2() {
    let table: Vec<Vec<char>> = read()
        .iter()
        .map(|x| x.chars().into_iter().collect::<Vec<char>>())
        .collect();
    let mut table_copy = table.clone();

    let mut total = 0i64;
    for (x, row) in table.iter().enumerate() {
        for (y, ch) in row.iter().enumerate() {
            if ch.is_ascii_digit() || *ch == '.' {
                continue;
            }

            let mut powers: Vec<i64> = vec![];
            for (off_x, off_y) in vec![
                (-1, -1),
                (-1, 0),
                (-1, 1),
                (0, -1),
                (0, 1),
                (1, -1),
                (1, 0),
                (1, 1),
            ] {
                if let Some(value) =
                    find_integer(&mut table_copy, (x as i32) + off_x, (y as i32) + off_y)
                {
                    powers.push(value);
                }
            }

            if powers.len() < 2 {
                continue;
            }

            let mut power: i64 = 1;
            for value in powers {
                power *= value;
            }
            total += power;
        }
    }
    println!("part2 {}", total);
}

fn main() {
    part1();
    part2();
}
