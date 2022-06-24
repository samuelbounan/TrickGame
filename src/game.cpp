#include "game.h"

void Game::newBid(int b, int p) {
  bids.push_front(b);
  turn = p;
}

void Game::newTurn(card c) {
  trick.push_back(c);
  remaining &= ~c;
  turn = (turn + 1) % N_PLAYERS;
}

void Game::newRound(int winner, int pts) {
  for (int i = 0; i < N_PLAYERS; i++)
    if (team[winner] == team[i]) points[i] += pts;
  list<card> last_trick(trick);
  played[round] = last_trick;
  trick.clear();
  round++;
  leader = winner;
  turn = leader;
}

void Game::removeCard(pair<int, int> info) {
  if (trick.empty()) {
    round--;
    for (int i = 0; i < N_PLAYERS; i++)
      if (team[leader] == team[i]) points[i] -= info.first;
    turn = info.second;
    leader = (turn + 1) % N_PLAYERS;
    trick = played[round];
  } else
    turn = (turn - 1 + N_PLAYERS) % N_PLAYERS;
  remaining |= trick.back();
  trick.pop_back();
}

void Game::print() {
  cout << "GAME: con " << contract;
  cout << "/ dec " << declarer;
  cout << "/ tru ";
  print_vector(trump);
  cout << "points [";
  for (int i = 0; i < N_PLAYERS; i++) {
    cout << points[i];
    if (i != N_PLAYERS - 1)
      cout << ",";
    else
      cout << "]/ ";
  }
  cout << "rnd " << round;
  cout << "/ lea " << leader;
  cout << "/ trn " << turn;
  cout << "/ rem ";
  print_vector(remaining);
  cout << "trick:" << endl;
  for (auto c : trick) print_vector(c);
  cout << endl;
}