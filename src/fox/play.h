// 4-player card playing program using expert knowledge, expert games, belief states, and-or search
// (c) 2022, Stefan Edelkamp

#ifndef __PLAY_H
#define __PLAY_H

#include "cards.h"
#include "aos.h"
#include "alphamu.h"
#include "knowledge.h"

class Play {

 public:
  card myhand; // my current hand, copy in kb
  card orighand; // my initial/original hand  
  int decl_score; // playing score as known of the declarer, copy in kb
  int opp_score; // playing score of the opponent, copy in kb
  card played; // set of cards being played, copy in kb
  int playerpos; // position of player, 0 for declarer, copy in kb
  int playtricks[DECKSIZE]; // all cards currently played, copy in kb
  int playtsize = 0; // number of cards being played, copy in kb
  AlphaMu* alphamu; // AS worst-case analysis
  KnowledgeBase kb; // mainly maintaining knowledge vectors and generating worlds
  int* total; // total ordering
  int* inverse; // total ordering   
  int chk = -1;
  int round = 0; // round of play
  GamePlay g; // copied member variable information on game being played
  card trumpmask = 0; // extracted filter for trump game
  int trumpsuit = -1; // suit [0..3] of suit game
  card asfullhand = 0; // original hand declarer, only for ouvertgames
  int killermove = 0; // extracted filter for trump game
  int holdsuit = -1; // prefer not to drop on card that my partner has dropped already
  card holdmask = 0; // according pattern
  bool interactive = false;  
  
  int solve(GamePlay g, card hand0, card hand1, card hand2,int play, int P0, int P1, int P2, int score_as, int score_gs, int start);

  // abstract functions for selecting card, to be implemented in derived classes
  virtual int propose_declarers_card(card hand, card played) = 0;
  virtual int propose_declarers_card(card hand, int trick1, card played) = 0;
  virtual int propose_declarers_card(card hand, int trick1, int trick2, card played) = 0;
  virtual int propose_declarers_card(card hand, int trick1, int trick2, int trick3, card played) = 0;

  virtual int propose_opponents_card(card hand, card played) = 0;
  virtual int propose_opponents_card(card hand, card played, int trick1) = 0;
  virtual int propose_opponents_card(card hand, card played, int trick1, int trick2) = 0;
  virtual int propose_opponents_card(card hand, card played, int trick1, int trick2, int trick3) = 0;

  // fuctions for selcting cards, nine different situations player player 0/1/2 in VH/MH/HH
  virtual int P0_T0(card hand0, card played, int as_score, int gs_score);
  virtual int P0_T1(card hand0, card played, int as_score, int gs_score, int P2);
  virtual int P0_T2(card hand0, card played,int as_score, int gs_score, int P1, int P2);
  virtual int P0_T3(card hand0, card played,int as_score, int gs_score, int P1, int P2, int P3);

  virtual int P1_T0(card hand1, card played, int as_score, int gs_score);
  virtual int P1_T1(card hand1, card played,int as_score, int gs_score,int P0);
  virtual int P1_T2(card hand1, card played,int as_score, int gs_score,int P3, int P0);
  virtual int P1_T3(card hand1, card played,int as_score, int gs_score,int P2, int P3, int P0);

  virtual int P2_T0(card hand2, card played,int as_score, int gs_score);
  virtual int P2_T1(card hand2, card played,int as_score, int gs_score, int P1);
  virtual int P2_T2(card hand2, card played,int as_score, int gs_score, int P0, int P1);
  virtual int P2_T3(card hand2, card played,int as_score, int gs_score, int P3, int P0, int P1);
 
  virtual int P3_T0(card hand3, card played,int as_score, int gs_score);
  virtual int P3_T1(card hand3, card played,int as_score, int gs_score, int P2);
  virtual int P3_T2(card hand3, card played,int as_score, int gs_score, int P1, int P2);
  virtual int P3_T3(card hand3, card played,int as_score, int gs_score, int P0, int P1, int P2);
 

public:

  Play(GamePlay game,card hand, int position); // constructor 
  virtual ~Play();

  int obscure(card hand, int ls, card played, card trumpmask, int trumpsuit);
  int conform(card hand, int ls, card played, card trumpmask, int trumpsuit);

  // called by server
  virtual int choosePlayerCard(); // T0 play
  virtual int choosePlayerCard(int trickCard1); // T1 play
  virtual int choosePlayerCard(int trickCard1, int trickCard2); // T2 play
  virtual int choosePlayerCard(int trickCard1, int trickCard2, int TickCard3); // T2 play

  int winner(int T0, int T1, int T2, int T4, int *i); // winning position in trick on table i[0..2]
  int win_card(int T1, int T2, int T3, int T4); // determine winner in full trick

  int informPlayerCard(int pos, int trickCard); // inform players that card has been played from player in given pos

  int dropped(int querycard, int wincard); // determine if card is dropped on leading card in trick

  card higher_trump(int index); // all cards higher than index, trump

  int countBelief() { return kb.countBelief(); } // counts information sets

  void print_knowledge() // inform KB about ouvert game 
  { kb.print(); }

  void convert_knowledge() // inform KB about ouvert game 
  { kb.convert(); }
  
  
  // update information set after a card has been selected for play
  void updateBelief(int trickpos, int trickCard1, int trickCard2, int trickCard3, int trickCard4) {
    kb.updateBelief(trickpos, trickCard1, trickCard2, trickCard3);
  }
  void updateBelief(int trickpos, int trickCard1, int trickCard2, int trickCard3) {
    kb.updateBelief(trickpos, trickCard1, trickCard2, trickCard3);
  }
  void updateBelief(int trickpos, int trickCard1, int trickCard2) {
    kb.updateBelief(trickpos, trickCard1, trickCard2);
  }
  void updateBelief(int trickpos, int trickCard1) {
    kb.updateBelief(trickpos, trickCard1);
  }
    
  virtual int suggestCard();  // suggesting card based on knowledge earned through play
};

#endif
