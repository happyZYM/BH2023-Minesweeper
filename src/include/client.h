#ifndef CLIENT_H
#define CLIENT_H

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iostream>
#include <queue>
#include <random>
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
/**
 * @brief The definition of function GenerateEquations()
 *
 * @details This function is designed to scan the game_map and map_status to
 * generate the equations that will be used in Gaussian-Jordan Elimination.
 * It returns a vector<vector<double>> equations, where equations[i] is the i th equation.
 */

/**
 * @brief The definition of function GaussianJordanElimination()
 * @details This function is designed to use Gaussian-Jordan Elimination to
 * solve the equations. It returns the processed vector<vector<double>>
 * &equations
 * @param vector<vector<double>> &equations The equations to be solved
 */
const double eps = 1e-8;
std::vector<std::vector<double> > &GaussianJordanElimination(
    std::vector<std::vector<double> > &equations) {
  using std::abs;
  int n = equations.size(), m = equations[0].size();
  assert(n + 1 == m);
  for (int i = 0; i < n; i++) {
    int pivot = i;
    for (int j = i + 1; j < n; j++)
      if (abs(equations[j][i]) > abs(equations[pivot][i])) pivot = j;
    std::swap(equations[i], equations[pivot]);
    if (abs(equations[i][i]) < eps) continue;
    const double pivot_value = equations[i][i];
    for (int j = 0; j < n; j++) equations[i][j] /= pivot_value;
    for (int j = 0; j < n; j++)
      if (j != i) {
        const double tmp = equations[j][i];
        for (int k = 0; k < n; k++) equations[j][k] -= tmp * equations[i][k];
      }
  }
  return equations;
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
  // find all unkown blocks that are adjacnent to clicked blocks and prepare
  // for Gaussian-Jordan Elimination.

  // start Gaussian-Jordan Elimination

  // interpret the result of Gaussian-Jordan Elimination,store the result in
  // map_status and push the newly found block that definitely has no mine
  // into no_mine_block_to_be_clicked
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
  std::uniform_int_distribution<int> row_dist(0, rows - 1),
      column_dist(0, columns - 1);
  int row = row_dist(RawRnd), column = column_dist(RawRnd);
  while (map_status[row][column] != 0) {
    row = row_dist(RawRnd);
    column = column_dist(RawRnd);
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