#ifndef __AI_H
#define __AI_H

using namespace std;

struct GameDeclaration {
  int game;
  int contract;
  card trumpmask;
  int limit;
  int total;
};

class AI {

 public:
  virtual void StartGame(int pos, card hand) = 0;
  virtual int PlayBid(int bidval) = 0;
  virtual void SetTeams(int* teams, int* bids) = 0;
  virtual void DeclareGame(card skat, GameDeclaration *gameDeclaration) = 0;
  virtual void SetGame(GameDeclaration *gameDeclaration) = 0;
  virtual void CardPlayed(int pos, card c) = 0;
  virtual card PlayCard(void) = 0;
};


#endif
