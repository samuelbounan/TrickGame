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

void update_bid(Game *game, Player *player, int bid) {
  game->newBid(bid, next_bid_turn(*game));
}

void bidding(Game *game, Player *player, int printing) {
  game->turn = game->leader;
  while (!end_bidding(game)) {
    int b = player[game->turn].playBid(*game);
    if (printing >= 2) {
      cout << "P" << game->turn << " bids ";
      print_bid({b});
      cout << endl;
    }
    update_bid(game, player, b);
  }
  for (int p = 0; p < N_PLAYERS; p++) {
    player[p].hand = sort(player[p].hand, game->trump);
    for (int i = 0; i < N_PLAYERS; i++)
      player[p].have_not[i] = sort(player[p].have_not[i], game->trump);
  }
  if (printing >= 2) cout << endl;
}

pair<int, int> update_card(Game *game, card c) {
  // forfeiting
  if (c == 0) {
    game->round = N_ROUNDS;
    return {0, game->turn};
  }
  int pts = 0;
  int p = game->turn;
  game->newTurn(c);
  // end of a trick
  if (game->trick.size() == N_PLAYERS) {
    int winner = winner_trick(*game);
    pts = points_trick(*game);
    game->newRound(winner, pts);
  }
  return {pts, p};
}

void trickgame(Game *game, Player *player, int printing) {
  while (!end_trickgame(game)) {
    if (printing == 3 && game->trick.empty())
      print(*game, player);
    else if (printing >= 4) {
      game->print();
      for (int i = 0; i < N_PLAYERS; i++) player[i].print(game->trump);
    }
    card c = player[game->turn].playCard(*game);
    if (printing >= 2) {
      cout << endl << "P" << game->turn << " plays ";
      print_card(c, game->trump);
      cout << endl;
      if (game->trick.size() >= N_PLAYERS - 1) cout << endl;
    }
    // update players knowledge and game
    for (int p = 0; p < N_PLAYERS; p++) player[p].updateBelief(*game, c);
    update_card(game, c);
  }
  if (printing > 0)
    cout << "score: " << score(*game, game->team[game->declarer]) << endl;
}

void print(Game game, Player *player) {
  cout << "Declarer P" << game.declarer << " - " << game.points[game.declarer]
       << "/";
  print_bid({game.contract});
  cout << endl << "round " << game.round << endl;
  for (int i = 0; i < N_PLAYERS; i++) {
    cout << "P" << i << ": ";
    print_card(player[i].hand, game.trump);
  }
  cout << endl;
}
