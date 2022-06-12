// 4-player card playing program using expert knowledge, expert games, belief
// states, and-or search (c) 2022, Stefan Edelkamp

#include "play.h"

#include <fstream>
#include <iomanip>
#include <iostream>

#include "alphamu.h"
#include "cards.h"
#include "game.h"
#include "knowledge.h"

using namespace std;

int Play::obscure(card hand, int ls, card played, card trumpmask,
                  int trumpsuit) {
  return ls;
}

int Play::P0_T0(card hand0, card played, int decl_score, int opp_score) {
  if (!killermove) return chk;
  chk = propose_declarers_card(hand0, played);
  g.preferred_card = chk;
  if (killermove) {
    card reduced = g.playable_cards(hand0, played);
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P0 T0 " << count(played) << " ";
    //    alphamu->print(g,hand0,kb.allopponent[0],kb.nothaving[1][0],kb.nothaving[2][0],kb.nothaving[3][0],played,-1,-1,-1-1,,decl_score,opp_score,0);
#endif
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p0 t0 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand0, kb.allopponent[0], kb.nothaving[1][0],
                             kb.nothaving[2][0], kb.nothaving[3][0], played, -1,
                             -1, -1, -1, decl_score, opp_score, 0);
      if ((g.team[0] == 0 && w > g.limit ||
           g.team[0] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << ", p0 t0 can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << ": " << w << endl;
#endif
        return obscure(hand0, alphamu->proposed_card, played, trumpmask,
                       trumpsuit);
      }
    }
    return -1;
  }
  return chk;
}

int Play::P0_T1(card hand0, card played, int decl_score, int opp_score,
                int P3) {
  if (!killermove) return chk;
  //  cout << "before P0_T1 pdc " << card_name[P3] << endl;
  chk = propose_declarers_card(hand0, P3, played);
  //  cout << chk << endl;
  //  cout << card_name[chk] << endl;
  g.preferred_card = chk;  // global!!
  if (killermove) {
    int i[PLAYER] = {-1, -1, -1, P3};
    card reduced = g.playable_cards(hand0, 3, played, i);
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P0 T1 " << count(played) << " ";
    //    alphamu->print(g,hand0,kb.allopponent[0],kb.nothaving[1][0],kb.nothaving[2][0],kb.nothaving[3][0],played,-1,-1,-1,P3,decl_score,opp_score,0);
#endif
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p0 t1 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand0, kb.allopponent[0], kb.nothaving[1][0],
                             kb.nothaving[2][0], kb.nothaving[3][0], played, -1,
                             -1, -1, P3, decl_score, opp_score, 0);
      if ((g.team[0] == 0 && w > g.limit ||
           g.team[0] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << ", p0 t1 can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << ": " << w << endl;
#endif
        return alphamu->proposed_card;
      }
    }
    return -1;
  }
  return chk;
}

int Play::P0_T2(card hand0, card played, int decl_score, int opp_score, int P2,
                int P3) {
  if (!killermove) return chk;
  chk = propose_declarers_card(hand0, P2, P3, played);
  g.preferred_card = chk;
  if (killermove) {
    int i[PLAYER] = {-1, -1, P2, P3};
    card reduced =
        g.playable_cards(hand0, 2, played, i);  // only consider valid cards
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P0 T2;" << count(played) << " ";
    //    alphamu->print(g,hand0,kb.allopponent[0],kb.nothaving[1][0],kb.nothaving[2][0],kb.nothaving[3][0],played,-1,-1,P2,P3,decl_score,opp_score,0);
#endif
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p0 t2 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand0, kb.allopponent[0], kb.nothaving[1][0],
                             kb.nothaving[2][0], kb.nothaving[3][0], played, -1,
                             -1, P2, P3, decl_score, opp_score, 0);
      if ((g.team[0] == 0 && w > g.limit ||
           g.team[0] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << ", p0 t2 can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << ": " << w << endl;
#endif
        if (w != decl_score) return alphamu->proposed_card;
      }
    }
    return -1;
  }
  return chk;
}

int Play::P0_T3(card hand0, card played, int decl_score, int opp_score, int P1,
                int P2, int P3) {
  if (!killermove) return chk;
  chk = propose_declarers_card(hand0, P1, P2, P3, played);
  g.preferred_card = chk;
  if (killermove) {
    int i[PLAYER] = {-1, P1, P2, P3};
    card reduced =
        g.playable_cards(hand0, 1, played, i);  // only consider valid cards
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P0 T3;" << count(played) << " ";
    //    alphamu->print(g,hand0,kb.allopponent[0],kb.nothaving[1][0],kb.nothaving[2][0],kb.nothaving[3][0],played,-1,P1,P2,P3,decl_score,opp_score,0);
#endif
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p0 t3 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand0, kb.allopponent[0], kb.nothaving[1][0],
                             kb.nothaving[2][0], kb.nothaving[3][0], played, -1,
                             P1, P2, P3, decl_score, opp_score, 0);
      if ((g.team[0] == 0 && w > g.limit ||
           g.team[0] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << ", p0 t3 can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << ": " << w << endl;
#endif
        if (w != decl_score) return alphamu->proposed_card;
      }
    }
    return -1;
  }
  return chk;
}

int Play::P1_T0(card hand1, card played, int decl_score, int opp_score) {
  if (!killermove) return chk;
  chk = propose_opponents_card(hand1, played);
  g.preferred_card = chk;  // global!!
  if (killermove) {
    card reduced = g.playable_cards(hand1, played);
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P1 T0 " << count(played) << " ";
    //    alphamu->print(g,hand1,kb.allopponent[1],kb.nothaving[0][1],kb.nothaving[2][1],kb.nothaving[3][1],played,-1,-1,-1,-1,decl_score,opp_score,1);
#endif
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p1 t0 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand1, kb.allopponent[1], kb.nothaving[0][1],
                             kb.nothaving[2][1], kb.nothaving[3][1], played, -1,
                             -1, -1, -1, decl_score, opp_score, 1);
      if ((g.team[1] == 0 && w > g.limit ||
           g.team[1] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << " p1 t0 can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << ": " << w << endl;
#endif
        return obscure(hand1, alphamu->proposed_card, played, trumpmask,
                       trumpsuit);
      }
    }
    return -1;
  }
  return chk;
}

int Play::P1_T1(card hand1, card played, int decl_score, int opp_score,
                int P0) {
  if (!killermove) return chk;
  chk = propose_opponents_card(hand1, played, P0);
  g.preferred_card = chk;  // global!!
  if (killermove) {
    int i[PLAYER] = {P0, -1, -1, -1};
    card reduced =
        g.playable_cards(hand1, 0, played, i);  // only consider valid cards
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P1 T1 " << count(played) << " ";
    //    alphamu->print(g,hand1,kb.allopponent[1],kb.nothaving[0][1],kb.nothaving[2][1],kb.nothaving[3][1],played,P0,-1,-1,-1,decl_score,opp_score,1);
#endif
    card tablecards = (ONE << P0);
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p1 t1 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand1, kb.allopponent[1], kb.nothaving[0][1],
                             kb.nothaving[2][1], kb.nothaving[3][1], played, P0,
                             -1, -1, -1, decl_score, opp_score, 1);
      if ((g.team[1] == 0 && w > g.limit ||
           g.team[1] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << " p1 t1 can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << ": " << w << endl;
#endif
        return alphamu->proposed_card;
      }
    }
    return -1;
  }
  return chk;
}

int Play::P1_T2(card hand1, card played, int decl_score, int opp_score, int P3,
                int P0) {
  if (!killermove) return chk;
  chk = propose_opponents_card(hand1, played, P3, P0);
  g.preferred_card = chk;
  if (killermove) {
    int i[PLAYER] = {P0, -1, -1, P3};
    card reduced = g.playable_cards(hand1, 3, played, i);
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P1 T2;" << count(played) << " ";
    cout << card_name[chk] << endl;
    //    alphamu->print(g,hand1,kb.allopponent[1],kb.nothaving[0][1],kb.nothaving[2][1],kb.nothaving[3][1],played,P0,-1,-1,P3,decl_score,opp_score,1);
#endif
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p1 t2 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand1, kb.allopponent[1], kb.nothaving[0][1],
                             kb.nothaving[2][1], kb.nothaving[3][1], played, P0,
                             -1, -1, P3, decl_score, opp_score, 1);
      if ((g.team[1] == 0 && w > g.limit ||
           g.team[1] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << " p1 hh can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << ": " << w << endl;
#endif
        return alphamu->proposed_card;
      }
    }
    return -1;
  }
  return chk;
}

int Play::P1_T3(card hand1, card played, int decl_score, int opp_score, int P2,
                int P3, int P0) {
  if (!killermove) return chk;
  chk = propose_opponents_card(hand1, played, P2, P3, P0);
  g.preferred_card = chk;  // global!!
  if (killermove) {
    int i[PLAYER] = {P0, -1, P2, P3};
    card reduced =
        g.playable_cards(hand1, 2, played, i);  // only consider valid cards
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P1 T3;" << count(played) << " ";
    //    alphamu->print(g,hand1,kb.allopponent[1],kb.nothaving[0][1],kb.nothaving[2][1],kb.nothaving[3][1],played,P0,-1,P2,P3,decl_score,opp_score,1);
#endif
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p1 t3 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand1, kb.allopponent[1], kb.nothaving[0][1],
                             kb.nothaving[2][1], kb.nothaving[3][1], played, P0,
                             -1, P2, P3, decl_score, opp_score, 1);
      if ((g.team[1] == 0 && w > g.limit ||
           g.team[1] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << " p1 t3 can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << ": " << w << endl;
#endif
        return alphamu->proposed_card;
      }
    }
    return -1;
  }
  return chk;
}

int Play::P2_T0(card hand2, card played, int decl_score, int opp_score) {
  if (!killermove) return chk;
  chk = propose_opponents_card(hand2, played);
  g.preferred_card = chk;  // global!!
  if (killermove) {
    card reduced = g.playable_cards(hand2, played);
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P2 T0 " << count(played) << " ";
    //    alphamu->print(g,hand2,kb.allopponent[2],kb.nothaving[0][2],kb.nothaving[1][2],kb.nothaving[3][2],played,-1,-1,-1,-1,decl_score,opp_score,2);
#endif
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p2 t0 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand2, kb.allopponent[2], kb.nothaving[0][2],
                             kb.nothaving[1][2], kb.nothaving[3][2], played, -1,
                             -1, -1, -1, decl_score, opp_score, 2);
      if ((g.team[2] == 0 && w > g.limit ||
           g.team[2] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << " p2 t0 can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << ": " << w << endl;
#endif
        return obscure(hand2, alphamu->proposed_card, played, trumpmask,
                       trumpsuit);
      }
    }
    return -1;
  }
  return chk;
}

int Play::P2_T1(card hand2, card played, int decl_score, int opp_score,
                int P1) {
  if (!killermove) return chk;
  chk = propose_opponents_card(hand2, played, P1);
  if (killermove) {
    int i[PLAYER] = {-1, P1, -1, -1};
    card reduced =
        g.playable_cards(hand2, 1, played, i);  // only consider valid cards
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P2 T1 " << count(played) << " ";
    // alphamu->print(g,hand2,kb.allopponent[2],kb.nothaving[0][2],kb.nothaving[1][2],kb.nothaving[3][2],played,-1,P1,-1,-1,decl_score,opp_score,2);
#endif
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p2 t1 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand2, kb.allopponent[2], kb.nothaving[0][2],
                             kb.nothaving[1][2], kb.nothaving[3][2], played, -1,
                             P1, -1, -1, decl_score, opp_score, 2);
      if ((g.team[2] == 0 && w > g.limit ||
           g.team[2] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << " p2 t1 can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << endl;
#endif
        return alphamu->proposed_card;
      }
    }
    return -1;
  }
  return chk;
}

int Play::P2_T2(card hand2, card played, int decl_score, int opp_score, int P0,
                int P1) {
  if (!killermove) return chk;
  chk = propose_opponents_card(hand2, played, P0, P1);
  g.preferred_card = chk;
  if (killermove) {
    int i[PLAYER] = {P0, P1, -1, -1};
    card reduced = g.playable_cards(hand2, 0, played, i);
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P2 T2 " << count(played) << " ";
    // alphamu->print(g,hand2,kb.allopponent[2],kb.nothaving[0][2],kb.nothaving[1][2],kb.nothaving[3][2],played,P0,P1,-1,-1,decl_score,opp_score,2);
#endif
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p2 t2 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand2, kb.allopponent[2], kb.nothaving[0][2],
                             kb.nothaving[1][2], kb.nothaving[3][2], played, P0,
                             P1, -1, -1, decl_score, opp_score, 2);
      if ((g.team[2] == 0 && w > g.limit ||
           g.team[2] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << " p2 t2 can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << endl;
#endif
        return alphamu->proposed_card;
      }
    }
    return -1;
  }
  return chk;
}

int Play::P2_T3(card hand2, card played, int decl_score, int opp_score, int P3,
                int P0, int P1) {
  if (!killermove) return chk;
  chk = propose_opponents_card(hand2, played, P3, P0, P1);
  g.preferred_card = chk;
  if (killermove) {
    int i[PLAYER] = {P0, P1, -1, P3};
    card reduced = g.playable_cards(hand2, 3, played, i);
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P2 T3 " << count(played) << " ";
    // alphamu->print(g,hand2,kb.allopponent[2],kb.nothaving[0][2],kb.nothaving[1][2],kb.nothaving[3][2],played,-1,P0,P1,P3,decl_score,opp_score,2);
#endif
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p2 t3 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand2, kb.allopponent[2], kb.nothaving[0][2],
                             kb.nothaving[1][2], kb.nothaving[3][2], played, -1,
                             P0, P1, P3, decl_score, opp_score, 2);
      if ((g.team[2] == 0 && w > g.limit ||
           g.team[2] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << " p2 t3 can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << endl;
#endif
        return alphamu->proposed_card;
      }
    }
    return -1;
  }
  return chk;
}

int Play::P3_T0(card hand3, card played, int decl_score, int opp_score) {
  if (!killermove) return chk;
  //  g.print_vector(hand3,trumpmask); cout << "HERE WE GO" << endl;
  chk = propose_opponents_card(hand3, played);
  g.preferred_card = chk;  // global!!
  if (killermove) {
    card reduced = g.playable_cards(hand3, played);
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P3 T0 " << count(played) << " ";
    // alphamu->print(g,hand3,kb.allopponent[3],kb.nothaving[0][3],kb.nothaving[1][3],kb.nothaving[2][3],played,-1,-1,P2,-1,decl_score,opp_score,3);
#endif
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p3 t0 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand3, kb.allopponent[3], kb.nothaving[0][3],
                             kb.nothaving[1][3], kb.nothaving[2][3], played, -1,
                             -1, -1, -1, decl_score, opp_score, 3);
      if ((g.team[3] == 0 && w > g.limit ||
           g.team[3] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << " p3 t1 can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << endl;
#endif
        return alphamu->proposed_card;
      }
    }
    return -1;
  }
  return chk;
}

int Play::P3_T1(card hand3, card played, int decl_score, int opp_score,
                int P2) {
  if (!killermove) return chk;
  chk = propose_opponents_card(hand3, played, P2);
  g.preferred_card = chk;
  if (killermove) {
    int i[PLAYSIZE] = {-1, -1, P2, -1};
    card reduced = g.playable_cards(hand3, 2, played, i);
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P3 T1 " << count(played) << " " << g.game;
    // alphamu->print(g,hand3,kb.allopponent[3],kb.nothaving[0][3],kb.nothaving[1][3],kb.nothaving[2][3],played,-1,-1,P2,-1,decl_score,opp_score,3);
#endif
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p3 t1 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand3, kb.allopponent[3], kb.nothaving[0][3],
                             kb.nothaving[1][3], kb.nothaving[2][3], played, -1,
                             -1, P2, -1, decl_score, opp_score, 3);
      if ((g.team[3] == 0 && w > g.limit ||
           g.team[3] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << " p3 t1 can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << endl;
#endif
        return alphamu->proposed_card;
      }
    }
    return -1;
  }
  return chk;
}

int Play::P3_T2(card hand3, card played, int decl_score, int opp_score, int P1,
                int P2) {
  if (!killermove) return chk;
  chk = propose_opponents_card(hand3, played, P1, P2);
  g.preferred_card = chk;
  if (killermove) {
    int i[PLAYER] = {-1, P1, P2, -1};
    card reduced = g.playable_cards(hand3, 1, played, i);
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P3 T2 " << count(played) << " " << g.game;
    // alphamu->print(g,hand3,kb.allopponent[3],kb.nothaving[0][3],kb.nothaving[1][3],kb.nothaving[2][3],played,-1,P1,P2,-1,decl_score,opp_score,3);
#endif
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p3 t2 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand3, kb.allopponent[3], kb.nothaving[0][3],
                             kb.nothaving[1][3], kb.nothaving[2][3], played, -1,
                             P1, P2, -1, decl_score, opp_score, 3);
      if ((g.team[3] == 0 && w > g.limit ||
           g.team[3] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << " p3 t2 can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << endl;
#endif
        return alphamu->proposed_card;
      }
    }
    return -1;
  }
  return chk;
}

int Play::P3_T3(card hand3, card played, int decl_score, int opp_score, int P0,
                int P1, int P2) {
  if (!killermove) return chk;
  chk = propose_opponents_card(hand3, played, P0, P1, P2);
  g.preferred_card = chk;
  if (killermove) {
    int i[PLAYER] = {P0, P1, P2, -1};
    card reduced = g.playable_cards(hand3, 0, played, i);
    if (count(reduced) == 1) return g.smallest(reduced);
#ifdef KNOWLEDGEPRINT
    cout << "P3 T3;" << count(played) << " ";
    // alphamu->print(g,hand3,kb.allopponent[3],kb.nothaving[0][3],kb.nothaving[1][3],kb.nothaving[2][3],played,P0,P1,P2,-1,decl_score,opp_score,3);
#endif
    if (count(played) >= FULLIITRICK) {
#ifdef KNOWLEDGEPRINT
      cout << "p2 t3 (" << count(played) << ") " << decl_score << " vs "
           << opp_score << endl;
#endif
      int w = alphamu->solve(g, hand3, kb.allopponent[3], kb.nothaving[0][3],
                             kb.nothaving[1][3], kb.nothaving[2][3], played, P0,
                             P1, P2, -1, decl_score, opp_score, 3);
      if ((g.team[3] == 0 && w > g.limit ||
           g.team[3] == 1 && w >= g.sumvalues - g.limit) &&
          alphamu->proposed) {
#ifdef KNOWLEDGEPRINT
        cout << " p3 t3 can win against all odds with strong card "
             << card_name[alphamu->proposed_card] << endl;
#endif
        return alphamu->proposed_card;
      }
    }
    return -1;
  }
  return chk;
}

Play::~Play() { delete alphamu; }

Play::Play(GamePlay game, card hand, int position) : kb(game, hand, position) {
  alphamu = new AlphaMu(&kb);  // worst-case analysis
  g = game;                    // copy all game information
  playerpos = position;        // 0 = N, 1 = E, 2 = S, 3 = W
  trumpmask = game.trumpmask;  // used as shortcut to g.trumpmask
  trumpsuit = -1;              // set in derived classes
  orighand = myhand = hand;    // orighand is initial deal
  round = 0;                   // counting rounds of play
  playtsize = 0;               // number of points
  for (int i = 0; i < DECKSIZE; i++) playtricks[i] = -1;     // for output
  for (int i = 0; i < DECKSIZE; i++) kb.playtricks[i] = -1;  // for output
  decl_score = opp_score = 0;                                // current scores
  played = 0;  // vector of played cards
  interactive = false;
}

int Play::choosePlayerCard() {
  //    if (killermove) {cout << endl << "choose killerMove with no card on
  //    table, P" << playerpos << " " << endl; g.print_vector(myhand,trumpmask);
  //    cout << endl;} else   {    cout << endl << "choose normalMove with no
  //    card on table, P" << playerpos << " " << endl;
  //    g.print_vector(myhand,trumpmask); cout << endl; }
  int index = -1;
  if (playerpos == 0)
    index = P0_T0(myhand, played, decl_score, opp_score);
  else if (playerpos == 1)
    index = P1_T0(myhand, played, decl_score, opp_score);
  else if (playerpos == 2)
    index = P2_T0(myhand, played, decl_score, opp_score);
  else if (playerpos == 3)
    index = P3_T0(myhand, played, decl_score, opp_score);
  //  cout << "found " << index << " km " << killermove << endl;
  if (interactive) {
    cout << endl << " Table ";
    cout << endl << " Hand ";
    g.print_vector(myhand, trumpmask);
    cout << endl << " Play ";
    g.print_extvector(g.playable_cards(myhand, played), trumpmask);
    //    cout << " AI Choice " << card_name[index] << endl;
    cout << "> ";
    cin >> index;
  }
  if (killermove && index == -1) return -1;
  //  cout << "found " << card_name[index] << endl;
  kb.playtable[playerpos][round] = index;

  //    cout << " I am player " << playerpos << "!";
  //    g.print_vector(myhand,trumpmask); cout << endl;
  if (!(myhand & (ONE << index))) {
    cout << "cpc/0 flaw in selecting " << card_name[index] << " game " << g.game
         << endl;
    g.print_vector(myhand, trumpmask);
    cout << " played:";
    g.print_vector(played, trumpmask);
    exit(1);
  }

  //  cout << "selecting " << index <<";" << playerpos << ";" << round << ";" <<
  //  playtsize << "/" << kb.playtsize << endl;
  //  g.print_vector(myhand,trumpmask);

  myhand = kb.myhand &= ~(ONE << index);
  playtricks[playtsize++] = index;
  kb.playtricks[kb.playtsize++] = index;
  played |= (ONE << index);
  kb.played |= (ONE << index);
  return index;
}

int Play::choosePlayerCard(int trickCard1) {
  int index = -1;
  //    if (killermove) cout << endl << "choose killerMove with one card "<<
  //    card_name[trickCard1] << " on table, P" << playerpos << " " << endl;
  //    g.print_vector(myhand,trumpmask); cout << endl;
  if (playerpos == 0)
    index = P0_T1(myhand, played, decl_score, opp_score, trickCard1);
  else if (playerpos == 1)
    index = P1_T1(myhand, played, decl_score, opp_score, trickCard1);
  else if (playerpos == 2)
    index = P2_T1(myhand, played, decl_score, opp_score, trickCard1);
  else if (playerpos == 3)
    index = P3_T1(myhand, played, decl_score, opp_score, trickCard1);
  //  cout << "found " << index << " km " << killermove << endl;
  //   cout << "selecting " << index <<";" << playerpos << ";" << round << endl;
  //   g.print_vector(myhand,trumpmask); cout << " I am player " << playerpos <<
  //   "!"; g.print_vector(myhand,trumpmask); cout << endl;
  if (interactive) {
    cout << endl << " Table " << card_name[trickCard1];
    card table = (ONE << trickCard1);
    cout << endl << " Hand ";
    g.print_vector(myhand, trumpmask);
    //    cout << " Played ";  g.print_extvector(played,trumpmask);
    //    cout << " Table ";  g.print_extvector(table,trumpmask);
    int issuer = (playerpos + 3) % PLAYER;
    int i[PLAYER] = {-1, -1, -1, -1};
    i[(playerpos + 3) % PLAYER] = trickCard1;
    cout << endl << " Play ";
    g.print_extvector(g.playable_cards(myhand, issuer, played, table, i),
                      trumpmask);
    cout << "> ";
    //   cout << " AI Choice " << card_name[index] << endl;
    cin >> index;
  }
  if (killermove && index == -1) return -1;
  //  cout << "found " << card_name[index] << endl;
  kb.playtable[playerpos][round] = index;
  if (!(myhand & (ONE << index))) {
    cout << "cpc/1 flaw in selecting " << card_name[index] << endl;
    g.print_vector(myhand, trumpmask);
    exit(1);
  }
  myhand = kb.myhand &= ~(ONE << index);
  playtricks[playtsize++] = index;
  kb.playtricks[kb.playtsize++] = index;
  played |= (ONE << index);
  kb.played |= (ONE << index);
  //  cout << "got here returning " << index << endl;
  return index;
}

int Play::choosePlayerCard(int trickCard1, int trickCard2) {
  int index = -1;
  //  cout << endl << "choose declarer card "<< SHOW(trickCard1) << " on table,
  //  P" << playerpos << " " << endl; g.print_vector(myhand,trumpmask); cout <<
  //  decl_score << " vs " << opp_score;
  if (playerpos == 0)
    index =
        P0_T2(myhand, played, decl_score, opp_score, trickCard1, trickCard2);
  else if (playerpos == 1)
    index =
        P1_T2(myhand, played, decl_score, opp_score, trickCard1, trickCard2);
  else if (playerpos == 2)
    index =
        P2_T2(myhand, played, decl_score, opp_score, trickCard1, trickCard2);
  else if (playerpos == 3)
    index =
        P3_T2(myhand, played, decl_score, opp_score, trickCard1, trickCard2);
  //  cout << "found " << index << " km " << killermove << endl;
  if (interactive) {
    cout << endl
         << " Table " << card_name[trickCard1] << " " << card_name[trickCard2];
    card table = (ONE << trickCard1) | (ONE << trickCard2);
    cout << endl << " Hand ";
    g.print_vector(myhand, trumpmask);
    //    cout << " Played ";  g.print_extvector(played,trumpmask);
    //    cout << " Table ";  g.print_extvector(table,trumpmask);
    int issuer = (playerpos + 2) % PLAYER;
    int i[PLAYER] = {-1, -1, -1, -1};
    i[(playerpos + 2) % PLAYER] = trickCard1;
    i[(playerpos + 3) % PLAYER] = trickCard2;
    cout << " Play ";
    g.print_extvector(g.playable_cards(myhand, issuer, played, table, i),
                      trumpmask);
    cout << "> ";
    //    cout << " AI Choice " << card_name[index] << endl;
    cin >> index;
  }
  if (killermove && index == -1) return -1;
  //  cout << "found " << card_name[index] << endl;
  kb.playtable[playerpos][round] = index;
  if (!(myhand & (ONE << index))) {
    cout << "cpc/2 flaw in selecting " << card_name[index] << endl;
    g.print_vector(myhand, trumpmask);
    exit(1);
  }
  myhand = kb.myhand &= ~(ONE << index);
  playtricks[playtsize++] = index;
  kb.playtricks[kb.playtsize++] = index;
  played |= (ONE << index);
  kb.played |= (ONE << index);
  return index;
}

int Play::winner(int T0, int T1, int T2, int T3, int *i) {
  int c1 = i[T0], c2 = i[T1], c3 = i[T2], c4 = i[T3];
  int r = g.win(c1, c2, c3, c4, trumpmask);
  if (r == c1) return 0;
  if (r == c2) return 1;
  if (r == c3) return 2;
  if (r == c4) return 3;
  return -1;
}

int Play::choosePlayerCard(int trickCard1, int trickCard2, int trickCard3) {
  int index;
  //  cout << endl << "choose player card with 3 cards on table, P" << playerpos
  //  << endl; ::g.print_vector(myhand,trumpmask);
  if (playerpos == 0) {
    index = P0_T3(myhand, played, decl_score, opp_score, trickCard1, trickCard2,
                  trickCard3);
    if (interactive) {
      cout << endl
           << " Table " << card_name[trickCard1] << " " << card_name[trickCard2]
           << " " << card_name[trickCard3];
      card table =
          (ONE << trickCard1) | (ONE << trickCard2) | (ONE << trickCard3);
      cout << endl << " Hand ";
      g.print_vector(myhand, trumpmask);
      //      cout << " Played ";  g.print_extvector(played,trumpmask);
      //      cout << " Table ";  g.print_extvector(table,trumpmask);
      int issuer = (playerpos + 1) % PLAYER;
      int i[PLAYER] = {-1, -1, -1, -1};
      i[(playerpos + 1) % PLAYER] = trickCard1;
      i[(playerpos + 2) % PLAYER] = trickCard2;
      i[(playerpos + 3) % PLAYER] = trickCard3;
      cout << endl << " Play ";
      g.print_extvector(g.playable_cards(myhand, issuer, played, table, i),
                        trumpmask);
      cout << "> ";
      //      cout << " AI Choice " << card_name[index] << endl;
      cin >> index;
    }
    //    cout << "found " << index << " km " << killermove << endl;
    if (killermove && index == -1) return -1;
    //    cout << "found " << card_name[index] << endl;
    playtricks[playtsize++] = index;
    kb.playtricks[kb.playtsize++] = index;
    /*
    if (trumpmask != 0) {
      int wp = winner(0,1,2,3,&playtricks[playtsize-4]);
      int wpos = (wp == 2) ? 0 : (wp == 1) ? 2 : 1; // ?????
      if (wpos % 2 == 0 && opp_score +
    COST(playtricks[playtsize-3],playtricks[playtsize-2],playtricks[playtsize-1],playtricks[playtsize-0])
    >= g.sumvalues-g.limit) { card reduced =
    g.playable_cards(myhand,trickCard1); // only consider valid cards while
    (reduced) { int next = g.smallest(reduced); playtricks[playtsize-1] = next;
    kb.playtricks[kb.playtsize-1] = next; int winnerpos =
    winner(0,1,2,3,&playtricks[playtsize-4]); int w = (playerpos + 1 +
    winnerpos) % PLAYER; if (w == 0) { index = next; break;
          }
          reduced &= ~(ONE<<next);
        }
        playtricks[playtsize-1] = index;
        kb.playtricks[kb.playtsize-1] = index;
      }
    }
    */
  } else if (playerpos == 1) {
    index = P1_T3(myhand, played, decl_score, opp_score, trickCard1, trickCard2,
                  trickCard3);
    if (interactive) {
      cout << endl
           << " Table " << card_name[trickCard1] << " " << card_name[trickCard2]
           << " " << card_name[trickCard3];
      card table =
          (ONE << trickCard1) | (ONE << trickCard2) | (ONE << trickCard3);
      cout << endl << " Hand ";
      g.print_vector(myhand, trumpmask);
      //      cout << " Played ";  g.print_extvector(played,trumpmask);
      //      cout << " Table ";  g.print_extvector(table,trumpmask);
      int issuer = (playerpos + 1) % PLAYER;
      int i[PLAYER] = {-1, -1, -1, -1};
      i[(playerpos + 1) % PLAYER] = trickCard1;
      i[(playerpos + 2) % PLAYER] = trickCard2;
      i[(playerpos + 3) % PLAYER] = trickCard3;
      cout << endl << " Play ";
      g.print_extvector(g.playable_cards(myhand, issuer, played, table, i),
                        trumpmask);
      cout << "> ";
      //      cout << " AI Choice " << card_name[index] << endl;
      cin >> index;
    }
    //    cout << "found " << index << " km " << killermove << endl;
    if (killermove && index == -1) return -1;
    //    cout << "found " << card_name[index] << endl;
    playtricks[playtsize++] = index;
    kb.playtricks[kb.playtsize++] = index;
    /*
    if (trumpmask != 0) {
      int wp = winner(0,1,2,3,&playtricks[playtsize-4]);
      int wpos = (wp == 2) ? 1 : (wp == 1) ? 0 : 2; // ????
      if (wpos % 2 == 0 && decl_score +
    COST(playtricks[playtsize-3],playtricks[playtsize-2],playtricks[playtsize-1],playtricks[playtsize-0])
    > g.limit) { card reduced = g.playable_cards(myhand,trickCard1); // only
    consider valid cards while (reduced) { int next = g.smallest(reduced);
          playtricks[playtsize-1] = next; kb.playtricks[kb.playtsize-1] = next;
          int winnerpos = winner(0,1,2,3,&playtricks[playtsize-4]);
          int w = (playerpos + 1 + winnerpos) % PLAYER;
          if (w > 0) {
            index = next;
            break;
          }
          reduced &= ~(ONE<<next);
        }
        playtricks[playtsize-1] = index; kb.playtricks[kb.playtsize-1] = index;
      }
    }
    */
  } else if (playerpos == 2) {
    index = P2_T3(myhand, played, decl_score, opp_score, trickCard1, trickCard2,
                  trickCard3);
    if (interactive) {
      cout << endl
           << " Table " << card_name[trickCard1] << " " << card_name[trickCard2]
           << " " << card_name[trickCard3];
      card table =
          (ONE << trickCard1) | (ONE << trickCard2) | (ONE << trickCard3);
      cout << endl << " Hand ";
      g.print_vector(myhand, trumpmask);
      //      cout << " Played ";  g.print_extvector(played,trumpmask);
      //      cout << " Table ";  g.print_extvector(table,trumpmask);
      int issuer = (playerpos + 1) % PLAYER;
      int i[PLAYER] = {-1, -1, -1, -1};
      i[(playerpos + 1) % PLAYER] = trickCard1;
      i[(playerpos + 2) % PLAYER] = trickCard2;
      i[(playerpos + 3) % PLAYER] = trickCard3;
      cout << endl << " Play ";
      g.print_extvector(g.playable_cards(myhand, issuer, played, table, i),
                        trumpmask);
      cout << "> ";
      //      cout << " AI Choice " << card_name[index] << endl;
      cin >> index;
    }
    //    cout << "found " << index << " km " << killermove << endl;
    if (killermove && index == -1) return -1;
    //    cout << "found " << card_name[index] << endl;
    playtricks[playtsize++] = index;
    kb.playtricks[kb.playtsize++] = index;
    /*
    if (trumpmask != 0) {
      int wpos = winner(0,1,2,3,&playtricks[playtsize-4]);
      if (wpos % 2 == 0 && opp_score +
    COST(playtricks[playtsize-3],playtricks[playtsize-2],playtricks[playtsize-1],playtricks[playtsize-0])
    >= g.sumvalues - g.limit) { card reduced =
    g.playable_cards(myhand,trickCard1); // only consider valid cards while
    (reduced) { int next = g.smallest(reduced); playtricks[playtsize-1] = next;
          kb.playtricks[kb.playtsize-1] = next;
          int winnerpos = winner(0,1,2,3,&playtricks[playtsize-4]);
          int w = (playerpos + 1 + winnerpos) % PLAYER;
          if (w > 0) {
            index = next;
            break;
          }
          reduced &= ~(ONE<<next);
        }
        playtricks[playtsize-1] = index;
        kb.playtricks[kb.playtsize-1] = index;
      }
    }
    */
  } else if (playerpos == 3) {
    index = P3_T3(myhand, played, decl_score, opp_score, trickCard1, trickCard2,
                  trickCard3);
    if (interactive) {
      cout << endl
           << " Table " << card_name[trickCard1] << " " << card_name[trickCard2]
           << " " << card_name[trickCard3];
      card table =
          (ONE << trickCard1) | (ONE << trickCard2) | (ONE << trickCard3);
      cout << endl << " Hand ";
      g.print_vector(myhand, trumpmask);
      //      cout << " Played ";  g.print_extvector(played,trumpmask);
      //      cout << " Table ";  g.print_extvector(table,trumpmask);
      int issuer = (playerpos + 1) % PLAYER;
      int i[PLAYER] = {-1, -1, -1, -1};
      i[(playerpos + 1) % PLAYER] = trickCard1;
      i[(playerpos + 2) % PLAYER] = trickCard2;
      i[(playerpos + 3) % PLAYER] = trickCard3;
      cout << endl << " Play ";
      g.print_extvector(g.playable_cards(myhand, issuer, played, i), trumpmask);
      //      cout << " AI Choice " << card_name[index] << endl;
      cin >> index;
    }
    //    cout << "found " << index << " km " << killermove << endl;
    if (killermove && index == -1) return -1;
    //    cout << "found " << card_name[index] << endl;
    playtricks[playtsize++] = index;
    kb.playtricks[kb.playtsize++] = index;
    /*
    if (trumpmask != 0) {
      int wpos = winner(0,1,2,3,&playtricks[playtsize-4]);
      if (wpos % 2 == 0 && decl_score +
    COST(playtricks[playtsize-3],playtricks[playtsize-2],playtricks[playtsize-1],playtricks[playtsize-0])
    > g.limit) { card reduced = g.playable_cards(myhand,trickCard1); // only
    consider valid cards while (reduced) { int next = g.smallest(reduced);
          playtricks[playtsize-1] = next;
          kb.playtricks[kb.playtsize-1] = next;
          int winnerpos = winner(0,1,2,3,&playtricks[playtsize-4]);
          int w = (playerpos + 1 + winnerpos) % PLAYER;
          if (w > 0) {
            index = next;
            break;
          }
          reduced &= ~(ONE<<next);
        }
        playtricks[playtsize-1] = index;
        kb.playtricks[kb.playtsize-1] = index;
      }
    }
    */
  }
  kb.playtable[playerpos][round] = index;
  if (!(myhand & (ONE << index))) {
    cout << "cpc/3 flaw in obeying (" << index << ")" << card_name[index]
         << endl;
    exit(1);
  }
  myhand = kb.myhand &= ~(ONE << index);
  round++;
  int winnerpos = winner(0, 1, 2, 3, &playtricks[playtsize - 4]);
  int w = (playerpos + 1 + winnerpos) % PLAYER;
  if (g.team[w] == 0)
    kb.decl_score = decl_score +=
        COST(playtricks[playtsize - 4], playtricks[playtsize - 3],
             playtricks[playtsize - 2], playtricks[playtsize - 1]);
  else
    kb.opp_score = opp_score +=
        COST(playtricks[playtsize - 4], playtricks[playtsize - 3],
             playtricks[playtsize - 2], playtricks[playtsize - 1]);
  played |= (ONE << index);
  kb.played |= (ONE << index);
  return index;
}

int Play::informPlayerCard(int pos, int trickCard) {
  //  cout << endl << "inform player " << playerpos << " that player " << pos <<
  //  " played card " << SHOW(trickCard) << endl;
  //  ::g.print_vector(myhand,trumpmask);
  playtricks[playtsize++] = trickCard;
  kb.playtricks[kb.playtsize++] = trickCard;
  kb.playtable[pos][round] = trickCard;
  if ((playtsize % PLAYER) == 0) {
    round++;
    int winnerpos = winner(0, 1, 2, 3, &playtricks[playtsize - 4]);
    int w = (pos + 1 + winnerpos) % PLAYER;
    if (g.team[w] == 0)
      kb.decl_score = decl_score +=
          COST(playtricks[playtsize - 4], playtricks[playtsize - 3],
               playtricks[playtsize - 2], playtricks[playtsize - 1]);
    else
      kb.opp_score = opp_score +=
          COST(playtricks[playtsize - 4], playtricks[playtsize - 3],
               playtricks[playtsize - 2], playtricks[playtsize - 1]);
  }
  played |= (ONE << trickCard);
  kb.played |= (ONE << trickCard);
  return 0;
}

int Play::suggestCard() {  // only called after count belief
  long long chands = kb.countrek;
  if (chands > BSSIZE) return -1;
  // cout << "suggest card" << chands << endl;
  int suggest = -1;
  card reduced = 0;

  if (playtsize % PLAYER == 3) {
    int issuer = (playerpos + 1) % PLAYER;
    int i[PLAYER] = {-1, -1, -1, -1};
    int trickCard1 = playtricks[playtsize - 3];
    int trickCard2 = playtricks[playtsize - 2];
    int trickCard3 = playtricks[playtsize - 1];
    i[issuer] = trickCard1;
    i[(issuer + 1) % PLAYER] = trickCard2;
    i[(issuer + 2) % PLAYER] = trickCard3;
    reduced = g.playable_cards(myhand, issuer, played, i);
  }
  if (playtsize % PLAYER == 2) {
    int issuer = (playerpos + 2) % PLAYER;
    int i[PLAYER] = {-1, -1, -1, -1};
    int trickCard1 = playtricks[playtsize - 2];
    int trickCard2 = playtricks[playtsize - 1];
    i[issuer] = trickCard1;
    i[(issuer + 1) % PLAYER] = trickCard2;
    reduced = g.playable_cards(myhand, issuer, played, i);
  }
  if (playtsize % PLAYER == 1) {
    int issuer = (playerpos + 3) % PLAYER;
    int i[PLAYER] = {-1, -1, -1, -1};
    int trickCard1 = playtricks[playtsize - 1];
    i[issuer] = trickCard1;
    reduced = g.playable_cards(myhand, issuer, played, i);
  }
  if (playtsize % PLAYER == 0) {
    reduced = g.playable_cards(myhand, played);
  }
  if (count(reduced) == 1) {
    int index = g.smallest(reduced);
    kb.playtable[playerpos][round] = index;
    myhand = kb.myhand &= ~(ONE << index);
    playtricks[playtsize++] = index;
    kb.playtricks[kb.playtsize++] = index;
    if ((playtsize % PLAYER) == 0) {
      round++;
      int winnerpos = winner(0, 1, 2, 3, &playtricks[playtsize - 3]);
      int w = (playerpos + 1 + winnerpos) % PLAYER;
      if (g.team[w] == 0)
        kb.decl_score = decl_score +=
            COST(playtricks[playtsize - 4], playtricks[playtsize - 3],
                 playtricks[playtsize - 2], playtricks[playtsize - 1]);
      else
        kb.opp_score = opp_score +=
            COST(playtricks[playtsize - 4], playtricks[playtsize - 3],
                 playtricks[playtsize - 2], playtricks[playtsize - 1]);
    }
    played |= (ONE << index);
    kb.played |= (ONE << index);
#ifdef KNOWLEDGEPRINT
    cout << " suggestion with only one option to play " << card_name[suggest]
         << endl;
#endif
    return index;
  }
#ifdef KNOWLEDGEPRINT
  cout << " suggest P" << playerpos << "'s card " << chands << " worlds, score "
       << decl_score << ":" << opp_score << endl;
#endif
  if (chands >= BSSIZE) return -1;
  for (int i = 0; i < DECKSIZE; i++)
    kb.winning_cards[i] = kb.suggested_cards[i] = kb.counted_wins[i] = 0;
  if (playerpos == 0) {
    if (chands > 0 && chands < BSSIZE) {
      kb.generate_hands(kb.allopponent[0], kb.nothaving[1][0],
                        kb.nothaving[2][0], kb.nothaving[3][0], 0);
    }
  } else if (playerpos == 1) {
    if (chands > 0 && chands < BSSIZE) {
      kb.generate_hands(kb.allopponent[1], kb.nothaving[0][1],
                        kb.nothaving[2][1], kb.nothaving[3][1], 1);
    }
  } else if (playerpos == 2) {
    if (chands > 0 && chands < BSSIZE) {
      kb.generate_hands(kb.allopponent[2], kb.nothaving[0][2],
                        kb.nothaving[1][2], kb.nothaving[3][2], 2);
    }
  } else if (playerpos == 3) {
    if (chands > 0 && chands < BSSIZE) {
      kb.generate_hands(kb.allopponent[3], kb.nothaving[0][3],
                        kb.nothaving[1][3], kb.nothaving[2][3], 3);
    }
  }
  card trick = 0;
  if ((playtsize % PLAYER) == 3)
    trick |= (ONE << playtricks[playtsize - 3]) |
             (ONE << playtricks[playtsize - 2]) |
             (ONE << playtricks[playtsize - 1]);
  if ((playtsize % PLAYER) == 2)
    trick |=
        (ONE << playtricks[playtsize - 1]) | (ONE << playtricks[playtsize - 2]);
  if ((playtsize % PLAYER) == 1) trick |= (ONE << playtricks[playtsize - 1]);
#ifdef KNOWLEDGEPRINT
  for (int i = 0; i < DECKSIZE; i++) std::cout << "; " << card_name[i];
  std::cout << std::endl;
  for (int i = 0; i < DECKSIZE; i++)
    std::cout << "; " << (int)kb.suggested_cards[i];
  std::cout << std::endl;
  for (int i = 0; i < DECKSIZE; i++) std::cout << "; " << kb.winning_cards[i];
  std::cout << std::endl;
#endif
  if (chands > 0 && chands < BSSIZE) {
    int mini = 0, normalize = 0;
    for (int t = 0; t < DECKSIZE;
         t++) {  // null ordering A,K,D,10,9,8,7 - kr,pi,he,ka
      normalize += kb.suggested_cards[t];
      if (kb.suggested_cards[t] > mini) {
        mini = kb.suggested_cards[t];
        suggest = t;
      }
    }
    if (suggest != -1 && ((double)mini / normalize) > BSCONF) {
      int index = suggest;
      if (!(myhand & (ONE << suggest))) {
        return -1;  // accept errors in suggestion
        cout << "suggest, flaw in selecting " << SHOW(index) << endl;
        g.print_vector(myhand, trumpmask);
        exit(1);
      }
      kb.playtable[playerpos][round] = index;
      myhand = kb.myhand &= ~(ONE << index);
      playtricks[playtsize++] = kb.playtricks[kb.playtsize++] = index;
      if ((playtsize % PLAYER) == 0) {
        round++;
        int winnerpos = winner(0, 1, 2, 3, &playtricks[playtsize - 4]);
        int w = (playerpos + 1 + winnerpos) % PLAYER;
        if (g.team[w] == 0)
          kb.decl_score = decl_score +=
              COST(playtricks[playtsize - 4], playtricks[playtsize - 3],
                   playtricks[playtsize - 2], playtricks[playtsize - 1]);
        else
          kb.opp_score = opp_score +=
              COST(playtricks[playtsize - 4], playtricks[playtsize - 3],
                   playtricks[playtsize - 2], playtricks[playtsize - 1]);
      }
      played |= (ONE << index);
      kb.played |= (ONE << index);
#ifdef KNOWLEDGEPRINT
      cout << " suggestion made on confidence " << card_name[suggest] << endl;
#endif
      return suggest;
    } else {
#ifdef NOHOPE
      return -1;
#endif
#ifdef KNOWLEDGEPRINT
      cout << "no suggestion made based on confidence " << endl;
#endif
      int mini = 0;
      suggest = -1;
      int sole = false, i = 0, l;
      for (; i < DECKSIZE; i++) {
        int l = i;
        if (kb.winning_cards[l] > 0) {
          if (!sole) {
            sole = true;
            mini = kb.winning_cards[l];
            suggest = l;
          } else {
            sole = false;
            break;
          }
        }
      }
      card potcards = 0;  // potentially winning
      int potworlds = 0;  // potentially winning
      for (int j = 0; j < DECKSIZE; j++) {
        if (kb.winning_cards[j] > 0) {
          potcards |= (ONE << j);
          potworlds += kb.winning_cards[j];
        }
      }
      //      if ((playtsize % PLAYER) != 0 && playerpos > 0) potcards =
      //      reduced;
      int m = 0, s = -1;
      double sumwins = 0;
      for (int i = 0; i < DECKSIZE; i++) kb.counted_wins[i] = 0;
      {
        if (potworlds) {
#ifdef KNOWLEDGEPRINT
          cout << "pot worlds " << potworlds << " of " << chands;
#endif
          kb.g.limit = g.limit;
          if (playerpos == 0) {
            kb.generate_winning_hands(kb.allopponent[0], kb.nothaving[1][0],
                                      kb.nothaving[2][0], kb.nothaving[3][0], 0,
                                      potcards);
          } else if (playerpos == 1) {
            kb.generate_winning_hands(kb.allopponent[1], kb.nothaving[0][1],
                                      kb.nothaving[2][1], kb.nothaving[3][1], 1,
                                      potcards);
          } else if (playerpos == 2) {
            kb.generate_winning_hands(kb.allopponent[2], kb.nothaving[0][2],
                                      kb.nothaving[1][2], kb.nothaving[3][2], 2,
                                      potcards);
          } else if (playerpos == 3) {
            kb.generate_winning_hands(kb.allopponent[3], kb.nothaving[0][3],
                                      kb.nothaving[1][3], kb.nothaving[2][3], 3,
                                      potcards);
          }
        }
#ifdef KNOWLEDGEPRINT
        for (int k = 0; k < DECKSIZE; k++)
          std::cout << card_name[k] << ":" << kb.counted_wins[k] << "; ";
        std::cout << std::endl;
#endif
        for (int j = 0; j < DECKSIZE;
             j++) {  // null ordering A,K,D,10,9,8,7 - kr,pi,he,ka
          int t = j;
          if (kb.counted_wins[t] > m) {
            m = kb.counted_wins[t];
            s = t;
          }
        }
        //	cout << "BEST CARD:" << card_name[s] << " with " << m << " and "
        //<< kb.winning_cards[s] << endl;;
        int maxi = kb.winning_cards[s];
        for (int j = 0; j < DECKSIZE;
             j++) {  // null ordering A,K,D,10,9,8,7 - kr,pi,he,ka
          int t = j;
          if (m > 0 && kb.counted_wins[t] == m &&
              kb.suggested_cards[t] > maxi) {
            s = t;
            maxi = kb.suggested_cards[t];
          }
        }
        //		cout << "BEST CARD (CORRECTED):" << card_name[s] << "
        // with " << m << endl;;
        for (int j = 0; j < DECKSIZE;
             j++) {  // null ordering A,K,D,10,9,8,7 - kr,pi,he,ka
          int t = j;
          if (kb.counted_wins[t] > 0) {
            sumwins += kb.counted_wins[t];
          }
        }
      }

      if (s != -1 && suggest != -1 && sole && m == potworlds && s != suggest)
        suggest = s;

      cout << fixed << setprecision(2);
      double best = 0.0, pwin = 0.80;
      //      cout <<" hope needed "<< (double)m / potworlds << endl;
      if (i == DECKSIZE && sole && suggest != -1 ||
          s != -1 && (double)m / potworlds >= pwin) {  // only one card

#ifdef KNOWLEDGEPRINT
        if (i == DECKSIZE && sole && suggest != -1)
          cout << "no suggestion made with confidence, playing only hope card "
               << card_name[suggest] << " P" << playerpos << endl;
#endif
        if (!(i == DECKSIZE && sole && suggest != -1) && s != -1 &&
            (double)m == potworlds) {
          suggest = s;
#ifdef KNOWLEDGEPRINT
          cout << "no suggestion made with confidence, playing best hope card "
               << card_name[suggest] << " P" << playerpos << endl;
#endif
        } else if (!(i == DECKSIZE && sole && suggest != -1) && s != -1 &&
                   ((double)m / potworlds >= pwin)) {
          suggest = s;
#ifdef KNOWLEDGEPRINT
          cout << "no suggestion made with confidence, playing "
               << ((int)(((double)m / potworlds) * 100)) << "% hope card "
               << card_name[suggest] << " P" << playerpos << endl;
#endif
        }
        cout << fixed << setprecision(2);
        int index = suggest;
        if (!(myhand & (ONE << suggest))) {
          return -1;  // accept errors in suggestion
          cout << "suggest, flaw in selecting " << SHOW(index) << endl;
          g.print_vector(myhand, trumpmask);
          exit(1);
        }
        kb.playtable[playerpos][round] = index;
        myhand = kb.myhand &= g.ALLBITS & ~(ONE << index);
        playtricks[playtsize++] = kb.playtricks[kb.playtsize++] = index;
        if ((playtsize % PLAYER) == 0) {
          round++;
          int winnerpos = winner(0, 1, 2, 3, &playtricks[playtsize - 4]);
          int w = (playerpos + 1 + winnerpos) % PLAYER;
          if (g.team[w] == 0)
            kb.decl_score = decl_score +=
                COST(playtricks[playtsize - 4], playtricks[playtsize - 3],
                     playtricks[playtsize - 2], playtricks[playtsize - 1]);
          else
            kb.opp_score = opp_score +=
                COST(playtricks[playtsize - 4], playtricks[playtsize - 3],
                     playtricks[playtsize - 2], playtricks[playtsize - 1]);
        }
        played |= (ONE << index);
        kb.played |= (ONE << index);
        return suggest;
      } else {
#ifdef KNOWLEDGEPRINT
        cout << "no suggestion made with confidence, no hope card P"
             << playerpos << endl;
#endif
        ;
      }
    }
  }
#ifdef KNOWLEDGEPRINT
  cout << "no suggestion made based on too few or too many worlds " << endl;
#endif
  return -1;
}

int Play::dropped(int querycard,
                  int wincard) {  // returns suit of dropping suit
  if (trumpmask & (ONE << wincard)) {
    if (~trumpmask & (ONE << querycard)) return g.Suit[querycard];
  } else {
    int suit = g.Suit[wincard];
    card colormask = g.Mask[suit];
    if (~colormask & (ONE << querycard)) return g.Suit[querycard];
  }
  return -1;
}
