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

void Game::removeCard() {
  if (trick.empty()) {
    round--;
    turn = (leader + 1) % N_PLAYERS;
    leader = turn;
    trick = played[round];
  }
  remaining |= trick.back();
  trick.pop_back();
  turn = (turn - 1) % N_PLAYERS;
}