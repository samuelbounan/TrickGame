#include "main.h"

using namespace std;

int main() {
  // initialization
  Player player[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++) {
    player[i].id = i;
    player[i].trickstrat = &alpha_beta;
  }
  Game game;

  // playing
  deal_hands(player);

  // initialize have_not with the not hands of players
  for (int i = 0; i < N_PLAYERS; i++)
    for (int j = 0; j < N_PLAYERS; j++) player[i].have_not[j] = ~player[j].hand;

  card res[N_PLAYERS];
  for (int i = 0; i < N_PLAYERS; i++)
    print_card(~player[0].have_not[i], game.trump);
  random_world(player[0].have_not, res, game);
  for (int i = 0; i < N_PLAYERS; i++) print_card(~res[i], game.trump);
  // bidding(&game, player, 2);
  // trickgame(&game, player, 4);
  return 0;
}