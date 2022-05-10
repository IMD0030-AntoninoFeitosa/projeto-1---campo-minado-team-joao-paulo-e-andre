#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <time.h> 
#include <iomanip>
#include <sstream>

#include "./utils/strings.cpp"
#include "Game.h"

//explica como usar comandos do jogo
void show_usage(){
  std::cout << "Usage: game [option]" << std::endl;
  std::cout << "Option:" << std::endl;
  std::cout << " -h or --help                  Display this information." << std::endl;
  std::cout << " -d or --difficulty <option>   Change the game difficulty for <option>" << std::endl;
  std::cout << "                               -b or --beginner" << std::endl;
  std::cout << "                               -i or --intermediary" << std::endl;
  std::cout << "                               -a or --advanced" << std::endl;
}

//exibe mapa do jogo em terminal
void show_map(Map map){
  std::cout << std::endl;
  
  for (int i = 0; i < map.height; i++){
    std::cout << std::setfill('0') << std::setw(2) << i << "   ";
    for (int j = 0; j < map.width; j++){
      if (map.cells[i][j].is_hidden == true){
        std::cout << ".";
      }
      else {
        if(map.cells[i][j].has_flag == true){
          std::cout << "F";
        }
        else if(map.cells[i][j].has_bomb == true){
          std::cout << "B";
        }
        else{
          std::cout << map.cells[i][j].total_mines;
        }
      }
      std::cout << "  ";
    }
    std::cout << std::endl;
  }

  std::cout << std::endl << "    ";

  for (int i = 0; i < map.height; i++){
    std::cout << std::setfill('0') << std::setw(2) << i << " ";
  }

  std::cout << std::endl;

  std::cout << std::endl;
}

//cria mapa do jogo
Map create_map(int height, int width, int total_mines) {
  Map map;  
  map.height = height;
  map.width = width;

  Cell cell;  

  for (int i = 0; i < map.height; i++){
    std::vector<Cell> cells;

    for (int j = 0; j < map.width; j++){
      cells.push_back(cell);
    }

    map.cells.push_back(cells);
  }

  return map;
}

Game start_game(Difficulty level) {
  Game game;

  int height = 10;
  int width = 10;  
  int total_mines = 10;

  if(level == Difficulty::intermediary){
      height = 15;
      width = 15;
      total_mines = 40; 
  }

 if(level == Difficulty::advanced){
     height = 30;
     width = 15;
     total_mines = 100;
  }

  game.map = create_map(height, width, total_mines);
  game.level = level;
  game.total_mines = total_mines;

  return game;
}

//metodo principal para realizar logica da partida
bool play(Difficulty level){
  std::cout << "\nWelcome to minesweeper!" << std::endl;  
  
  Game game = start_game(level);
  show_map(game.map);  

  return true;
}

//grava dificuldade (nivel) em arquivo de configuracao
void log_difficulty(const std::string config_file, Difficulty level){
  std::ofstream file;
  file.open (config_file.c_str(), std::ifstream::out);
  if(file.is_open()){
    switch(level){
      case Difficulty::beginner:
        file << 'b';
        std::cout << "Difficulty set to begginer!" << std::endl;
        break;
      case Difficulty::intermediary:
        file << 'i';
        std::cout << "Difficulty set to intermediary!" << std::endl;
        break;
      case Difficulty::advanced:
        file << 'a';
        std::cout << "Difficulty set to advanced!" << std::endl;
        break;
    }
    file.close();
  }
}

//carrega a dificuldade (nivel) atual
Difficulty load_difficulty(const std::string config_file){
  Difficulty level;
  std::ifstream file;
  file.open (config_file.c_str(), std::ifstream::in);
  if(file.is_open()){
    char c;
    file >> c;
    switch(c){
      case 'b': level = Difficulty::beginner; break;
      case 'i': level = Difficulty::intermediary; break;
      case 'a': level = Difficulty::advanced; break;
    }
    file.close();
  } else {
    log_difficulty(config_file, Difficulty::beginner);
    level = Difficulty::beginner;
  }
  return level;
}

//reage a acoes do usuario via terminal. Prompt do jogo
void prompt(int argc, char** argv) {
    std::string arg = argv[1];
    
    if (arg == "-h" || arg == "-help"){
      show_usage();
    }
    else if (arg == "-d" || arg == "--difficulty"){
      if (argc > 2){
        std::string newlevel = argv[2];
        
        if(newlevel == "-b" || newlevel == "--beginner"){
          log_difficulty(CONFIG_FILE, Difficulty::beginner);
        }
        else if(newlevel == "-i" || newlevel == "--intermediary"){
          log_difficulty(CONFIG_FILE, Difficulty::intermediary);
        }
        else if(newlevel == "-a" || newlevel == "--advanced"){
          log_difficulty(CONFIG_FILE, Difficulty::advanced);
        }
        else {
          std::cout << "Unknown difficulty argument: " << newlevel << std::endl;
          show_usage();
        }
      }
      else {
        std::cout << "It was expected a difficulty for: " << argv[1] << std::endl;
        show_usage();
      }
    }
    else {
      std::cout << "Unknown argument: " << argv[1] << std::endl;
      show_usage();
    }
}

//finaliza a partida
void end_game(bool hasFailed, int seconds){
  std::string name;
  
  if (hasFailed){
    std::cout << "Game Over!!!" << std::endl;
    return;
  }
  
  std::cout << "Congrats! You finished the level in " << seconds << " seconds!"<< std::endl;
}
