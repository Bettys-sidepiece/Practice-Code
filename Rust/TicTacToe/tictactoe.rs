use std::io;

#[derive(Clone, Copy, PartialEq)]
enum Player {
    X,
    O,
}

struct Game {
    board: [Option<Player>; 9],
    current_player: Player,
}

impl Game {
    fn new() -> Game {
        Game {
            board: [None; 9],
            current_player: Player::X,
        }
    }

    fn print_board(&self) {
        for i in 0..3 {
            for j in 0..3 {
                match self.board[i * 3 + j] {
                    Some(Player::X) => print!("X"),
                    Some(Player::O) => print!("O"),
                    None => print!("."),
                }
                if j < 2 {
                    print!("|");
                }
            }
            println!();
            if i < 2 {
                println!("-+-+-");
            }
        }
    }

    fn make_move(&mut self, position: usize) -> bool {
        if position < 9 && self.board[position].is_none() {
            self.board[position] = Some(self.current_player);
            self.current_player = match self.current_player {
                Player::X => Player::O,
                Player::O => Player::X,
            };
            true
        } else {
            false
        }
    }

    fn check_winner(&self) -> Option<Player> {
        let winning_combinations = [
            [0, 1, 2],
            [3, 4, 5],
            [6, 7, 8], // Rows
            [0, 3, 6],
            [1, 4, 7],
            [2, 5, 8], // Columns
            [0, 4, 8],
            [2, 4, 6], // Diagonals
        ];

        for combo in winning_combinations.iter() {
            if let (Some(player), Some(player), Some(player)) = (
                self.board[combo[0]],
                self.board[combo[1]],
                self.board[combo[2]],
            ) {
                return Some(player);
            }
        }
        None
    }

    fn is_board_full(&self) -> bool {
        self.board.iter().all(|&cell| cell.is_some())
    }
}

fn main() {
    let mut game = Game::new();

    loop {
        game.print_board();

        if let Some(winner) = game.check_winner() {
            println!("Player {:?} wins!", winner);
            break;
        }

        if game.is_board_full() {
            println!("It's a draw!");
            break;
        }

        println!(
            "Player {:?}'s turn. Enter a position (0-8):",
            game.current_player
        );

        let mut input = String::new();
        io::stdin()
            .read_line(&mut input)
            .expect("Failed to read line");

        if let Ok(position) = input.trim().parse() {
            if !game.make_move(position) {
                println!("Invalid move. Try again.");
            }
        } else {
            println!("Invalid input. Please enter a number between 0 and 8.");
        }
    }
}
