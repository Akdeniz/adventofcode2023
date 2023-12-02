use std::fs::File;
use std::io::{self, BufRead};
use std::iter::Map;
use std::path::Path;
use std::slice::Iter;
use lazy_static::lazy_static;
use regex::{Match, Regex};

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>> where P: AsRef<Path> {
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


struct Result {
    game:i32,
    red: i32,
    blue: i32,
    green: i32
}

fn find(line: &str) -> Result {


    lazy_static! {
        static ref GAME_RE: Regex = Regex::new(r#"Game (?P<number>[0-9]+)"#).unwrap();
        static ref CUBE_RE: Regex = Regex::new(r#"(?P<count>[0-9]+) (?P<color>red|green|blue)"#).unwrap();
    }

    let mut result = Result{game:0, red: 0, blue: 0, green: 0 };

    result.game = GAME_RE.captures_iter(line).next().unwrap().name("number").unwrap().as_str().parse::<i32>().unwrap();
    for section in line.split(';') {
        for capture in CUBE_RE.captures_iter(section) {
            let count = capture.name("count").unwrap().as_str().parse::<i32>().unwrap();
            match capture.name("color").unwrap().as_str() {
                "red" => { result.red = result.red.max(count) }
                "blue" => { result.blue = result.blue.max(count) }
                "green" => { result.green = result.green.max(count) }
                _ => panic!("unknown color")
            }
        }
    }

    //println!("{} game({}) red({}) blue({}) green({})", line, result.game, result.red, result.blue, result.green);

    result
}

fn main() {
    let mut total: i32 = 0;
    for line in read() {
        let result = find(&line);
        if result.red <= 12 && result.green <= 13 && result.blue <= 14 {
            total += result.game;
        }
    }

    println!("Part1: {}", total);

    let mut total: i32 = 0;
    for line in read() {
        let result = find(&line);
        let mut power = 1;
        if result.red > 0 { power*=result.red }
        if result.blue > 0 { power*=result.blue }
        if result.green > 0 { power*=result.green }

        total+=power;
    }

    println!("Part2: {}", total);
}

