#include <iostream> // For input/output operations (cin, cout, etc.)
#include <cstdlib>  // For general purpose functions, including rand() for random number generation
#include <string>   // For string handling
#include <map>      // For std::map, an associative container
#include <vector>   // For std::vector, a dynamic array
#include <chrono>   // For time-related functions, useful for delays or timing operations
#include <thread>   // For std::this_thread::sleep_for, used with chrono for creating delays

// Windows - specific headers
#include <winsock2.h>
#include <windows.h> // Core Windows functions, structures, and constants
#include <conio.h>   // For console I/O, including _kbhit() and _getch() for non-blocking input

#include "nlohmann/json.hpp"

#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

typedef enum
{
    easy = 5,
    normal = 8,
    hard = 10,
    expert = 15
} game_mode_t;

const std::string mode_str[] = {
    "", "", "", "",         // Indices 0-4
    "easy", "", "",         // Indices 5-7
    "normal", "",           // Indices 8-9
    "hard", "", "", "", "", // Indices 10-14
    "expert"                // Index 15
};

const std::string hints[] = {
    "Disabled", "Enabled"};

// game settings
typedef struct
{
    game_mode_t difficulty;
    std::string game_mode_strings;
    int max_attempts;
    int enable_hints;
    std::vector<std::string> category;
    int category_mode;

} settings_t;

std::string get_random_word(int len, std::string type);
void settings(settings_t set);
void print_menu(const std::vector<std::string> &options, int selectedOption, int type, settings_t set);
void clear_screen();
void print_hangman(int stage);
void run_gameplay(settings_t set);
void default_settings(settings_t &game);

int main()
{
    int sel = 0;
    settings_t setting;
    default_settings(setting);
    std::vector<std::string>
        options = {
            "Start Game",
            "Settings",
            "Exit"};

    int current = 0;
    int stage = 0;
    char ch;

    while (true)
    {
        clear_screen();
        print_hangman(stage);
        std::cout << "\nWelcome to Hang-a-man!\n\n";
        print_menu(options, current, 0, setting);
        std::cout << "\nup- w key : down - s key : select - enter\n\n";

        if (_kbhit())
        {
            ch = _getch();
            switch (ch)
            {
            case 'w':
                current = (current - 1 + options.size()) % options.size();
                break;
            case 's':
                current = (current + 1) % options.size();
                break;
            case '\r': // Changed from '\r' to '\n'
                if (current == 0)
                {
                    std::cout << "You're about ot hang a man!" << "\n";
                    run_gameplay(setting);
                }
                else if (current == 1)
                {
                    settings(setting);
                }
                else if (current == options.size() - 1)
                {
                    return 0; // Exit the function
                }
                break;
            case 'q':
                return 0; // Exit the function
            }
        }

        // Animate hangman
        stage = (stage + 1) % 7;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void default_settings(settings_t &game)
{
    game.difficulty = easy;

    game.max_attempts = 6; // max is 6 in is 7

    game.enable_hints = 0; // 1 is enabled 0 is disabled

    game.category = {"animal", "adjective", "noun"};

    game.category_mode = 0;
}

// Function to clear the console screen
void clear_screen()
{
#ifdef _WIN32

    system("cls");

#else

    system("clear");

#endif
}

void print_hangman(int stage)
{
    const std::vector<std::string> hangmanStages = {
        "  +---+\n  |   |\n      |\n      |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|\\  |\n /    |\n      |\n=========",
        "  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n========="};

    std::cout << hangmanStages[stage] << "\n";
}

void print_menu(const std::vector<std::string> &options, int selectedOption, int type, settings_t set)
{
    for (size_t i = 0; i < options.size(); ++i)
    {
        if (i == selectedOption)
        {
            std::cout << " > " << " ";
        }
        else
        {
            std::cout << "  " << " ";
        }

        if (type == 0)
        {
            std::cout << options[i] << "\n";
        }
        else
        {
            std::cout << options[i] << " : ";
            if (options[i] == "Difficulty")
            {
                std::cout << mode_str[set.difficulty] << "\n";
            }
            else if (options[i] == "Attempts")
            {
                std::cout << set.max_attempts << "\n";
            }
            else
            {
                std::cout << hints[set.enable_hints] << "\n";
            }
        }
    }
}

void run_gameplay(settings_t set)
{
    std::string type = set.category[rand() % 2];
    std::string word = get_random_word(set.difficulty, type);
    std::string word_attempt;
    word_attempt.resize(word.size());

    for (int i = 0; i < set.difficulty; i++)
    {
        word_attempt[i] = '_';
    }

    int tries = 0;
    int index;
    char ch;

    if (word.empty())
    {
        return;
    }

    while (tries < set.max_attempts && word_attempt != word)
    {
        clear_screen();
        ch = getchar();
        std::cout << "category" << ":" << type;
        std::cout << "\t\tTries Left" << ":" << set.max_attempts - tries << "\n\n";
        print_hangman(tries);

        if (ch != '\0' && ch != '\n' && ch != ' ')
        {
            if (word.find(ch) != std::string::npos)
            {
                word_attempt[word.find(ch)] = ch;
            }
            else
            {
                std::cout << "Wrong Character enter!" << "\n";
                tries++;
            }
            getchar();
        }
        std::cout << "Word Attempt" << ":" << word_attempt << "\n";
    }

    if (word_attempt == word)
    {
        std::cout << "Congratulations! You guessed the word!" << std::endl;
    }
    else
    {
        std::cout << "You ran out of tries!" << std::endl;
    }
}

void settings(settings_t set)
{
    int current = 0;

    std::vector<std::string> options = {
        "Difficulty",
        "Attempts",
        "Hints"};
    while (0)
    {
        std::cout << "Settings" << "\n\n";
        print_menu(options, current, 1, set);
    }
}

std::string get_random_word(int len, std::string type)
{
    std::string word_type = "/=" + type + "?length=" + std::to_string(len);
    httplib::Client cli("https://random-word-form.herokuapp.com/random");

    auto result = cli.Get(word_type.c_str());

    // if successful
    if (result && result->status == 200)
    {
        nlohmann::json response = nlohmann::json::parse(result->body);
        if (!response.empty() && response.is_array())
        {
            return response[0];
        }
    }
    return "";
}