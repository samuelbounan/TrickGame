#include "play.h"

void deal_hands(Player *player) {
  list<int> distrib(N_PLAYERS, size_hand);
  list<card> hands = deal(deck, distrib);
  int i = 0;
  for (auto hand : hands) {
    player[i].hand = hand;
    for (int j = 0; j < N_PLAYERS; j++) player[i].have_not[j] = hand;
    player[i].have_not[i] = ~hand;
    i++;
  }
}

void update_bid(Game *game, Player *player, int bid) {
  game->newBid(bid, next_bid_turn(*game));
}

void bidding(Game *game, Player *player, bool printing) {
  while (!end_bidding(game)) {
    int b = player[game->turn].playBid(*game);
    if (printing) {
      cout << "P" << game->turn << " bids ";
      print_bid({b});
    }
    update_bid(game, player, b);
  }
  if (printing) cout << endl;
}

void update_card(Game *game, Player *player, card c) {
  // forfeiting
  if (c == 0) {
    game->round = N_ROUNDS;
    return;
  }
  // update players knowledge and game
  for (int p = 0; p < N_PLAYERS; p++) player[p].updateBelief(*game, c);
  game->newTurn(c);
  // end of a trick
  if (game->trick.size() == N_PLAYERS) {
    int winner = winner_trick(*game);
    int pts = points_trick(*game);
    game->newRound(winner, pts);
  }
}

void trickgame(Game *game, Player *player, bool printing) {
  if (printing) print(*game, player);
  while (!end_trickgame(game)) {
    card c = player[game->turn].playCard(*game);
    if (printing) {
      cout << "P" << game->turn << " plays ";
      print_card(c, game->trump);
      cout << endl;
    }
    update_card(game, player, c);
    if (printing) print(*game, player);
  }
  if (printing) {
    if (won(game->declarer, *game))
      cout << "delarer won with ";
    else
      cout << "declarer lost with ";
    cout << game->points[game->declarer] << " points" << endl;
  }
}

void print(Game game, Player *player) {
  cout << "Contract in P" << game.declarer << "(" << game.points[game.declarer]
       << "): ";
  print_bid({game.contract});
  cout << "Leader: P" << game.leader << endl;
  cout << "Trick" << game.round << ": ";
  print_trick(game.trick, game.trump);
  for (int i = 0; i < N_PLAYERS; i++) {
    cout << "P" << i << ": ";
    print_card(player[i].hand, game.trump);
  }
  cout << endl;
}
