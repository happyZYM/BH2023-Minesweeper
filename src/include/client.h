#ifndef CLIENT_H
#define CLIENT_H

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <utility>
#include <vector>

extern int rows;     // The count of rows of the game map
extern int columns;  // The count of columns of the game map

// You can not use any other external variables except for rows and columns.

/**
 * @brief The definition of function Execute(int, int)
 *
 * @details This function is designed to take a step when player the client's
 * (or player's) role, and the implementation of it has been finished by TA. (I
 * hope my comments in code would be easy to understand T_T) If you do not
 * understand the contents, please ask TA for help immediately!!!
 *
 * @param row The row coordinate (0-based) of the block to be visited.
 * @param column The column coordinate (0-based) of the block to be visited.
 */
void Execute(int row, int column);
/**
 * @brief The definition of function InitGame()
 *
 * @details This function is designed to initialize the game. It should be
 * called at the beginning of the game, which will read the scale of the game
 * map and the first step taken by the server (see README).
 */
void InitGame() {
  int first_row, first_column;
  std::cin >> first_row >> first_column;
  Execute(first_row, first_column);
}

/**
 * @brief The definition of function ReadMap()
 *
 * @details This function is designed to read the game map from stdin when
 * playing the client's (or player's) role. Since the client (or player) can
 * only get the limited information of the game map, so if there is a 3 * 3 map
 * as above and only the block (2, 0) has been visited, the stdin would be
 *     ???
 *     12?
 *     01?
 */
namespace Client {
const unsigned int RndSeed = std::random_device{}();
std::mt19937 RawRnd(RndSeed);  // a basic random generator
const int max_size = 35;
char game_map[max_size][max_size];  // store the raw game map in format of char
std::queue<std::pair<int, int> >
    no_mine_block_to_be_clicked;  // store the block that definitely has no mine
                                  // and not yet clicked
int map_status[max_size]
              [max_size];  // store the status of each block(processed version),
                           // 0 means unknown , -1 means has mine, 1 means no
                           // mine and not yet clicked, and 2 means has been
                           // clicked Note that if some block is found to be
                           // definitely has no mine or has mine, it will be
                           // marked as known even if it is not clicked. In
                           // conclusion, if map_status[i][j] == 0, then
                           // game_map[i][j] == '?'. If map_status[i][j] == -1,
                           // then game_map[i][j] == '?', and it will never be
                           // clicked. If map_status[i][j] == 1, then
                           // game_map[i][j] == '?', and it will be clicked
                           // later. If map_status[i][j] == 2, then
                           // game_map[i][j] == '0'-'8', and it has been clicked
                           // And when a block's status is updated from 0 to 1,
                           // it will be pushed into no_mine_block_to_be_clicked
/**
 * @brief The definition of function ProcessSimpleCase()
 *
 * @details This function is designed to process the simplest case
 */
void ProcessSimpleCase() {
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++)
      if (map_status[i][j] == 2) {
        int nearby_mines = game_map[i][j] - '0',
            nearby_unkown =
                0;  // nearby_mines is the number of mines in currently unknown
                    // blocks that are adjacent to the block (i,j)
        const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1},
                  dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
        for (int k = 0; k < 8; k++) {
          int x = i + dx[k], y = j + dy[k];
          if (x >= 0 && x < rows && y >= 0 && y < columns) {
            if (map_status[x][y] == 0)
              nearby_unkown++;
            else if (map_status[x][y] == -1)
              nearby_mines--;
          }
        }
        if (nearby_unkown != 0) {
          if (nearby_mines == 0) {
            for (int k = 0; k < 8; k++) {
              int x = i + dx[k], y = j + dy[k];
              if (x >= 0 && x < rows && y >= 0 && y < columns &&
                  map_status[x][y] == 0) {
                map_status[x][y] = 1;
                no_mine_block_to_be_clicked.push(std::make_pair(x, y));
              }
            }
          } else if (nearby_mines == nearby_unkown) {
            for (int k = 0; k < 8; k++) {
              int x = i + dx[k], y = j + dy[k];
              if (x >= 0 && x < rows && y >= 0 && y < columns &&
                  map_status[x][y] == 0)
                map_status[x][y] = -1;
            }
          }
        }
      }
}
std::map<std::pair<int, int>, int>
    position_to_variaID;  // convert the (row,column) to variable ID in the
                          // equations,0 based
std::vector<std::pair<int, int> > variaID_to_position;
/**
 * @brief The definition of function PrintEquations()
 *
 * @details This function is designed to print the equations for debugging
 */
void PrintEquations(std::vector<std::vector<double> > equations) {
  // return;
  std::cout << "equations:" << std::endl;
  for (int i = 0; i < equations.size(); i++) {
    for (int j = 0; j < equations[i].size(); j++)
      std::cout << equations[i][j] << " ";
    std::cout << std::endl;
  }
  // use variaID_to_position to print the position of each variable
  std::cout << "variaID_to_position:" << std::endl;
  for (int i = 0; i < variaID_to_position.size(); i++)
    std::cout << "(" << variaID_to_position[i].first << ","
              << variaID_to_position[i].second << ")"
              << " ";
  std::cout << std::endl;
  // print map_status
  std::cout << "map_status:" << std::endl;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < columns; j++) std::cout << map_status[i][j] << " ";
    std::cout << std::endl;
  }
}
/**
 * @brief The definition of function GenerateEquations()
 *
 * @details This function is designed to scan the game_map and map_status to
 * generate the equations that will be used in Gaussian-Jordan Elimination.
 * It returns a vector<vector<double>> equations, where equations[i] is the i th
 * equation.
 */
std::vector<std::vector<double> > GenerateEquations() {
  variaID_to_position.clear();
  position_to_variaID.clear();
  int number_of_equations = 0;
  std::set<std::pair<int, int> > can_form_equations;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++)
      if (map_status[i][j] == 2) {
        const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1},
                  dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
        bool there_is_unknown_nearby = false;
        for (int k = 0; k < 8; k++) {
          int nr = i + dx[k], nc = j + dy[k];
          if (nr < 0 || nr >= rows || nc < 0 || nc >= columns) continue;
          if (map_status[nr][nc] != 0) continue;
          there_is_unknown_nearby = true;
          std::pair<int, int> pos = std::make_pair(nr, nc);
          if (position_to_variaID.find(pos) == position_to_variaID.end()) {
            int cnt = variaID_to_position.size();
            variaID_to_position.push_back(pos);
            position_to_variaID[pos] = cnt;
          }
        }
        number_of_equations += there_is_unknown_nearby;
        if (there_is_unknown_nearby)
          can_form_equations.insert(std::make_pair(i, j));
      }
  std::vector<std::vector<double> > equations;
  std::vector<double> equa_template;
  equa_template.resize(position_to_variaID.size() + 1);
  for (int i = 0; i < equa_template.size(); i++) equa_template[i] = 0;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++)
      if (can_form_equations.count(std::make_pair(i, j)) == 1) {
        assert('0' <= game_map[i][j] && game_map[i][j] <= '8');
        equations.push_back(equa_template);
        int nearby_mines = game_map[i][j] - '0';
        const int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1},
                  dy[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
        for (int k = 0; k < 8; k++) {
          int x = i + dx[k], y = j + dy[k];
          if (x >= 0 && x < rows && y >= 0 && y < columns) {
            if (map_status[x][y] == -1) nearby_mines--;
          }
        }
        equations[equations.size() - 1][position_to_variaID.size()] =
            nearby_mines;
        for (int k = 0; k < 8; k++) {
          int nr = i + dx[k], nc = j + dy[k];
          if (nr < 0 || nr >= rows || nc < 0 || nc >= columns) continue;
          if (map_status[nr][nc] != 0) continue;
          equations[equations.size() - 1]
                   [position_to_variaID[std::make_pair(nr, nc)]] = 1;
        }
      }
  // PrintEquations(equations);
  return equations;
}
/**
 * @brief The definition of function GaussianJordanElimination()
 * @details This function is designed to use Gaussian-Jordan Elimination to
 * solve the equations. It returns the processed vector<vector<double>>
 * &equations
 * @param vector<vector<double>> equations The equations to be solved
 */
const double eps = 1e-6;
const int error_status_of_nearint = -0x3f3f3f3f;
inline int nearint(double v) {
  int raw = v + 0.5;
  if (abs(v - raw) < eps)
    return raw;
  else
    return error_status_of_nearint;
}
std::vector<std::vector<double> > GaussianJordanElimination(
    std::vector<std::vector<double> > equations) {
  using std::abs;
  int n = equations.size();
  if (n == 0) return equations;
  int m = equations[0].size();
  // assert(n + 1 == m);
  for (int i = 0; i < n; i++) {
    int pivot = i;
    for (int j = i + 1; j < n; j++)
      if (abs(equations[j][i]) > abs(equations[pivot][i])) pivot = j;
    std::swap(equations[i], equations[pivot]);
    if (abs(equations[i][i]) < eps) continue;
    const double pivot_value = equations[i][i];
    for (int j = 0; j < m; j++) equations[i][j] /= pivot_value;
    for (int j = 0; j < n; j++)
      if (j != i) {
        const double tmp = equations[j][i];
        for (int k = 0; k < m; k++) equations[j][k] -= tmp * equations[i][k];
      }
  }
  return equations;
}
/**
 * @brief The definition of function InterpretResult()
 *
 * @details This function is designed to interpret the result of Gaussian-Jordan
 * Elimination
 * @param std::vector<std::vector<double> > &equations The solved status of the
 * equations
 */
void InterpretResult(std::vector<std::vector<double> > equations) {
  // std::cout << "InterpretResult" << std::endl;
  // PrintEquations(equations);
  int n = equations.size();
  if (n == 0) return;
  int m = equations[0].size();
  if (m == 1) return;
  for (int i = 0; i < n; i++) {
    // std::cout << "equations[" << i << "]:" << std::endl;
    int number_of_1 = 0, number_of_non1 = 0, vid = -1;
    for (int j = 0; j < m - 1; j++)
      if (nearbyint(equations[i][j]) == 1) {
        number_of_1++;
        vid = j;
      } else if (nearbyint(equations[i][j]) != 0)
        number_of_non1++;
    if (number_of_non1) continue;
    if (number_of_1 != 1) continue;
    int sol = nearbyint(equations[i][m - 1]);
    if (sol == error_status_of_nearint) continue;
    assert(sol == 0 || sol == 1);
    assert(vid >= 0);
    assert(vid < variaID_to_position.size());
    std::pair<int, int> pos = variaID_to_position[vid];
    if (map_status[pos.first][pos.second] != 0) continue;
    if (sol == 0) {
      map_status[pos.first][pos.second] = 1;
      no_mine_block_to_be_clicked.push(pos);
      // std::cout << "push (" << pos.first << "," << pos.second << ")"
      //           << std::endl;
    }
    if (sol == 1) {
      map_status[pos.first][pos.second] = -1;
      // std::cout << "set (" << pos.first << "," << pos.second << ")"
      //           << std::endl;
    }
  }
}
/**
 * @brief The definition of function PreProcessData()
 *
 * @details This function is designed to preprocess the data of the game map
 * immedietly after reading it.
 * It will check unknown blocks and use Gaussian Elimination to find if there
 * is any block that definitely has no mine or has mine. If there is a block
 * definitely has no mine, it will push the block into
 * no_mine_block_to_be_clicked.
 * Note that if some block is found to be definitely has no mine or has mine,
 * it will be marked as known even if it is not clicked.
 */
void PreProcessData() {
  using namespace Client;
  // scan the game_map and mark clicked block in map_status
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++)
      if (game_map[i][j] != '?') {
        assert(game_map[i][j] >= '0' && game_map[i][j] <= '8');
        map_status[i][j] = 2;
      }
  // scan the map and process the simplest case
  ProcessSimpleCase();
  // 1.find all unkown blocks that are adjacnent to clicked blocks and prepare
  // for Gaussian-Jordan Elimination.
  // 2. start Gaussian-Jordan Elimination
  // 3. interpret the result of Gaussian-Jordan Elimination,store the result in
  // map_status and push the newly found block that definitely has no mine
  // into no_mine_block_to_be_clicked
  std::vector<std::vector<double> > equations = GenerateEquations();
  equations = GaussianJordanElimination(equations);
  InterpretResult(equations);
}
/**
 * @brief The definition of function TotalRandomGuess()
 *
 * @details This function is designed to make a total random guess when there
 * is no definite none-mine block to be clicked. Note that this function is
 * just used temporarily before a better algorithm is designed.
 */
std::pair<int, int> TotalRandomGuess() {
  using namespace Client;
  // std::cout << "TotalRandomGuess" << std::endl;
  std::uniform_int_distribution<int> row_dis(0, rows - 1),
      column_dis(0, columns - 1);
  int row = row_dis(RawRnd), column = column_dis(RawRnd);
  while (map_status[row][column] != 0) {
    row = row_dis(RawRnd);
    column = column_dis(RawRnd);
  }
  return std::make_pair(row, column);
}
/**
 * @brief The definition of function MakeBestGuess()
 *
 * @details This function is designed to make the best guess when there is no
 * definite none-mine block to be clicked.
 */
std::pair<int, int> MakeBestGuess() {
  using namespace Client;
  // just make a total random guess before a better algorithm is designed
  return TotalRandomGuess();
  return std::make_pair(0, 0);
}
/**
 * @brief The definition of function GenerateNextStep()
 *
 * @details This function is designed to generate the next step when playing
 * the client's (or player's) role.
 */
std::pair<int, int> GenerateNextStep() {
  using namespace Client;
  if (!no_mine_block_to_be_clicked.empty()) {
    std::pair<int, int> next_step = no_mine_block_to_be_clicked.front();
    no_mine_block_to_be_clicked.pop();
    return next_step;
  } else
    return MakeBestGuess();
}
}  // namespace Client
void ReadMap() {
  using namespace Client;
  for (int i = 0; i < rows; i++) {
    std::cin >> game_map[i];
    assert(strlen(game_map[i]) == columns);
  }
  PreProcessData();
}

/**
 * @brief The definition of function Decide()
 *
 * @details This function is designed to decide the next step when playing the
 * client's (or player's) role. Open up your mind and make your decision here!
 */
void Decide() {
  using namespace Client;
  while (true) {
    std::pair<int, int> next_step = GenerateNextStep();
    Execute(next_step.first, next_step.second);
  }
}

#endif