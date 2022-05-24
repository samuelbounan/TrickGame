#include "play.h"

void deal_hands(Player *player) {
  list<int> distrib(N_PLAYERS, SIZE_HAND);
  list<card> hands = deal(deck, distrib);
  int i = 0;
  for (auto hand : hands) {
    player[i].hand = hand;
    for (int j = 0; j < N_PLAYERS; j++) player[i].have_not[j] = hand;
    player[i].have_not[i] = ~hand;
  }
}

void bidding(Game *game, Player *player) {
  trivial_bid(game);
  for (int i = 0; i < N_PLAYERS; i++) {
    sort(&player[i].hand, game->trump);
  }
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
  while (game->round < N_ROUNDS) {
    card c = player[game->turn].playCard(*game);
    if (printing) {
      cout << "P" << game->turn << " plays ";
      print_card(c, game->trump);
      cout << endl;
    }
    update_card(game, player, c);
    if (printing) print(*game, player);
  }
  finish(game);
}

void print(Game game, Player *player) {
  cout << "Leader: P" << game.leader << endl;
  cout << "Trick" << game.round << ": ";
  print_trick(game.trick, game.trump);
  for (int i = 0; i < N_PLAYERS; i++) {
    cout << "P" << i << ": ";
    print_card(player[i].hand, game.trump);
  }
  cout << endl;
}
