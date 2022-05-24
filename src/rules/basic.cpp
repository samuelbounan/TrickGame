#ifdef RULES_BASIC
#include "../rules.h"

/* suitmasks in the current game */
card suits[N_SUITS] = {deck};

char cardname[N_CARDS][80] = {" 1", " 2", " 3", " 4", " 5",
                              " 6", " 7", " 8", " 9", "10"};

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

void trivial_bid(Game *game) {
  game->trump = 0;
  game->contract = (N_ROUNDS / N_PLAYERS) + 1;
  game->declarer = 0;
  for (int i = 0; i < N_PLAYERS; i++) game->team[i] = i % 2;
  game->leader = 0;
  game->turn = 0;
}

card playable(card hand, Game game) { return hand; }

void finish(Game *game){};

card sort(card *hand, card trump) { return *hand; };

card unsort(card *hand, card trump) { return *hand; };

bool won(int p, Game game) {
  if (game.team[p] == game.team[game.declarer])
    return game.points[game.declarer] >= game.contract;
  return game.points[game.declarer] < game.contract;
}

#endif