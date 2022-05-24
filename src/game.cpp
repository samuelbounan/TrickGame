#include "game.h"

void Game::newBid(int b, int p) {
  bids.push_front(b);
  turn = p;
}

void Game::newTurn(card c) {
  trick.push_back(c);
  played[turn][round] = c;
  remaining &= ~c;
  turn = (turn + 1) % N_PLAYERS;
}

void Game::newRound(int winner, int pts) {
  for (int i = 0; i < N_PLAYERS; i++)
    if (team[winner] == team[i]) points[i] += pts;
  trick.clear();
  round++;
  leader = winner;
  turn = leader;
}