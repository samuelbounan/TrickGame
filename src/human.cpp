#include "human.h"

void Human::StartGame(int pos, card handcards) {
  player.id = pos;
  player.hand = handcards;
  for (int i = 0; i < N_PLAYERS; i++) player.have_not[i] = handcards;
  player.have_not[player.id] = ~handcards;
  player.trickstrat = &interactive_card;
}

int Human::PlayBid(int bidval) {
  if (bidval == -1) return 3;
  return 0;
}

void Human::SetTeams(int* team, int* bits) {
  for (int i = 0; i < N_PLAYERS; i++) game.team[i] = team[i];
}

void Human::DeclareGame(card skat, GameDeclaration* gameDeclaration) {}

void Human::SetGame(GameDeclaration* gameDeclaration) {
  game.trump = gameDeclaration->trumpmask;
  game.declarer = game.leader;
  game.turn = 0;
  player.hand = sort(player.hand, game.trump);
}

void Human::CardPlayed(int pos, card c) {
  card sc = sort(c, game.trump);
  update_card(game, sc);
  player.updateBelief(game, sc);
}

card Human::PlayCard() {
  std::cout << "[ Human ] " << endl;
  card c = player.playCard(game);
  return unsort(c, game.trump);
}