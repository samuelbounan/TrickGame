#include "main.h"

using namespace std;

int main() {
  // initialization
  Player player[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) {
    player[i].id = i;
  }
  player[0].strategy = &aos;
  player[1].strategy = &aos;
  Game game;

  // playing
  deal_hands(player);
  for (int i = 0; i < N_PLAYERS; i++)
    for (int j = 0; j < N_PLAYERS; j++) player[i].have_not[j] = ~player[j].hand;

  bidding(&game, player);
  trickgame(&game, player, true);
  return 0;
}