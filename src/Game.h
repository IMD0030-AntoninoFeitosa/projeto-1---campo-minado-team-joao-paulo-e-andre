#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

const std::string CONFIG_FILE = "./dist/config.txt";

enum class Difficulty { beginner, intermediary, advanced };

struct Cell {
  bool is_hidden {true};
  bool has_bomb {false};
  bool has_flag {false};
  short qtd_bombs;
};

using Map = std::vector<std::vector<Cell>>;

struct Point{
  short x, y;
};

struct Game{
  short total_bombs;
  Point dimensions;
};

//show usage game options
void show_usage();

//start the game bases on current level
bool start_game(Difficulty level);

//end the game and stores player information to the ranking if they win.
void end_game(bool hasFailed, int seconds);

//store game current level on txt config file
void store_difficulty(const std::string config_file, Difficulty level);

//load current difficulty
Difficulty load_difficulty(const std::string config_file);

//show users and their times
void show_leaderboard();

//reacts to user's inputs such as --difficulty flags etc
void prompt(int argc, char** argv);

//show map eaccording to the state of each cell
void show_map(Game game,  Map map);

//stores game settings based on the selected level.
Game create_game(Difficulty difficulty);

//generates map when called.
Map create_map(Game game);

//reveal all bombs locations.
void show_bombs(Game game, Map & map);

//check if the position is valid.
bool is_valid(Game game, Map & map, int x, int y);

//count mines around cell.
int count_bombs(Game game, Map & map, int x, int y);

//count flags around cell.
int count_flags(Game game, Map & map, int x, int y);

//reveals what's around the selected cell.
bool reveal_around(Game game, Map &map, int x,int y);

//clear cells around 0's.
void clear_neighbor(Game game, Map & map, int x, int y);

//receives player's input
bool has_flag(short &x, short &y);

//checks if the player won the game.
bool check_victory(Game game, Map map);

//game ending in case the user clicks on a bomb
bool game_lost(Game game, Map map);

#endif