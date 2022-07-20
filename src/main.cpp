#include "main.h"

using namespace std;

int main() {
  // initialization
  Player player[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) {
    player[i].id = i;
    if (i % 2 == 0)
      player[i].trickstrat = &voting;
    else
      player[i].trickstrat = &voting;
  }
  for (int seed = 3; seed < 4; seed++) {
    // playing
    cout << seed << ": ";
    deal_hands(player, seed);
    card hands[N_PLAYERS];
    for (int p = 0; p < N_PLAYERS; p++) hands[p] = player[p].hand;
    for (int rot = 0; rot < 2; rot++) {
      Game game(rot);
      for (int p = 0; p < N_PLAYERS; p++) {
        player[p].hand = hands[(p - rot + N_PLAYERS) % N_PLAYERS];
        for (int i = 0; i < N_PLAYERS; i++)
          player[i].have_not[p] = ~player[p].hand;
      }
      bidding(game, player, PRINTING);
      trickgame(game, player, PRINTING);
    }
  }
  return 0;
}