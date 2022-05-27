#include "main.h"

using namespace std;

int main() {
  // initialization
  Player player[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) {
    player[i].id = i;
  }
  player[0].trickstrat = &aos;
  player[1].trickstrat = &aos;
  Game game;

  // playing
  deal_hands(player);
  bidding(&game, player, 2);
  trickgame(&game, player, 4);
  return 0;
}