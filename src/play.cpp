#include "play.h"

void deal_hands(Player *player, int seed) {
  list<int> distrib(N_PLAYERS, SIZE_HAND);
  list<card> hands = deal(deck, distrib, seed);
  int i = 0;
  for (auto hand : hands) {
    player[i].hand = hand;
    i++;
  }
}

void update_bid(Game &game, int bid) {
  game.newBid(bid, next_bid_turn(game));
}

pair<int, int> update_card(Game &game, const card &c) {
  // forfeiting
  if (c == 0) {
    cout << "forfeit" << endl;
    exit(0);
    game.round = N_ROUNDS;
    return {0, game.turn};
  }
  int pts = 0;
  int p = game.turn;
  game.newTurn(c);
  // end of a trick
  if (game.turn == game.leader) {
    int winner = winner_trick(game);
    pts = points_trick(game);
    game.newRound(winner, pts);
  }
  return {pts, p};
}

void print(const Game &game, Player *player) {
  cout << "Declarer P" << game.declarer << ": " << game.min_points << "-"
       << game.max_points << " round " << game.round << endl;
  for (int i = 0; i < N_PLAYERS; i++) {
    cout << "P" << i << ": ";
    print_card(player[i].hand, game.trump);
  }
  cout << endl;
}
