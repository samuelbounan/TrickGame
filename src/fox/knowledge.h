// Bridge playing program using expert knowledge, expert games, belief states, and-or search
// (c) 2022, Stefan Edelkamp a

#ifndef __KB_H
#define __KB_H

#include "cards.h"
#include "game.h"
#include "aos.h"
#include "vector"

using namespace std;

// knowledge about tricks and card placement that player accumulates, enumerates and evals beliefspace

class KnowledgeBase {

 public:
  GamePlay g; // copied member variable information on game being played
  card myhand; // my current hand
  int playerpos; // position of player
  card played = 0; // vector of cards being playerd
  int decl_score = 0, opp_score = 0; // scores of declarer and opponentss  
  card trumpmask = 0; // extracted filter for trump game
  int trumpsuit = -1; // suit [0..3] of suit game
  int playtricks[DECKSIZE]; // all cards currently played
  int playtsize = 0; // number of cards being played
  long long countrek = 0; // global for counting the size of information set
  double best; // best possible skat probabilit
  double thresh; // for selecting good skats
  int sampleh1 = 0; // size of hands for generation of known cards
  int sampleh2 = 0;  // size of hands for generation of known cards
  int sampleh3 = 0;  // size of hands for generation of known cards
  OpenCard* AOS; // and-or open card game solver

  card partial[PLAYER];
  vector<World> allworlds;
  int ratio;
  
  int results[BSSIZE]; // vector of optimized winning card suggestions being voted on
  double suggested_cards[DECKSIZE]; // vector of card suggestions being voted on
  int winning_cards[DECKSIZE]; // vector of optimized winning card suggestions being voted on
  int best_cards[DECKSIZE]; // vector of optimized winning card suggestions being voted on
  int counted_wins[DECKSIZE]; // vector of winning card suggestions being voted on
  double belief[PLAYER][DECKSIZE];

  int holdsuit = -1;
  int order[PLAYER];
  card knowhand[PLAYER];
  int p[PLAYER][MAXTRICKS];
  long long worlds = 0;
  
// bitvector on information sets XsomethingY is knowledge of cards of X in view of player Y
  card allopponent[PLAYER];

  // n * (n-1) knowlege vector
  card nothaving[PLAYER][PLAYER];

  int playtable[PLAYER][MAXTRICKS]; // partitioning tricks among player
  
  KnowledgeBase(GamePlay game, card hand, int position); // constructor
  ~KnowledgeBase() {  delete AOS; } 

  
  // generate hands according to information set and call and-or open card game solver
  void generate_recursive1(card rest, card h1, card h2, card sk, card not1, card not2, card not3, int turn);
  void generate_recursive(card rem, card rest, card h1, card h2, card sk, card not1, card not2, card not3, int turn);
  void generate_hands(card allopponent, card notinhand1, card notinhand2, card notinskat, int turn);

  void generate_winning_recursive1(card rest, card h1, card h2, card h3, card not1, card not2, card not3, int turn, card testcard);
  void generate_winning_recursive(card rem, card rest, card h1, card h2, card h3, card not1, card not2, card nots, int turn, card testcard);
  void generate_winning_hands(card allopponent, card notinhand1, card notinhand2, card notinhand3, int turn, card testcard);

  void generate_worlds_recursive1(card rest, card h1, card h2, card h3, card not1, card not2, card not3, int turn);
  void generate_worlds_recursive(card rem, card rest, card h1, card h2, card h3, card not1, card not2, card nots, int turn);
  
  // count hands in information set
  void count_recursive1(card rest, card h1, card h2, card h3, card not1, card not2, card not3, int turn);
  void count_recursive(card rem, card rest, card h1, card h2, card h3, card not1, card not2, card not3, int turn);    
  long long count_hands(card allopponent, card notinhand1, card notinhand2, card notinskat, int turn);
  
  long long countBelief(); // counts information sets

  // update information set after a card has been selected for play
  void updateBelief(int trickpos, int trickCard1, int trickCard2, int trickCard3, int trickCard4);
  void updateBelief(int trickpos, int trickCard1, int trickCard2, int trickCard3);
  void updateBelief(int trickpos, int trickCard1, int trickCard2);
  void updateBelief(int trickpos, int trickCard1);

  // subfunctions for belief update, changing knowledge vectors
  void DropTrump(int player, int obeyer, int issuer, int ocard, int icard);
  void ObeyTrump(int player, int obeyer, int issuer, int ocard, int icard);

  void DropSuit(int player, int obeyer, int issuer, int ocard, int icard);
  void ObeySuit(int player, int obeyer, int issuer, int ocard, int icard);

  void DropTrump(int player, int obeyer, int issuer, int missing, int ocard, int icard, int mcard);
  void ObeyTrump(int player, int obeyer, int issuer, int missing, int ocard, int icard, int mcard);

  void DropSuit(int player, int obeyer, int issuer, int missing, int ocard, int icard, int mcard);
  void ObeySuit(int player, int obeyer, int issuer, int missing, int ocard, int icard, int mcard);
  
  void DropTrump(int player, int obeyer, int issuer, int missing1, int missing2, int ocard, int icard, int m1card, int m2card);
  void ObeyTrump(int player, int obeyer, int issuer, int missing1, int missing2, int ocard, int icard, int m1card, int m2card);

  void DropSuit(int player, int obeyer, int issuer, int missing1, int missing2, int ocard, int icard, int m1card, int m2card);
  void ObeySuit(int player, int obeyer, int issuer, int missing1, int missing2, int ocard, int icard, int m1card, int m2card);

  int Opponentsnothavingtrump();
  int Partnernothavingtrump();
  int Nextnothavingtrump();
  int Nextnothavingsuit(int suit);
  bool P0hasplayedsuit(int suit);
  bool P1hasplayedsuit(int suit);
  bool P2hasplayedsuit(int suit);
  bool P3hasplayedsuit(int suit);

  // interface status functions for players
  
  void print();  // helper function, prints knowledge matrix, extracted from knowledge vectors  
  void convert();
  void sizes(int issuer, int cardno, int ctn[3]); // helper
};

#endif
