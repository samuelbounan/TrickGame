// 4-player card playing program using expert knowledge, expert games, belief states, and-or search
// (c) 2022, Stefan Edelkamp

#ifndef __ALPHAMU_H
#define __ALPHAMU_H

#include <vector>
#include <list>
#include <iostream>
#include <bitset>
#include <algorithm>

#include "knowledge.h"
#include "cards.h"
#include "game.h"


using namespace std;

//typedef std::vector<bool> Bitset;
typedef unsigned long long Bitset;
typedef std::list<Bitset> Pareto;
//typedef pair<Front,int> Pareto;
typedef std::vector<World> Worlds;

struct State {

  int i[PLAYER];
  card played = 0;
  int decl_points = 0;
  int opp_points = 0;

  State(int P0, int P1, int P2, int P3, card play, int decl, int opp) {
    i[0] = P0; i[1] = P1; i[2] = P2; i[3] = P3; played = play; decl_points = decl, opp_points = opp;
  }
  void print() {
    std::cout << " state, table cards (unordered) ";
    std::cout << i[0] << "_"; if (i[0] != -1) std::cout << card_name[i[0]]; 
    std::cout << i[0] << "_"; if (i[1] != -1) std::cout << card_name[i[1]];
    std::cout << i[0] << "_"; if (i[2] != -1) std::cout << card_name[i[2]];
    std::cout << i[0] << "_"; if (i[3] != -1) std::cout << card_name[i[3]];
    std::cout << " played cards"; print_vector(played,0);
    std::cout << " score declarer team " << decl_points;
    std::cout << " vs score opponent team " << opp_points;
  }
  
  int cardontable(int player) {
    player = (player+1) % PLAYER;
    if (i[player] != -1) return i[player];
    player = (player+1) % PLAYER;
    if (i[player] != -1) return i[player];
    player = (player+1) % PLAYER;
    if (i[player] != -1) return i[player];
    return i[(player+1) % PLAYER];
  }
  int issuer(int player) {
    player = (player+1) % PLAYER;
    if (i[player] != -1) return player;
    player = (player+1) % PLAYER;
    if (i[player] != -1) return player;
    player = (player+1) % PLAYER;
    if (i[player] != -1) return player;
    return (player+1) % PLAYER;
  }
};


  // main class for and-or incomplete informatino play



class AlphaMu {
  card allopponenthand, origallopponenthand;
  card table;
  int limit; // current game contract in trump game
  card trumpmask; // filter for trumpgame
  int trumpsuit; // chosen suit [0..3] for trumpgame
  int sumvalues;
  KnowledgeBase* kb; // mainly maintaining knowledge vectors and generating worlds
  card partial[PLAYER]; // vectors of played cards, each player
  GamePlay g;
  
 public:

  AlphaMu(KnowledgeBase* k);
  ~AlphaMu();

  bool isdominated(Bitset b1, Bitset b2);
  bool dominates(Bitset b1, Bitset b2);
  bool isdominated(Bitset b, Pareto front);
    
  Pareto alphamu(State s, int m, Worlds w, int turn);
  State play(State s, int index, int turn);
  Pareto minmerge(Pareto front, Pareto f);
  Pareto maxmerge(Pareto front, Pareto f);

  void print(Pareto f);
  Pareto insert(Pareto front, Bitset s);
  
  int proposed_card; // one (!) card to be played
  card proposed_set; // all (!) cards to be played
  int proposed; // flag if card has been found
  long long generations; // count number of calls for statistics

  int run(GamePlay g, card hand0, card allopponent, card block1, card block2, card block3, card play, int P0, int P1, int P2, int P3,  int score_decl, int score_opp, int player);
  int solve(GamePlay g, card hand0, card allopponent, card block1, card block2, card block3, card play, int P0, int P1, int P2, int P3,  int score_decl, int score_opp, int player);
  bool stop(State s, int m, Worlds& worlds, int turn);
  Worlds generate(State state, int player, int samplesize);
};

#endif








