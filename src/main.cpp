#include "main.h"

using namespace std;

int main() {
  // initialization
  Player player[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) {
    player[i].id = i;
  }
  player[0].strategy = &interactive_card;
  player[0].bidstrat = &interactive_bid;
  Game game;

  // playing
  deal_hands(player);
  // for (int i = 0; i < N_PLAYERS; i++)
  // for (int j = 0; j < N_PLAYERS; j++) player[i].have_not[j] =
  // ~player[j].hand;

  bidding(&game, player, 2);
  trickgame(&game, player, 4);
  return 0;
}