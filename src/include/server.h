#ifndef SERVER_H
#define SERVER_H

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <queue>

/*
 * You may need to define some global variables for the information of the game
 * map here. Although we don't encourage to uss global variables in real cpp
 * projects, you may have to use them because the use of class is not taught
 * yet. However, if you are member of A-class or have learnt the use of cpp
 * class, member functions, etc., you're free to modify this structure.
 */
int rows;        // The count of rows of the game map
int columns;     // The count of columns of the game map
int game_state;  // The state of the game, 0 for continuing, 1 for winning, -1
                 // for losing
namespace Server {
int visit_count, step_count;
const int max_size = 35;
char origin_map[max_size][max_size];   // The original map
char visible_map[max_size][max_size];  // The map that the player can see
int number_of_nearby_mines[max_size][max_size];  // The number of nearby mines
int number_of_all_mines;                         // The number of all mines
}  // namespace Server
/**
 * @brief The definition of function InitMap()
 *
 * @details This function is designed to read the initial map from stdin. For
 * example, if there is a 3 * 3 map in which mines are located at (0, 1) and (1,
 * 2) (0-based), the stdin would be 3 3 .X.
 *     ...
 *     ..X
 * where X stands for a mine block and . stands for a normal block. After
 * executing this function, your game map would be initialized, with all the
 * blocks unvisited.
 */
void InitMap() {
  using namespace std;
  using namespace Server;
  std::cin >> rows >> columns;
  assert(2 <= rows && rows <= 30 && 2 <= columns && columns <= 30);
  for (int i = 0; i < rows; i++) {
    cin >> origin_map[i];
    assert(strlen(origin_map[i]) == columns);
  }
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++) visible_map[i][j] = '?';
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++) {
      for (int dr = -1; dr <= 1; dr++)
        for (int dc = -1; dc <= 1; dc++) {
          int nr = i + dr, nc = j + dc;
          if (nr < 0 || nr >= rows || nc < 0 || nc >= columns) continue;
          if (origin_map[nr][nc] == 'X') number_of_nearby_mines[i][j]++;
        }
    }
  number_of_all_mines = 0;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++)
      if (origin_map[i][j] == 'X') number_of_all_mines++;
  // bug test: output the number of nearby mines
  // for (int i = 0; i < rows; i++) {
  //   for (int j = 0; j < columns; j++) {
  //     cout << number_of_nearby_mines[i][j] << ' ';
  //   }
  //   cout << endl;
  // }
}

/**
 * @brief The definition of function VisitBlock(int, int)
 *
 * @details This function is designed to visit a block in the game map. We take
 * the 3 * 3 game map above as an example. At the beginning, if you call
 * VisitBlock(0, 0), the return value would be 0 (game continues), and the game
 * map would be 1??
 *     ???
 *     ???
 * If you call VisitBlock(0, 1) after that, the return value would be -1 (game
 * ends and the players loses) , and the game map would be 1X?
 *     ???
 *     ???
 * If you call VisitBlock(0, 2), VisitBlock(2, 0), VisitBlock(1, 2) instead, the
 * return value of the last operation would be 1 (game ends and the player
 * wins), and the game map would be 1@1 122 01@
 *
 * @param row The row coordinate (0-based) of the block to be visited.
 * @param column The column coordinate (0-based) of the block to be visited.
 *
 * @note You should edit the value of game_state in this function. Precisely,
 * edit it to 0  if the game continues after visit that block, or that block has
 * already been visited before. 1  if the game ends and the player wins. -1 if
 * the game ends and the player loses.
 */
void VisitBlock(int row, int column) {
  using namespace Server;
  step_count++;
  using namespace std;
  assert(0 <= row && row < rows && 0 <= column && column < columns);
  if (origin_map[row][column] == 'X') {
    game_state = -1;
    visible_map[row][column] = 'X';
  } else {
    assert(game_state == 0);
    if (visible_map[row][column] != '?') return;
    game_state = 0;
    visible_map[row][column] = '0' + number_of_nearby_mines[row][column];
    queue<pair<int, int>> q;
    if (visible_map[row][column] == '0') q.push(make_pair(row, column));
    visit_count++;
    while (!q.empty()) {
      pair<int, int> p = q.front();
      q.pop();
      int x = p.first, y = p.second;
      assert(visible_map[x][y] == '0');
      for (int i = -1; i <= 1; i++)
        for (int j = -1; j <= 1; j++) {
          int nx = x + i, ny = y + j;
          if (nx < 0 || nx >= rows || ny < 0 || ny >= columns) continue;
          if (visible_map[nx][ny] != '?') continue;
          visit_count++;
          visible_map[nx][ny] = '0' + number_of_nearby_mines[nx][ny];
          if (visible_map[nx][ny] == '0') q.push(make_pair(nx, ny));
        }
    }
    int cnt = 0;
    for (int i = 0; i < rows; i++)
      for (int j = 0; j < columns; j++)
        if (visible_map[i][j] == '?') cnt++;
    if (cnt == number_of_all_mines) game_state = 1;
  }
}

/**
 * @brief The definition of function PrintMap()
 *
 * @details This function is designed to print the game map to stdout. We take
 * the 3 * 3 game map above as an example. At the beginning, if you call
 * PrintMap(), the stdout would be
 *    ???
 *    ???
 *    ???
 * If you call VisitBlock(2, 0) and PrintMap() after that, the stdout would be
 *    ???
 *    12?
 *    01?
 * If you call VisitBlock(0, 1) and PrintMap() after that, the stdout would be
 *    ?X?
 *    12?
 *    01?
 * If the player visits all blocks without mine and call PrintMap() after that,
 * the stdout would be 1@1 122 01@ (You may find the global variable game_state
 * useful when implementing this function.)
 *
 * @note Use std::cout to print the game map, especially when you want to try
 * the advanced task!!!
 */
void PrintMap() {
	using namespace Server;
  if (game_state != 1) {
    for (int i = 0; i < rows; i++) {
      std::cout << visible_map[i] << std::endl;
    }
  } else {
    for (int i = 0; i < rows; i++) {
      for (int j = 0; j < columns; j++) {
        if (origin_map[i][j] == 'X') {
          std::cout << '@';
        } else {
          std::cout << number_of_nearby_mines[i][j];
        }
      }
      std::cout << std::endl;
    }
  }
}

/**
 * @brief The definition of function ExitGame()
 *
 * @details This function is designed to exit the game.
 * It outputs a line according to the result, and a line of two integers,
 * visit_count and step_count, representing the number of blocks visited and the
 * number of steps taken respectively.
 */
void ExitGame() {
	using namespace Server;
  assert(game_state != 0);
  if (game_state == 1) {
    std::cout << "YOU WIN!" << std::endl;
    std::cout << visit_count << ' ' << step_count << std::endl;
  } else {
    std::cout << "GAME OVER!" << std::endl;
    std::cout << visit_count << ' ' << step_count << std::endl;
  }
  exit(0);  // Exit the game immediately
}

#endif
