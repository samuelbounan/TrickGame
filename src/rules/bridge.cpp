#ifdef RULES_BRIDGE
#include "../rules.h"

/* N_CARDS = 10, N_SUITS = 1 */

card clubs = (ONE << (N_CARDS / N_SUITS)) - 1;
card diamo = clubs << (N_CARDS / N_SUITS);
card heart = diamo << (N_CARDS / N_SUITS);
card spade = heart << (N_CARDS / N_SUITS);
card suits[N_SUITS] = {clubs, diamo, heart, spade};

char cardname[N_CARDS][80] = {
    "\u26632", "\u26633", "\u26634", "\u26635", "\u26636", "\u26637", "\u26638",
    "\u26639", "\u2663T", "\u2663J", "\u2663Q", "\u2663K", "\u2663A",

    "\u25C62", "\u25C63", "\u25C64", "\u25C65", "\u25C66", "\u25C67", "\u25C68",
    "\u25C69", "\u25C6T", "\u25C6J", "\u25C6Q", "\u25C6K", "\u25C6A",

    "\u26652", "\u26653", "\u26654", "\u26655", "\u26656", "\u26657", "\u26658",
    "\u26659", "\u2665T", "\u2665J", "\u2665Q", "\u2665K", "\u2665A",

    "\u26602", "\u26603", "\u26604", "\u26605", "\u26606", "\u26607", "\u26608",
    "\u26609", "\u2660T", "\u2660J", "\u2660Q", "\u2660K", "\u2660A"};

int winner_trick(Game game) {
  if (game.trick.empty()) return game.leader;
  card best_card = game.trick.front();
  int p = 0;
  int winner = 0;
  for (card c : game.trick) {
    if (c > best_card) {
      best_card = c;
      winner = p;
    }
    p++;
  }
  return (winner + game.leader) % N_PLAYERS;
}
int points_trick(Game game) { return 1; }

card playable(card hand, Game game) { return hand; }
void finish(Game *game){};
card sort(card *hand, card trump) { return *hand; };
card unsort(card *hand, card trump) { return *hand; };
void forfeit(Game *game) {
  for (int p = 0; p < N_PLAYERS; p++) {
    if (game->team[p] == game->team[game->turn])
      game->points[p] = -1;
    else
      game->points[p] = game->contract;
  }
}
bool won(int p, Game game) {
  if (game.team[p] == game.team[game.declarer])
    return game.points[game.declarer] >= game.contract;
  return game.points[game.declarer] < game.contract;
}

#endif