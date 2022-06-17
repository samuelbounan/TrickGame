#include "main.h"

using namespace std;

int main() {
  // initialization
  Player player[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) {
    player[i].id = i;
    player[i].trickstrat = &pimc;
  }
  Game game;

  // playing
  deal_hands(player);
  bidding(&game, player, 0);
  trickgame(&game, player, 4);
  return 0;
}