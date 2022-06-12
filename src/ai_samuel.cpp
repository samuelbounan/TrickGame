#include "ai_samuel.h"

void AI_samuel::StartGame(int pos, card handcards) {
  player.id = pos;
  player.hand = handcards;
  for (int i = 0; i < N_PLAYERS; i++) player.have_not[i] = handcards;
  player.have_not[player.id] = ~handcards;
}

int AI_samuel::PlayBid(int bidval) {
  if (bidval == -1) return 3;
  return 0;
}

void AI_samuel::SetTeams(int* team, int* bits) {
  for (int i = 0; i < N_PLAYERS; i++) game.team[i] = team[i];
}

void AI_samuel::DeclareGame(card skat, GameDeclaration* gameDeclaration) {}

void AI_samuel::SetGame(GameDeclaration* gameDeclaration) {
  game.trump = gameDeclaration->trumpmask;
  print_card(game.trump, game.trump);
  game.contract = 3;
  game.declarer = game.leader;
  game.turn = 0;
}

void AI_samuel::CardPlayed(int pos, card c) {
  update_card(&game, c);
  player.updateBelief(game, c);
}

card AI_samuel::PlayCard() {
  std::cout << "[ Samuel ] ";
  return player.playCard(game);
}
