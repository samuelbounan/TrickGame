// 4-player card playing program using expert knowledge, expert games, belief
// states, and-or search (c) 2022, Stefan Edelkamp

#include "alphamu.h"

#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <list>
#include <vector>

#include "aos.h"
#include "cards.h"
#include "knowledge.h"
#include "util.h"

using namespace std;

#define WORLDS 10

AlphaMu::~AlphaMu() {}

AlphaMu::AlphaMu(KnowledgeBase* k) : kb(k) {
  for (int i = 0; i < PLAYER; i++) partial[i] = 0;
}

bool AlphaMu::isdominated(Bitset b1, Bitset b2) {
  return  //(b1 != b2) &&
      (b1 & b2) == b1;
}

bool AlphaMu::dominates(Bitset b1, Bitset b2) {
  return  //(b1 != b2) &&
      (b1 & b2) == b2;
}

Pareto AlphaMu::minmerge(Pareto front, Pareto f) {
  //  cout << " minmerge of "; print(front); cout << " and "; print(f);
  if (front.size() == 0) return f;
  Pareto result;
  for (auto vector : front) {
    for (auto v : front) {
      Bitset b = vector & v;
      if (!isdominated(b, result)) {
        result = insert(result, b);
      }
    }
  }
  //  cout << " is "; print(result); std::cout << std::endl;
  return result;
}

Pareto AlphaMu::insert(Pareto front, Bitset b) {
  bool change = true;
  while (change) {
    change = false;
    for (auto v : front) {
      if (isdominated(v, b)) {
        change = true;
        front.remove(v);
        break;
      }
    }
  }
  front.push_back(b);
  return front;
}
bool AlphaMu::isdominated(Bitset b, Pareto front) {
  for (auto r : front) {
    if (dominates(r, b)) return true;
  }
  return false;
}

void AlphaMu::print(Pareto f) {
  std::cout << "{";
  for (auto v : f) {
    std::cout << "(";
    for (int i = 0; i < WORLDS; i++) {
      if (v & (ONE << i))
        std::cout << "1";
      else
        std::cout << "0";
    }
    std::cout << "),";
  }
  std::cout << "}";
}

Pareto AlphaMu::maxmerge(Pareto front, Pareto f) {
  //  cout << " maxmerge of "; print(front); cout << " and "; print(f);
  for (auto v : f) {
    if (!isdominated(v, front)) {
      front = insert(front, v);
    }
  }
  //  cout << " is ";  print(front);  std::cout << std::endl;
  return front;
}

State AlphaMu::play(State s, int index, int player) {
  State t = s;
  //    cout << "play card "; s.print(); cout << card_name[index] << " P" <<
  //    player << endl;
  card bitindex = (ONE << index);
  t.played |= bitindex;
  t.i[player] = index;
  if (count(t.played) % PLAYER == 0) {
    int turn = -1, score = -1;
    if (turn == 0) {
      turn = kb->AOS->winner(1, 2, 3, 0);
      score = COST(t.i[0], t.i[1], t.i[2], t.i[3]);
    } else if (turn == 1) {
      turn = kb->AOS->winner(2, 3, 0, 1);
      score = COST(t.i[0], t.i[1], t.i[2], t.i[3]);
    } else if (turn == 2) {
      turn = kb->AOS->winner(3, 0, 1, 2);
      score = COST(t.i[0], t.i[1], t.i[2], t.i[3]);
    } else if (turn == 3) {
      turn = kb->AOS->winner(0, 1, 2, 3);
      score = COST(t.i[0], t.i[1], t.i[2], t.i[3]);
    }
    if (g.team[turn] == 0)
      t.decl_points += score;
    else
      t.opp_points += score;
    t.i[0] = t.i[1] = t.i[2] = t.i[3] = -1;
  }
  //  t.print();
  return t;
}

Pareto AlphaMu::alphamu(State state, int m, Worlds worlds, int player) {
  Pareto r;

  //  cout << " calling alphamu recursive function with m "<< m << " player " <<
  //  player << " #worlds " << worlds.size() << " state "; state.print(); cout
  //  << endl;

  if (stop(state, m, worlds, player)) {
    Pareto p;
    Bitset bs = 0;
    int cnt = 0;
    for (auto w : worlds) {
      if (w.won == 1) bs |= (ONE << cnt);
      cnt++;
    }
    // cout << "eval worlds" << endl; for (auto w : worlds) w.print();
    p.push_back(bs);
    //    cout << "leaf reached"; print(p);    cout << endl;
    return p;
  }
  Pareto front;

  card playc = 0;
  //  cout << "default" << endl;  state.print();
  int issuer = state.issuer(player);
  //  cout << " table " << "," << state.i[0] << "," << state.i[1] << ","<<
  //  state.i[2] << "," << state.i[3] << endl;
  //   cout << " collecting playable P" << issuer << " issues " <<
  //   card_name[state.i[issuer]] << endl; for (World w : worlds) w.print();
  for (World w : worlds) {
    card legal = 0;
    if (w.won != -2) {
      if (count(state.played) % PLAYER == 0) {
        //	cout << "world/0"; w.print(); cout << endl;
        legal = g.playable_cards(w.hands[player], state.played);
      } else {
        //	cout << "world/1"; w.print(); cout << endl;	cout << "current
        // hand ";g.print_vector(w.hands[player],trumpmask); cout << endl;
        legal =
            g.playable_cards(w.hands[player], issuer, state.played, state.i);
      }
    }
    playc |= legal;
  }

  //  cout << "joint legal cards "; g.print_vector(playc,trumpmask); cout << "
  //  player P" << player << " team " << g.team[player];  cout << endl;  cout <<
  //  "unprogressed / unevalated worlds" << endl;  for (auto w : worlds)
  //  w.print();

  if (g.team[player] == 1) {
    while (playc) {
      int index = CTZ(playc);
      //      state.print();      cout << "checking declarer " <<
      //      card_name[index] << endl;
      State newstate = play(state, index, player);
      Worlds W1;
      for (auto w : worlds) {
        if (w.won == -2)
          W1.push_back(w);
        else if (w.hands[player] & (ONE << index)) {
          w.hands[player] &= ~(ONE << index);
          W1.push_back(w);
        } else {
          w.won = -2;
          W1.push_back(w);
        }
      }

      // cout << "progressed worlds" << endl;cout << "after play";
      // newstate.print();      for (auto w1 : W1) w1.print();
      Pareto f = alphamu(newstate, m, W1, (player + 1) % PLAYER);
      //      cout << "before minmerge" << endl;
      front = minmerge(front, f);
      //      cout << "after minmerge" << endl;
      playc &= ~(ONE << index);
    }
  } else {
    while (playc) {
      int index = CTZ(playc);
      //    cout << "checking opponent " << card_name[index] << endl;
      State newstate = play(state, index, player);
      Worlds W1;
      for (auto w : worlds) {
        if (w.won == -2)
          W1.push_back(w);
        else if (w.hands[player] & (ONE << index)) {
          w.hands[player] &= ~(ONE << index);
          W1.push_back(w);
        } else {
          w.won = -2;
          W1.push_back(w);
        }
      }
      //      cout << "progressed worlds" << endl;
      //      for (auto w1 : W1) w1.print();
      Pareto f = alphamu(newstate, m - 1, W1, (player + 1) % PLAYER);
      //      cout << "before maxmerge" << endl;
      front = maxmerge(front, f);
      //      cout << "after maxmerge" << endl;
      playc &= ~(ONE << index);
    }
    //    cout << "end of playc" << endl;
  }
  //  cout << "end of proc, returning" << endl; print(front);
  return front;
}

int AlphaMu::run(GamePlay game, card hand, card allopponent, card block1,
                 card block2, card block3, card play, int P0, int P1, int P2,
                 int P3, int score_decl, int score_opp, int player) {
  generations = 0;

  g = game;
  limit = g.limit;
  trumpmask = g.trumpmask;
  trumpsuit = g.trumpsuit;
  sumvalues = g.sumvalues;
  ;

  int result = (g.team[player] == 0) ? 0 : sumvalues;
  if (printing >= 4) {
    std::cout << " alphamu @" << g.limit << " player " << player << " ";
    if (P1 != -1) std::cout << card_name[P1] << " ";
    if (P2 != -1) std::cout << card_name[P2] << " ";
    if (P3 != -1) std::cout << card_name[P3] << " ";
    if (P0 != -1) std::cout << card_name[P0] << " ";
    std::cout << " P0 ";
    g.print_vector(hand, trumpmask);
  }
  /*
 std::cout << " AO "; g.print_vector(allopponent,trumpmask);
  std::cout << " B1 "; g.print_vector(block1,trumpmask);
  std::cout << " B2 "; g.print_vector(block2,trumpmask);
  std::cout << " B3 "; g.print_vector(block3,trumpmask);
  */
  if (printing >= 4) {
    std::cout << " decl score " << score_decl;
    std::cout << " opp score " << score_opp << " ";
  }
  //  std::cout << std::endl;

  State s(P0, P1, P2, P3, play, score_decl, score_opp);
  proposed = 0;
  proposed_set = 0;

  Worlds worlds = generate(s, player, WORLDS);
  Pareto front = alphamu(s, 2, worlds, player);
  /*
  for (auto w : front) {
    card legal = 0;
    if (w.won != -2) {
      if (count(state.played) % PLAYER == 0) {
        legal = g.playable_cards(w.hands[player],state.played);
      }
      else {
        legal = g.playable_cards(w.hands[player],issuer,state.played,state.i);
      }
    }
    playc |= legal;
  }
  */
  //  std::cout << " alphamu finished " << worlds.size();
  Bitset b = 0;
  int max = 0;
  for (auto f : front) {
    //    b |= f;
    if (count(f) > max) {
      b = f;
      max = count(f);
    }
  }

  if (printing >= 4) {
    cout << " best vector ";
    std::cout << "(";
    for (int i = 0; i < WORLDS; i++) {
      if (b & (ONE << i))
        std::cout << "1";
      else
        std::cout << "0";
    }
    std::cout << "),";
    cout << endl;
  }
  card playc = 0;
  int issuer = s.issuer(player);
  for (int i = 0; i < worlds.size(); i++) {
    World w = worlds[i];
    if ((ONE << i) & b) {
      card legal = 0;
      if (count(s.played) % PLAYER == 0)
        legal = g.playable_cards(w.hands[player], s.played);
      else
        legal = g.playable_cards(w.hands[player], issuer, s.played, s.i);
      playc |= legal;
    }
  }
  //  cout << " found "; g.print_vector(playc,trumpmask); cout << endl;

  if (false) {  // (b) {
    proposed = 1;
    //    proposed_set = playc;
    proposed_card = g.largest(playc);
    return true;
  } else {
    proposed = 1;
    int issuer = s.issuer(player);
    card legal = 0;
    if (count(s.played) % PLAYER == 0) {
      legal = g.playable_cards(hand, s.played);
    } else {
      legal = g.playable_cards(hand, issuer, s.played, s.i);
    }
    proposed_card = g.smallest(legal);
    return false;
  }
  //  cout << std::endl;
  //  return result;
}

int AlphaMu::solve(GamePlay g, card hand, card allopponent, card block1,
                   card block2, card block3, card play, int P0, int P1, int P2,
                   int P3, int score_decl, int score_opp, int player) {
  int left = score_decl - 1, right = g.sumvalues;  // binsearch

  //  return run(g, hand, allopponent, block1, block2, block3, play, P0, P1, P2,
  //  P3, score_decl,score_opp, player);
  proposed = 0;
  proposed_card = -1;
  while (true) {
    //    std::cout << "Search Interval [" << left << "," << right << "]" <<
    //    std::endl;
    if (left >=
        right -
            1) {  // P0 can win with 1+left, but not with more than 1+right pts
      if (g.team[player] == 1)
        g.limit = right;
      else
        g.limit = right - 1;
      int s = run(g, hand, allopponent, block1, block2, block3, play, P0, P1,
                  P2, P3, score_decl, score_opp, player);
      return right;
    }
    g.limit = (left + right) / 2;
    int x = run(g, hand, allopponent, block1, block2, block3, play, P0, P1, P2,
                P3, score_decl, score_opp, player);
    if (!x) {  // std::cout << "...too big" << std::endl;
      right = g.limit;
    } else {  // std::cout << "...too small, but win, pc = proposed_card" <<
              // std::endl;
      left = g.limit;
    }
  }
}

Worlds AlphaMu::generate(State state, int player, int samplesize) {
  card h1 = 0, h2 = 0, h3 = 0;
  kb->allworlds.clear();
  long long chands = 0;
  for (int i = 0; i < PLAYER; i++) partial[i] = 0;
  for (int j = 0; j < PLAYER; j++) {
    for (int i = 0; i < MAXTRICKS; i++) {
      if (kb->playtable[j][i] != -1) partial[j] |= (ONE << kb->playtable[j][i]);
    }
  }

  //  cout << "[" << count(partial[0]) << "," << count(partial[1]) << "," <<
  //  count(partial[2]) << "," << count(partial[3]) << "]";

  if (player == 0) {
    kb->sampleh1 = MAXTRICKS - count(partial[1]);
    kb->sampleh2 = MAXTRICKS - count(partial[2]);
    kb->sampleh3 = MAXTRICKS - count(partial[3]);
    chands = kb->count_hands(kb->allopponent[0], kb->nothaving[1][0],
                             kb->nothaving[2][0], kb->nothaving[3][0], 0);
  }
  if (player == 1) {
    kb->sampleh1 = MAXTRICKS - count(partial[0]);
    kb->sampleh2 = MAXTRICKS - count(partial[2]);
    kb->sampleh3 = MAXTRICKS - count(partial[3]);
    chands = kb->count_hands(kb->allopponent[1], kb->nothaving[0][1],
                             kb->nothaving[2][1], kb->nothaving[3][1], 1);
  }
  if (player == 2) {
    kb->sampleh1 = MAXTRICKS - count(partial[0]);
    kb->sampleh2 = MAXTRICKS - count(partial[1]);
    kb->sampleh3 = MAXTRICKS - count(partial[3]);
    chands = kb->count_hands(kb->allopponent[2], kb->nothaving[0][2],
                             kb->nothaving[1][2], kb->nothaving[3][2], 2);
  }
  if (player == 3) {
    kb->sampleh1 = MAXTRICKS - count(partial[0]);
    kb->sampleh2 = MAXTRICKS - count(partial[1]);
    kb->sampleh3 = MAXTRICKS - count(partial[2]);
    chands = kb->count_hands(kb->allopponent[3], kb->nothaving[0][3],
                             kb->nothaving[1][3], kb->nothaving[2][3], 3);
  }

  int ratio = 1 + (chands / samplesize);
  kb->ratio = ratio;
  /*
  cout << " generate selection of " << chands <<  " worlds, samplesize " <<
  samplesize; cout << " taking every " << ratio << " element " ; cout << "(" <<
  player << ")"; cout << "[" << kb->sampleh1 << "," << kb->sampleh2 << "," <<
  kb->sampleh3 << "]"; cout << endl;
  */
  kb->allworlds.clear();

  if (player == 0) {
    if (chands > 0) {
      kb->generate_worlds_recursive(0, kb->allopponent[0], 0, 0, 0,
                                    kb->nothaving[1][0], kb->nothaving[2][0],
                                    kb->nothaving[3][0], 0);
    }
  } else if (player == 1) {
    if (chands > 0) {
      kb->generate_worlds_recursive(0, kb->allopponent[1], 0, 0, 0,
                                    kb->nothaving[0][1], kb->nothaving[2][1],
                                    kb->nothaving[3][1], 1);
    }
  } else if (player == 2) {
    if (chands > 0) {
      kb->generate_worlds_recursive(0, kb->allopponent[2], 0, 0, 0,
                                    kb->nothaving[0][2], kb->nothaving[1][2],
                                    kb->nothaving[3][2], 2);
    }
  } else if (player == 3) {
    if (chands > 0) {
      kb->generate_worlds_recursive(0, kb->allopponent[3], 0, 0, 0,
                                    kb->nothaving[0][3], kb->nothaving[1][3],
                                    kb->nothaving[2][3], 3);
    }
  }
  //  for (auto w : kb->allworlds)
  //    w.print();
  //    for (int i=0;i<kb->allworlds;i++)
  //      cout << kb->results[kb->worlds];
  //    cout << endl;
  return kb->allworlds;
}

bool AlphaMu::stop(State s, int m, Worlds& worlds, int player) {
  //    cout << "stop " << s.decl_points << ">" << limit << " and " <<
  //    s.opp_points << ">=" << sumvalues-limit << " player " << player << " m "
  //    << m << " belief space size " << worlds.size() << endl;
  if (s.decl_points > limit) return 1;
  if (s.opp_points >= sumvalues - limit) return 1;

  if (m == 0) {
    //    std::cout << "m=0 worlds " << worlds.size() << std::endl;
    for (auto& w : worlds) {
      if (w.won != -2) {
        int won = kb->AOS->run(g, w.hands[0], w.hands[1], w.hands[2],
                               w.hands[3], s.played, s.i[0], s.i[1], s.i[2],
                               s.i[3], s.decl_points, s.opp_points, player);
        w.won = won;
      }
      //      w.print();
    }
    return 1;
  }
  return 0;
}
