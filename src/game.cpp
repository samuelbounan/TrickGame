#include "game.h"

void Game::newTurn(const card& c) {
  trick[turn] = c;
  remaining &= ~c;
  turn = (turn + 1) % N_PLAYERS;
}

void Game::newRound(int winner, int pts) {
  points[team[winner]] += pts;
  copy_n(trick, N_PLAYERS, played[round]);
  round++;
  leader = winner;
  turn = leader;
}

void Game::removeCard(const pair<int, int>& info) {
  if (turn == leader) {
    round--;
    points[team[leader]] -= info.first;
    turn = info.second;
    leader = (turn + 1) % N_PLAYERS;
    copy_n(played[round], N_PLAYERS, trick);
  } else
    turn = (turn - 1 + N_PLAYERS) % N_PLAYERS;
  remaining |= trick[turn];
}

void Game::print() {
  cout << "GAME: dec " << declarer;
  cout << "/ tru ";
  print_vector(trump);
  cout << "points [" << points[0] << ", " << points[1] << "]/ ";
  cout << "rnd " << round;
  cout << "/ lea " << leader;
  cout << "/ trn " << turn;
  cout << "/ rem ";
  print_vector(remaining);
  cout << "trick:" << endl;
  for (int i = 0; i < N_PLAYERS; i++) print_vector(trick[i]);
  cout << endl;
}