#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <time.h> 
#include <iomanip>
#include <sstream>

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
      //celula oculta  
      if (map.cells[i][j].is_hidden == true){
        std::cout << ".";
      }
      else {
        //com bandeira (f)
        if(map.cells[i][j].has_flag == true){
          std::cout << "F";
        }
        //com minas (m)
        else if(map.cells[i][j].has_mine == true){
          std::cout << "M";
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

  for (int i = 0; i < map.width; i++){
    std::cout << std::setfill('0') << std::setw(2) << i << " ";
  }  

  //linha divisoria do mapa
  std::cout << std::endl;
  std::cout << std::endl << "    ";
  for (int i = 0; i < map.width; i++){
    std::cout << "---";
  }

  std::cout << std::endl;
}

//preenche as minas do mapa a depender do total estabelecido pelo nivel do jogo
void fill_with_mines(Map &map, int total_mines) {
  std::srand(time(NULL));

  int count_mines = 0;
  
  while(count_mines < total_mines){
    int random = rand() % (map.height * map.width);

    int h = random / map.height;
    
    int w = random % map.width;

    if (map.cells[h][w].has_mine == false){
        map.cells[h][w].has_mine = true;
        count_mines++;
    }
  }
}

//checa se ponto esta dentro do mapa
bool is_inside_map(Map map, int w, int h) {
    return (h >= 0 && h < map.height) && (w >= 0 && w < map.width);
}

//checa se ha minea em um ponto do mapa
bool has_mine(Map map, int w, int h) {
    return map.cells[h][w].has_mine;
}

//conta quantidade de minas ao redor de um ponto
int count_nested_mines(Map map, int px, int py) {
  int count = 0;
  
  for (int y = -1; y <= 1; y++) {
    for (int x = -1; x <= 1; x++) {
      int dx = px + x;
      int dy = py + y;
      if (is_inside_map(map, dx, dy) && has_mine(map, dx, dy)) {
        count++;
      }
    }
  }

  return count;
}

//preenche quantidade de minas ao redor de cada celula do mapa inteiro
void fill_with_count_nested_mines(Map &map) {
    for (int h = 0; h < map.height; h++){
        for (int w = 0; w < map.width; w++){
           map.cells[h][w].total_mines = count_nested_mines(map, w, h);           
        }
    }
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

  fill_with_mines(map, total_mines);
  
  fill_with_count_nested_mines(map);

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

//checa se e acao de revelar
bool isRevealAction(char action) {
    return action == 'r';
}

//checa se e acao de colocar bandeira
bool isFlagAction(char action) {
    return action == 'f';
}

//metodo principal para realizar logica da partida
bool play(Difficulty level){  
  std::cout << "\nWelcome to minesweeper!" << std::endl;  
  std::cout << "\nType r for revealing mines and f to put flag" << std::endl;  
    
  char action;
  bool end = false;
  bool won = false;
  int x, y = 0;

  Game game = start_game(level); 

  while(end == false) {  
   show_map(game.map);     

   std::cout << std::endl; 
   std::cout << "Type your action [r/f]: ";   
   std::cin >> action;

   if(isRevealAction(action) || isFlagAction(action)) {
       //ler coordenadas 
       std::cout << "Type coord x: ";
       std::cin >> x;
       std::cout << "Type coord y: ";
       std::cin >> y;

       //checar se posicao e valida. Enquanto nao for, pedir coordenadas
       while(!is_inside_map(game.map, x, y)) {
           std::cout << "Invalid input" << std::endl;
           std::cout << "Type coord x: ";
           std::cin >> x;
           std::cout << "Type coord y: ";
           std::cin >> y;
       }

       //checar se existe bomba, se sim, finaliza jogo com derrota
       if(has_mine(game.map, x, y)) {
            game.map.cells[y][x].is_hidden = false;
            game.map.cells[y][x].has_mine = true;
            show_map(game.map); 
            end = true;
            won = false;
       }

       //caso nao seja encontrada uma bomba, e exibida celula
       game.map.cells[y][x].is_hidden = false;
   }
   else {
    std::cout << "Oops..Invalid action!" << std::endl;       
   } 
  }

  return won;
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
void end_game(bool won, int seconds){
  if (won == false){
    std::cout << "\nGame Over!!!" << std::endl;
    return;
  }
  
  std::cout << "Congrats! You finished the game in " << seconds << " seconds!"<< std::endl;
}
