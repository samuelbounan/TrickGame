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

void bidding(Game *game, Player *player, int printing) {
  end_bidding(game);
  for (int p = 0; p < N_PLAYERS; p++) {
    player[p].hand = sort(player[p].hand, game->trump);
    for (int i = 0; i < N_PLAYERS; i++)
      player[p].have_not[i] = sort(player[p].have_not[i], game->trump);
  }
  if (printing >= 2) cout << endl;
}

pair<int, int> update_card(Game *game, const card &c) {
  // forfeiting
  if (c == 0) {
    game->round = N_ROUNDS;
    return {0, game->turn};
  }
  int pts = 0;
  int p = game->turn;
  game->newTurn(c);
  // end of a trick
  if (game->turn == game->leader) {
    int winner = winner_trick(*game);
    pts = points_trick(*game);
    game->newRound(winner, pts);
  }
  return {pts, p};
}

void trickgame(Game *game, Player *player, int printing) {
  while (!end_trickgame(game)) {
    if (printing >= 3 && game->turn == game->leader)
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
      if ((game->turn + 1 - game->leader) % N_PLAYERS == 0) cout << endl;
    }
    // update players knowledge and game
    for (int p = 0; p < N_PLAYERS; p++) player[p].updateBelief(*game, c);
    update_card(game, c);
  }
  if (printing > 0)
    cout << "score: " << game->points[game->team[game->declarer]] << endl;
}

void print(const Game &game, Player *player) {
  cout << "Declarer P" << game.declarer << " - "
       << game.points[game.team[game.declarer]] << endl;
  cout << endl << "round " << game.round << endl;
  for (int i = 0; i < N_PLAYERS; i++) {
    cout << "P" << i << ": ";
    print_card(player[i].hand, game.trump);
  }
  cout << endl;
}
