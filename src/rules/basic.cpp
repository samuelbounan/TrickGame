#ifdef RULES_BASIC
#include "../rules.h"

/** N_SUITS = 1
 * N_CARDS = 10 */

card suits[N_SUITS] = {deck};

char cardname[N_CARDS][80] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

bool end_bidding(Game *game) {
  game->trump = 0;
  for (int i = 0; i < N_PLAYERS; i++) game->team[i] = i % 2;
  game->declarer = game->leader;
  game->turn = game->leader;
  return true;
}

card sort(card hand, const card &trump) { return hand; }

card unsort(card hand, const card &trump) { return hand; }

card legal(const card &hand, const Game &game) { return hand; }

int winner_trick(const Game &game) {
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

int points_card(const card &c) { return 0; }

int points_trick(const Game &game) { return 1; }

bool end_trickgame(Game *game) { return (game->round >= N_ROUNDS); }

#endif