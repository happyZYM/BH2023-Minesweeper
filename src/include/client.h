#ifndef CLIENT_H
#define CLIENT_H

#include <cassert>
#include <cstring>
#include <iostream>
#include <queue>
#include <random>
#include <utility>

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
 * @brief The definition of function PreProcessData()
 *
 * @details This function is designed to preprocess the data of the game map
 * immedietly after reading it.
 * It will check unknown blocks and use Gaussian Elimination to find if there is
 * any block that definitely has no mine or has mine. If there is a block
 * definitely has no mine, it will push the block into
 * no_mine_block_to_be_clicked.
 * Note that if some block is found to be definitely has no mine or has mine, it
 * will be marked as known even if it is not clicked.
 */
void PreProcessData() {
  using namespace Client;
  // scan the game_map and mark clicked block in map_status
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++)
      if (game_map[i][j] != '?') {
        assert(game_map[i][j] >= '0' && game_map[i][j] <= '8');
        map_status[i][j] = 2;
        // special process for 0 before Gaussian Elimination starts
        if (game_map[i][j] == '0') {
          for (int k = i - 1; k <= i + 1; k++)
            for (int l = j - 1; l <= j + 1; l++)
              if (k >= 0 && k < rows && l >= 0 && l < columns &&
                  map_status[k][l] == 0) {
                map_status[k][l] = 1;
                no_mine_block_to_be_clicked.push(std::make_pair(k, l));
              }
        }
      }
  //find all unkown blocks that are adjacnent to clicked blocks and prepare for Gaussian Elimination
  //start Gaussian Elimination 
}
/**
 * @brief The definition of function MakeBestGuess()
 *
 * @details This function is designed to make the best guess when there is no
 * definite none-mine block to be clicked.
 */
std::pair<int, int> MakeBestGuess() {
  using namespace Client;
  return std::make_pair(0, 0);
}
/**
 * @brief The definition of function GenerateNextStep()
 *
 * @details This function is designed to generate the next step when playing the
 * client's (or player's) role.
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