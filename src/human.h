#ifndef __HUMAN
#define __HUMAN

#include "ai.h"
#include "trickstrat/interactive_card.h"

class Human : public AI {
 public:
  Player player;
  Game game;

  void StartGame(int pos, card handcards);
  int PlayBid(int bidval);
  void SetTeams(int* team, int* bits);
  void DeclareGame(card skat, GameDeclaration* gameDeclaration);
  void SetGame(GameDeclaration* gameDeclaration);
  void CardPlayed(int pos, card c);
  card PlayCard();
};

#endif
