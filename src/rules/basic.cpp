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
  int winner = -1;
  card best_card = 0;
  for (int i = 0; i < N_PLAYERS; i++) {
    int p = (game.leader + i) % N_PLAYERS;
    if (trick[p] > best_card) {
      best_card = c;
      winner = p;
    }
  }
  return winner;
}

int points_card(const card &c) { return 0; }

int points_trick(const Game &game) { return 1; }

bool end_trickgame(Game *game) { return (game->round >= N_ROUNDS); }

#endif