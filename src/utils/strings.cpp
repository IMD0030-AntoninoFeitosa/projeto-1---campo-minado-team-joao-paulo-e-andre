#include <vector>
#include <string>

/**
 * @brief quebra de string
 * 
 */
void split(const std::string &s, char c, std::vector<std::string> &v){
  std::string::size_type i = 0;
  std::string::size_type j = s.find(c);
  while (j != std::string::npos){
    v.push_back(s.substr(i, j-i));
    i = ++j;
    j = s.find(c, j);
    if (j == std::string::npos)
      v.push_back(s.substr(i, s.length( )));
  }
}

/**
 * @brief ordenacao de itens
 * 
 */
void sort(std::vector<std::vector<std::string>> &v){
  for (int i = 0; i < v.size()-1; i++){
    for (int j= 0; j < v.size() - i - 1; j++){
      if( std::stoi(v[j][1]) > std::stoi(v[j+1][1]) ){
        std::swap(v[j], v[j+1]);
      }
    }
  }
}