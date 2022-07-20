#include "game.h"

void Game::newTurn(const card& c) {
  trick[turn] = c;
  remaining &= ~c;
  turn = (turn + 1) % N_PLAYERS;
}

void Game::newRound(int winner, int pts) {
  if (team[winner] == team[declarer])
    min_points += pts;
  else
    max_points -= pts;
  copy_n(trick, N_PLAYERS, played[round]);
  round++;
  leader = winner;
  turn = leader;
}

void Game::removeCard(const pair<int, int>& info) {
  if (turn == leader) {
    round--;
    if (team[leader] == team[declarer])
      min_points -= info.first;
    else
      max_points += info.first;
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
  cout << "points [" << min_points << ", " << max_points << "]/ ";
  cout << "rnd " << round;
  cout << "/ lea " << leader;
  cout << "/ trn " << turn;
  cout << "/ rem ";
  print_vector(remaining);
  cout << "trick:" << endl;
  for (int i = 0; i < N_PLAYERS; i++) print_vector(trick[i]);
  cout << endl;
}