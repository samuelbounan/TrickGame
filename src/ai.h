#ifndef __AI
#define __AI

#include "card.h"

struct GameDeclaration {
  int game;
  int contract;
  card trumpmask;
  int limit;
  int total;
};

class AI {
 public:
  virtual void StartGame(int pos, card handcards) = 0;
  /*******************************************************************************
   * called when a new game is started
   * Parameters:
   * ePosition pos: position
   * int *handcards: pointer to array with card codes
   *******************************************************************************/
  virtual int PlayBid(int bidval) = 0;
  /*******************************************************************************
   * called when ki has to confirm a bid
   * Parameters:
   * int bidval: value to increas
   *******************************************************************************/
  virtual void SetTeams(int* team, int* bits) = 0;
  /*******************************************************************************
   * called when ki has lost the bidding
   * Parameters:
   * team: wrt initial posn, 0/1 array of players
   * int* bid: highest bid of players
   *******************************************************************************/
  virtual void DeclareGame(card skat, GameDeclaration* gameDeclaration) = 0;
  /*******************************************************************************
   * called when ki has requested has to declare a game
   * Parameters:
   * sGameDeclaration *gameDeclaration: pointer to Game Declaration struct
   *  return value, you have to set these values:
   *  eGameType game
   *******************************************************************************/
  virtual void SetGame(GameDeclaration* gameDeclaration) = 0;
  /*******************************************************************************
   * called after game is declared
   * Parameters:
   * sGameDeclaration *gameDeclaration: pointer to Game Declaration struct
   *  values set: eGameType game,
   *******************************************************************************/
  virtual void CardPlayed(int pos, card c) = 0;
  /*******************************************************************************
   * called always when a card is played by a player
   * Parameters:
   * ePosition pos: position of player who played the card
   * card: card code of played card
   *******************************************************************************/
  virtual card PlayCard(void) = 0;
  /*******************************************************************************
   * called when ki has to play a card
   * Parameters: none
   * returns: card code of card to be played
   *******************************************************************************/
};

#endif
