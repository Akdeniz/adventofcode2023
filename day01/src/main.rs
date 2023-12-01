use std::fs::File;
use std::io::{self, BufRead};
use std::iter::Map;
use std::path::Path;
use std::slice::Iter;

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

fn find(line: &str, check_words: bool) -> (usize, usize) {
    let chars = vec!['0', '1', '2', '3', '4', '5', '6', '7', '8', '9'];
    let words = vec!["zero", "one", "two", "three", "four", "five", "six", "seven", "eight", "nine"];

    let chars_first = chars.iter().enumerate().filter(|elem| line.find(*elem.1).is_some()).map(|elem| (line.find(*elem.1).unwrap(), elem.0)).min();
    let chars_last = chars.iter().enumerate().filter(|elem| line.find(*elem.1).is_some()).map(|elem| (line.rfind(*elem.1).unwrap(), elem.0)).max();

    if !check_words {
        return (chars_first.unwrap().1, chars_last.unwrap().1);
    }

    let words_first = words.iter().enumerate().filter(|elem| line.find(*elem.1).is_some()).map(|elem| (line.find(*elem.1).unwrap(), elem.0)).min();
    let words_last = words.iter().enumerate().filter(|elem| line.find(*elem.1).is_some()).map(|elem| (line.rfind(*elem.1).unwrap(), elem.0)).max();

    if words_first.is_none() {
        return (chars_first.unwrap().1, chars_last.unwrap().1);
    }


    return (chars_first.min(words_first).unwrap().1, chars_last.max(words_last).unwrap().1);
}

fn main() {
    let mut total: usize = 0;
    for line in read() {
        let (first, last) = find(&line, false);
        // println!("{} -- {}, {}", line, first, last);
        total += first * 10 + last;
    }

    println!("Part1: {}", total);

    let mut total: usize = 0;
    for line in read() {
        let (first, last) = find(&line, true);
        // println!("{} -- {}, {}", line, first, last);
        total += first * 10 + last;
    }

    println!("Part2: {}", total);
}

