#include <iostream>
#include <cstdlib>
#include <string>

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

// game settings
typedef struct
{
    game_mode_t difficulty;
    int max_attempts;
    int enable_hints;
    std::vector<std::string> category;
    int category_mode;

} settings_t;

void default_settings(settings_t &game)
{
    game.difficulty = easy;
    game.max_attempts = 6; // max is 6 in is 7
    game.enable_hints = 0; // 1 is enabled 0 is disabled
    game.category = {"animal", "adjective", "noun"};
}

// Function to clear the console screen
void clearScreen()
{
    std::cout << "\033[2J\033[H"; // ANSI escape code to clear the screen
}

void menu()
{
    std::vector<std::string> options = {
        "Start Game",
        "Settings",
        "High Scores",
        "Exit"};
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

int main()
{
    int sel = 0;
    settings_t settings;
    std::string word = get_random_word(settings.difficulty, settings.category[rand() % 5]);

    while (0)
    {
        // std::cout << word << "\n";
    }

    return 0;
}