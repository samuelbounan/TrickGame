#include "main.h"

#include "fox/fox.h"

using namespace std;

int main() {
  AI* ai[N_PLAYERS];
  Player player[N_PLAYERS];
  Game game;
  int printing = 4;
  for (int i = 0; i < N_PLAYERS; i++)
    if (i % 2 == 0)
      ai[i] = new Human;
    else
      ai[i] = new CardAI;

  // deal hands
  list<int> distrib(N_PLAYERS, SIZE_HAND);
  list<card> hands = deal(deck, distrib);
  int i = 0;
  for (auto hand : hands) {
    ai[i]->StartGame(i, hand);
    player[i].id = i;
    player[i].hand = hand;
    i++;
  }
  // bidding
  while (!end_bidding(&game)) {
    int last_bid = -1;
    if (!game.bids.empty()) last_bid = game.bids.front();
    int b = ai[game.turn]->PlayBid(last_bid);
    if (printing >= 2) {
      cout << "P" << game.turn << " bids ";
      print_bid({b});
      cout << endl;
    }
    update_bid(&game, b);
  }
  if (printing >= 2) cout << endl;

  card trashskat;
  int trashbid[N_PLAYERS];
  GameDeclaration gd;
  gd.trumpmask = game.trump;
  gd.contract = game.contract;
  for (int i = 0; i < N_PLAYERS; i++) {
    ai[i]->SetTeams(game.team, trashbid);
    ai[i]->DeclareGame(trashskat, &gd);
    ai[i]->SetGame(&gd);
    player[i].hand = sort(player[i].hand, game.trump);
  }
  // trickgame
  while (!end_trickgame(&game)) {
    if (printing >= 4 && game.trick.empty()) {
      cout << "Declarer P" << game.declarer << " - "
           << game.points[game.declarer] << "/";
      print_bid({game.contract});
      cout << endl << "round " << game.round << endl;
      for (int i = 0; i < N_PLAYERS; i++) {
        cout << "P" << i << ": ";
        print_card(player[i].hand, game.trump);
      }
      cout << endl;
    }
    card c = ai[game.turn]->PlayCard();
    card sc = sort(c, game.trump);
    player[game.turn].hand &= ~sc;

    if (printing >= 2) cout << endl << "P" << game.turn << " plays ";
    print_card(sc, game.trump);
    //    cout << endl;
    if (game.trick.size() >= N_PLAYERS - 1) cout << endl;

    // update players knowledge and game
    for (int p = 0; p < N_PLAYERS; p++) ai[p]->CardPlayed(game.turn, c);
    update_card(&game, sc);
  }
  if (printing >= 2) {
    cout << "score " << score(game, game.team[game.declarer]) << " - "
         << score(game, game.team[game.declarer + 1 % N_PLAYERS]) << endl;
  }
}