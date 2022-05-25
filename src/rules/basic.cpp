#ifdef RULES_BASIC
#include "../rules.h"

/**N_BIDS = 11
 * N_SUITS = 1
 * N_CARDS = 10 */

char bidname[N_BIDS][80] = {"pass", "1", "2", "3", "4", "5",
                            "6",    "7", "8", "9", "10"};

card suits[N_SUITS] = {deck};

char cardname[N_CARDS][80] = {" 1", " 2", " 3", " 4", " 5",
                              " 6", " 7", " 8", " 9", "10"};

int size_hand = 5;

list<int> biddable(list<int> bids) {
  int min_bid = 0;
  for (int b : bids)
    if (b > 0) {
      min_bid = b;
      break;
    }
  list<int> res;
  for (int b = min_bid + 1; b < N_BIDS; b++) res.push_back(b);
  res.push_back(0);
  return res;
}

int next_bid_turn(Game game) { return (game.turn + 1) % N_PLAYERS; }

bool end_bidding(Game *game) {
  int last_bid = 0;
  int n_passes = 0;
  for (int b : game->bids) {
    if (b) {
      last_bid = b;
      break;
    } else
      n_passes++;
  }

  if (n_passes >= N_PLAYERS - 1) {
    game->trump = 0;
    game->contract = last_bid;
    game->declarer = game->turn;
    for (int i = 0; i < N_PLAYERS; i++) game->team[i] = i % 2;
    game->leader = game->declarer;
    game->turn = game->leader;
    return true;
  }
  return false;
}

card sort(card *hand, card trump) { return *hand; };

card unsort(card *hand, card trump) { return *hand; };

card playable(card hand, Game game) { return hand; }

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

bool end_trickgame(Game *game) { return (game->round >= N_ROUNDS); }

bool won(int p, Game game) {
  if (game.team[p] == game.team[game.declarer])
    return game.points[game.declarer] >= game.contract;
  return game.points[game.declarer] < game.contract;
}

#endif