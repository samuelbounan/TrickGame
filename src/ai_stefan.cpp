#include "ai_stefan.h"

card sort_stefan(card hand) {
  int cards_per_suit = N_CARDS / N_SUITS;
  card res = 0;
  while (hand) {
    int idx = CTZ(hand);
    hand &= ~(ONE << idx);
    int r = cards_per_suit * (idx / cards_per_suit) + (cards_per_suit - 1 - idx % cards_per_suit);
    res |= (ONE << r);
  }
  return res;
}

card unsort_stefan(card x) { return sort_stefan(x); }

void AI_stefan::StartGame(int pos, card handcards) {
  ai.StartGame(pos, sort_stefan(handcards));
}

int AI_stefan::PlayBid(int bidval) {
  return ai.PlayBid(bidval);
}

void AI_stefan::SetTeams(int* team, int* bits) {
  ai.SetTeams(team, bits);
}

void AI_stefan::DeclareGame(card skat, GameDeclaration* gameDeclaration) {
  ai.DeclareGame(skat, gameDeclaration);
}

void AI_stefan::SetGame(GameDeclaration* gameDeclaration) {
  ai.SetGame(gameDeclaration);
}

void AI_stefan::CardPlayed(int pos, card c) {
  card sc = sort_stefan(c);
  ai.CardPlayed(pos, sc);
}

card AI_stefan::PlayCard() {
  card c = ai.PlayCard();
  return unsort_stefan(c);
}