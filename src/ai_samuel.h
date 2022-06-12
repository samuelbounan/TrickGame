#ifndef __AI_SAMUEL
#define __AI_SAMUEL

#include "ai.h"
#include "play.h"
#include "trickstrat/random_card.h"

class AI_samuel : public AI {
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
