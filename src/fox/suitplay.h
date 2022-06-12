// Bridge playing program using expert knowledge, expert games, belief states, and-or search
// (c) 2022, Stefan Edelkamp 

#ifndef __SUITPLAY_H
#define __SUITPLAY_H

#include "cards.h"
#include "play.h"

class Suitplay : public Play { // expert rules, keep simple

 public:

/* constructor assumes global variable game to be set, 
   trump mask and position in play given to super class */

  Suitplay(GamePlay game, card hand, int position);
  virtual ~Suitplay() {}
  
  // proposing card to be issued by the declarer if second in trick
  
  virtual int propose_declarers_card(card hand, card played); // declarer in VH
  virtual int propose_declarers_card(card hand, int trick1, card played); // declarer in 2nd Hand
  virtual int propose_declarers_card(card hand, int trick1, int trick2, card played); // declarer in 3rd Hand
  virtual int propose_declarers_card(card hand, int trick1, int trick2, int trick3, card played); // declarer in 4th Hand

  virtual int propose_opponents_card(card hand, card played); // opponent in VH
  virtual int propose_opponents_card(card hand, card played, int trick1); // opponent in 2nd Hand
  virtual int propose_opponents_card(card hand, card played, int trick1, int trick2); // opponent in 3rd Hand
  virtual int propose_opponents_card(card hand, card played, int trick1, int trick2, int trick3); // opponent in 4th Hand 

};

#endif
