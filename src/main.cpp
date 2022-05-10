#include <chrono>
#include "./utils/time.cpp"
#include "Game.h"

int main(int argc, char** argv){
  if (argc > 1){
      prompt(argc, argv);
  }
  else {
    Difficulty level = load_difficulty(CONFIG_FILE);
    
    std::chrono::_V2::system_clock::time_point begin = std::chrono::high_resolution_clock::now();
    
    bool gameResults = start_game(level);

    end_game(gameResults, calc_duration(begin));
  }
  return 0;
}