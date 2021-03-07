#include <iostream>
#include <vector>

typedef long long ll;
typedef unsigned long long ull;

using namespace std;

const int INF = 1e9;

char my_colour, opp_colour;

struct board {
  vector <vector <char>> state;
  board() {
    state = vector <vector <char>>(8, vector <char>(8, ' '));
  }
  board(vector <vector <char>> old_state) {
    state = old_state;
  }
  void print() {
    cout << ' ';
    for (int i = 0; i < 8; i++)
      cout << '-';
    cout << '\n';
    for (int i = 0; i < 8; i++) {
      cout << '|';
      for (int j = 0; j < 8; j++)
        cout << state[i][j];
      cout << '|';
      cout << '\n';
    }
    cout << ' ';
    for (int i = 0; i < 8; i++)
      cout << '-';
    cout << endl;
  }
  void make_main() {
    state[3][3] = state[4][4] = '1';
    state[3][4] = state[4][3] = '0';
  }
  int evaluation() {
    int sum = 0;
    for (int i = 0; i < 8; i++)
      for (int j = 0; j < 8; j++) {
        if (state[i][j] == ' ')
          continue;
        if (state[i][j] == my_colour)
          sum++;
        else
          sum--;
      }
    return sum;
  }
  bool check(int row, int col) {
    return (0 <= row && row <= 7) && (0 <= col && col <= 7);
  }
  pair <bool, board> make_move(char colour, int row, int col) {
    if (state[row][col] != ' ')
      return {false, {}};
    board board_new(state);
    bool flag = false;
    for (int e_row = -1; e_row <= 1; e_row++)
      for (int e_col = -1; e_col <= 1; e_col++) {
        if (e_col == 0 && e_row == 0)
          continue;
        vector <pair <int, int>> to_capture;
        for (int k = 1; check(row + e_row * k, col + e_col * k); k++) {
          char ch = state[row + e_row * k][col + e_col * k];
          if (ch == ' ')
            break;
          if (ch == colour) {
            if (k != 1)
              flag = true;
            for (auto &[x, y] : to_capture)
              board_new.state[x][y] = colour;
            break;
          } else {
            to_capture.push_back({row + e_row * k, col + e_col * k});
          }
        }
      }
    board_new.state[row][col] = colour;
    return {flag, board_new};
  }
};

int best_row, best_col;

int minimax(board state_cur, char colour, int depth, int alpha, int beta, int critical_depth) {
  if (depth == critical_depth)
    return (depth & 1 ? -1 : 1) * state_cur.evaluation();
  int value = -INF;
  bool is_there_legit_move = false;
  for (int row = 0; row < 8; row++)
    for (int col = 0; col < 8; col++) {
      auto now = state_cur.make_move(colour, row, col);
      if (!now.first)
        continue;
      is_there_legit_move = true;
      int eval = -minimax(now.second, ('0' + '1') - colour, depth + 1, -beta, -alpha, critical_depth);
      if (depth == 0 && eval > value)
        best_row = row, best_col = col;
      value = max(eval, value);
      alpha = max(alpha, value);
      if (alpha >= beta)
        break;
    }
  if (!is_there_legit_move)
    value = -minimax(state_cur, ('0' + '1') - colour, depth + 1, -beta, -alpha, critical_depth);
  return value;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  board main_board;
  main_board.make_main();
  string first_or_second;
  cin >> first_or_second;
  if (first_or_second == "second") {
    my_colour = '1', opp_colour = '0';
    int row, col;
    cin >> row >> col;
    main_board.make_move(opp_colour, row, col);
  } else {
    my_colour = '0', opp_colour = '1';
  }
  while (true) {
    best_row = -1, best_col = -1;
    minimax(main_board, my_colour, 0, -INF, INF, 7);
    if (best_row == -1) {
      cout << "Skip" << endl;
    } else {
      cout << best_row + 1 << ' ' << best_col + 1 << endl;
      main_board = main_board.make_move(my_colour, best_row, best_col).second;
    }
    main_board.print();
    int row, col;
    cin >> row >> col;
    if (row != -1)
      main_board = main_board.make_move(opp_colour, row - 1, col - 1).second;
    main_board.print();
  }
  return 0;
}
