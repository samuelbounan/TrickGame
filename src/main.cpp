#include "main.h"

using namespace std;

int main() {
  // initialization
  Player player[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) {
    player[i].id = i;
    player[i].trickstrat = &pimc;
  }
  for (int seed = 0; seed < 100; seed++) {
    // playing
    deal_hands(player, seed);
    card hands[N_PLAYERS];
    for (int p = 0; p < N_PLAYERS; p++) hands[p] = player[p].hand;
    for (int rot = 0; rot < 2; rot++) {
      Game game(rot);
      for (int p = 0; p < N_PLAYERS; p++) {
        for (int i = 0; i < N_PLAYERS; i++) player[p].have_not[i] = 0;
        player[p].hand = hands[(p - rot + N_PLAYERS) % N_PLAYERS];
      }
      bidding(&game, player, PRINTING);
      trickgame(&game, player, PRINTING);
    }
  }
  return 0;
}