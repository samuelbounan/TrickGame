#include "ai_samuel.h"

void AI_samuel::StartGame(int pos, card handcards) {
  player.id = pos;
  player.hand = handcards;
  player.trickstrat = &pimc;
  for (int k = 0; k < N_PLAYERS; k++)
    player.have_not[k] = 0;
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
  game.contract = 3;
  game.leader = 0;
  game.declarer = game.leader;
  game.turn = 0;
  game.max_points = MAX_SCORE;
  game.min_points = 0;
  game.remaining = deck;
  game.round = 0;
  player.hand = sort(player.hand, game.trump);
}

void AI_samuel::CardPlayed(int pos, card c) {
  card sc = sort(c, game.trump);
  player.updateBelief(game, sc);
  update_card(game, sc);
}

card AI_samuel::PlayCard() {
  if (PRINTING >= 2) std::cout << "[ Samuel ] ";
  card c = player.playCard(game);
  return unsort(c, game.trump);
}