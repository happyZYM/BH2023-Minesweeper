#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

#include "client.h"
#include "server.h"

/**
 * @brief The implementation of function CheckResult
 * @details Only for debug. It checks the map_status and orginal map to see if
 * the result of Gauss-Jordan Elimination is correct.
 */
void CheckResult() {
#define ERROR_IN_RESULT 0
  using namespace Client;
  using namespace Server;
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++) {
      if ('0' <= visible_map[i][j] && visible_map[i][j] <= '8')
        assert(map_status[i][j] == 2);
      if (visible_map[i][j] == '?') assert(map_status[i][j] != 2);
      if (map_status[i][j] == 2)
        assert('0' <= visible_map[i][j] && visible_map[i][j] <= '8');
      if (map_status[i][j] == 0) assert(visible_map[i][j] == '?');
      if (map_status[i][j] == 1) assert(origin_map[i][j] == '.');
      if (map_status[i][j] == -1) assert(origin_map[i][j] == 'X');
    }
#undef ERROR_IN_RESULT
}
/**
 * @brief The implementation of function Execute
 * @details Use it only when trying advanced task. Do NOT modify it before
 * discussing with TA.
 */
void Execute(int row, int column) {
  CheckResult();
  std::string str;
  VisitBlock(row, column);
  if (game_state != 0) {
    // PrintMap(); // this line may help you debug
    ExitGame();
  }
  std::ostringstream oss;
  std::streambuf *old_output_buffer = std::cout.rdbuf();
  std::cout.rdbuf(oss.rdbuf());
  // Here, we redirect the output stream to the string stream.
  // By this way the output of PrintMap() would be stored in the string.
  // If you do not understand, you can try to compare it with freopen, which
  // redirect the output stream to a file.
  PrintMap();
  std::cout.rdbuf(old_output_buffer);  // Restore the output buffer
  str = oss.str();                     // Read the output
  std::istringstream iss(str);  // Redirect the input to the string, which
                                // stores the output recently
  std::streambuf *old_input_buffer = std::cin.rdbuf();
  std::cin.rdbuf(iss.rdbuf());  // Redirect the input to the string, which
                                // stores the output recently
  ReadMap();
  std::cin.rdbuf(old_input_buffer);  // Restore the input buffer
  //   PrintMap();                        // These two lines may help you debug
  //   std::cout << std::endl;
}

int main() {
  InitMap();
  // std::cout << rows << " " << columns << std::endl;
  InitGame();
  while (true) {
    Decide();  // Exit() will be called in this function
  }
}