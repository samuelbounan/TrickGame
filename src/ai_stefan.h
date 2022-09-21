#ifndef __AI_STEFAN
#define __AI_STEFAN

#include "ai.h"
#include "fox/fox.h"

class AI_stefan : public AI {
 public:
  CardAI ai;

  void StartGame(int pos, card handcards);
  int PlayBid(int bidval);
  void SetTeams(int* team, int* bits);
  void DeclareGame(card skat, GameDeclaration* gameDeclaration);
  void SetGame(GameDeclaration* gameDeclaration);
  void CardPlayed(int pos, card c);
  card PlayCard();
};

#endif
