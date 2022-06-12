// 4-player card playing program using expert knowledge, expert games, belief states, and-or search
// (c) 2022, Stefan Edelkamp


#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "cards.h"
#include "game.h"
#include "knowledge.h"


using namespace std;

int KnowledgeBase::Opponentsnothavingtrump() {
  if (playerpos == 0)
    return
      count(nothaving[1][0] & trumpmask) == count(allopponent[0] & trumpmask) &&
      count(nothaving[3][0] & trumpmask) == count(allopponent[0] & trumpmask);
  if (playerpos == 1)
    return
      count(nothaving[2][1] & trumpmask) == count(allopponent[1] & trumpmask) &&
      count(nothaving[3][1] & trumpmask) == count(allopponent[1] & trumpmask);
  if (playerpos == 2)
    return
      count(nothaving[1][2] & trumpmask) == count(allopponent[2] & trumpmask) &&
      count(nothaving[3][2] & trumpmask) == count(allopponent[2] & trumpmask);
  if (playerpos == 3)
    return
      count(nothaving[1][3] & trumpmask) == count(allopponent[3] & trumpmask) &&
      count(nothaving[2][3] & trumpmask) == count(allopponent[3] & trumpmask);
  return 0; 
}

int KnowledgeBase::Partnernothavingtrump() {
  if (playerpos == 0) 
    return count(nothaving[2][0] & trumpmask) == count(allopponent[0] & trumpmask);
  if (playerpos == 2)
    return count(nothaving[0][2] & trumpmask) == count(allopponent[2] & trumpmask);
  if (playerpos == 1) 
    return count(nothaving[3][1] & trumpmask) == count(allopponent[1] & trumpmask);
  if (playerpos == 3)
    return count(nothaving[1][3] & trumpmask) == count(allopponent[3] & trumpmask);
  return 0; 
}

int KnowledgeBase::Nextnothavingtrump() {
  if (playerpos == 0) 
    return count(nothaving[1][0] & trumpmask) == count(allopponent[0] & trumpmask); 
  if (playerpos == 1)
    return count(nothaving[2][1] & trumpmask) == count(allopponent[1] & trumpmask);
  if (playerpos == 2)
    return count(nothaving[3][2] & trumpmask) == count(allopponent[2] & trumpmask); 
  if (playerpos == 3)
    return count(nothaving[0][3] & trumpmask) == count(allopponent[3] & trumpmask); 
  return 0; 
}

int KnowledgeBase::Nextnothavingsuit(int suit) {
  if (playerpos == 0) 
    return count(nothaving[1][0] & g.Mask[suit]) == count(allopponent[0] & g.Mask[suit]); 
  if (playerpos == 1)
    return count(nothaving[2][1] & g.Mask[suit]) == count(allopponent[1] & g.Mask[suit]);
  if (playerpos == 2)
    return count(nothaving[3][2] & g.Mask[suit]) == count(allopponent[2] & g.Mask[suit]); 
  if (playerpos == 3)
    return count(nothaving[0][3] & g.Mask[suit]) == count(allopponent[3] & g.Mask[suit]); 
  return 0;
}

bool KnowledgeBase::P0hasplayedsuit(int suit) {
  card played0 = 0;
  for (int i = 0; i < MAXTRICKS; i++) 
    if (playtable[0][i] != -1) played0 |= (ONE<<playtable[0][i]);
  if (played0 & g.Mask[suit]) return true;
  return false;
}

bool KnowledgeBase::P1hasplayedsuit(int suit) {
  card played1 = 0;
  for (int i = 0; i < MAXTRICKS; i++) 
    if (playtable[1][i] != -1) played1 |= (ONE<<playtable[1][i]);
  if (played1 & g.Mask[suit] ) return true;
  return false;
}

bool KnowledgeBase::P2hasplayedsuit(int suit) {
  card played2 = 0;
  for (int i = 0; i < MAXTRICKS; i++) 
    if (playtable[2][i] != -1) played2 |= (ONE<<playtable[2][i]);
  if (played2 & g.Mask[suit] ) return true;
  return false;
}

bool KnowledgeBase::P3hasplayedsuit(int suit) {
  card played3 = 0;
  for (int i = 0; i < MAXTRICKS; i++) 
    if (playtable[3][i] != -1) played3 |= (ONE<<playtable[3][i]);
  if (played & g.Mask[suit]) return true;
  return false;
}

KnowledgeBase::KnowledgeBase(GamePlay game,card hand, int position) { // constructor
  playerpos = position; // AS = 0, P1 = 1, P2 = 2
  g = game; // copy current game information
  myhand = hand; // current hand
  played = 0; playtsize = 0; // cards played and vector
  decl_score = opp_score = 0; // current score
  AOS = new OpenCard(game); // for endgame play
  trumpmask = game.trumpmask; // set game trump vector
  g.trumpsuit = trumpsuit = -1; // set gamesuit
  if (game.game == 9) g.trumpsuit = trumpsuit = 3;
  if (game.game == 10) g.trumpsuit = trumpsuit = 2;
  if (game.game == 11) g.trumpsuit = trumpsuit = 1;
  if (game.game == 12) g.trumpsuit = trumpsuit = 0;
  if (game.game == 0) g.trumpsuit = -1; trumpmask = 0;
  for (int i=0;i<4;i++) {
    allopponent[i] = 0;
    for (int j=0;j<4;j++)
      nothaving[i][j] == 0;
  }
  for (int i = 0; i < MAXTRICKS; i++) {
    for (int j=0;j<PLAYER;j++)
      playtable[j][i] = -1;
  }
  for (int i = 0; i < DECKSIZE; i++)
    playtricks[i] = -1;
  playtsize = 0;
  allopponent[playerpos] = g.ALLBITS & ~myhand; 
 
  for (int i=0;i<DECKSIZE;i++)
    for (int p=0;p<PLAYER;p++)
      belief[p][i] = -1;
}

void KnowledgeBase::generate_worlds_recursive1(card rest, card h1, card h2, card h3, card not1, card not2, card not3, int turn) {
  if (count(rest) == 0) {
    worlds++;
    if (worlds % ratio == 0) { 
      if (playerpos == 0) allworlds.push_back(World(myhand,h1,h2,h3));
      else if (playerpos == 1) allworlds.push_back(World(h1,myhand,h2,h3));
      else if (playerpos == 2) allworlds.push_back(World(h1,h2,myhand,h3));
      else allworlds.push_back(World(h1,h2,h3,myhand));
    }
    return;
  }
  int firstcard = CTZ(rest);
  card bitcard = ONE<<firstcard;
  if (count(h1) < sampleh1 && !(bitcard & not1)) 
    generate_worlds_recursive1(rest & ~bitcard, h1 | bitcard, h2, h3, not1, not2, not3, turn);
  if (count(h3) < sampleh3 && !(bitcard & not3)) 
    generate_worlds_recursive1(rest & ~bitcard, h1, h2, h3 | bitcard, not1, not2, not3, turn);
}

void KnowledgeBase::generate_recursive1(card rest, card h1, card h2, card h3, card not1, card not2, card not3, int turn) {
  if (count(rest) == 0) {
    /*
      cout << "Next ";
      g.print_vector(rest,trumpmask); cout << "|";
      g.print_vector(h1,trumpmask); cout << "|";
      g.print_vector(h2,trumpmask);cout << "|";
      g.print_vector(h3,trumpmask);cout << "|";
      cout << "(" << turn << ")";
      cout << endl;
    */
    int pts = -1;
    if (turn % 2 == 1) { // for opponents     
      if (playerpos == 0) {
	if (playtsize % PLAYER == 0) {
           pts = AOS->solve(g,myhand,h1,h2,h3,played,-1,-1,-1,-1,decl_score,opp_score,0);
	}
	else if (playtsize % PLAYER == 1) {
	  int P3 = playtricks[playtsize-1];
	  pts = AOS->solve(g,myhand,h1,h2,h3,played|(ONE<<P3),-1,-1,-1,P3,decl_score,opp_score,0);
	}
	else if (playtsize % PLAYER == 2) {
	  int P2 = playtricks[playtsize-2], P3 = playtricks[playtsize-1];
	  pts = AOS->solve(g,myhand,h1,h2,h3,played|(ONE<<P2)|(ONE<<P3),-1,-1,P2,P3,decl_score,opp_score,0);
	}
	else if (playtsize % PLAYER == 3) {
	  int P1 = playtricks[playtsize-3], P2 = playtricks[playtsize-2], P3 = playtricks[playtsize-1];
	  pts = AOS->solve(g,myhand,h1,h2,h3,played|(ONE<<P1)|(ONE<<P2)|(ONE<<P3),-1,P1,P2,P3,decl_score,opp_score,0);
	}
      }
      else if (playerpos == 1) {
	if (playtsize % PLAYER == 0) {
	  pts = AOS->solve(g,h1,myhand,h2,h3,played,-1,-1,-1,-1,decl_score,opp_score,1);
	}
	else if (playtsize % PLAYER == 1) {
	  int P0 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,myhand,h2,h3,played|(ONE<<P0),P0,-1,-1,-1,decl_score,opp_score,1);
	}
	else if (playtsize % PLAYER == 2) {
	  int P3 = playtricks[playtsize-2], P0 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,myhand,h2,h3,played|(ONE<<P0)|(ONE<<P3),P0,-1,-1,P3,decl_score,opp_score,1);
	}
	else if (playtsize % PLAYER == 3) {
	  int P2 = playtricks[playtsize-3], P3 = playtricks[playtsize-2], P0 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,myhand,h2,h3,played|(ONE<<P2)|(ONE<<P3)|(ONE<<P0),P0,-1,P2,P3,decl_score,opp_score,1);
	}
      }
      else if (playerpos == 2) {
	if (playtsize % PLAYER == 0) {
	  pts = AOS->solve(g,h1,h2,myhand,h3,played,-1,-1,-1,-1,decl_score,opp_score,2);	    
	}
	else if (playtsize % PLAYER == 1) {
	  int P1 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,h2,myhand,h3,played|(ONE<<P1),-1,P1,-1,-1,decl_score,opp_score,2);
	}
	else if (playtsize % PLAYER == 2) {
	  int P0 = playtricks[playtsize-2], P1 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,h2,myhand,h3,played|(ONE<<P0)|(ONE<<P1),P0,P1,-1,-1,decl_score,opp_score,2);
	}
	else if (playtsize % PLAYER == 3) {
	  int P3 = playtricks[playtsize-3], P0 = playtricks[playtsize-2], P1 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,h2,myhand,h3,played|(ONE<<P0)|(ONE<<P1)|(ONE<<P3),P0,P1,-1,P3,decl_score,opp_score,2);
	}
      }
      else if (playerpos == 3) {
	if (playtsize % PLAYER == 0) {
	  pts = AOS->solve(g,h1,h2,h3,myhand,played,-1,-1,-1,-1,decl_score,opp_score,3);	    
	}
	else if (playtsize % PLAYER == 1) {
	  int P2 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,h2,h3,myhand,played|(ONE<<P2),-1,-1,P2,-1,decl_score,opp_score,3);	    
	}
	else if (playtsize % PLAYER == 2) {
	  int P1 = playtricks[playtsize-2], P2 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,h2,h3,myhand,played|(ONE<<P2)|(ONE<<P1),-1,P1,P2,-1,decl_score,opp_score,3);
	}
	else if (playtsize % PLAYER == 3) {
	  int P0 = playtricks[playtsize-3], P1 = playtricks[playtsize-2], P2 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,h2,h3,myhand,played|(ONE<<P0)|(ONE<<P1)|(ONE<<P2),P0,P1,P2,-1,decl_score,opp_score,3);
	}
      }
      int index = AOS->proposed_card;
      if (index != -1) {
	if (index > DECKSIZE || index < 0) { cout << "ERROR wrong index" << endl; exit(1); } 
	suggested_cards[index] += (g.sumvalues - pts);
	results[worlds++] = (pts <= g.limit);
	if (pts <= g.limit) {	  
	  winning_cards[index]++;
	  suggested_cards[index]+=10;
	}
      }
    } 
    else {
      if (playerpos == 0) {
	if (playtsize % PLAYER == 0) {
	  pts = AOS->solve(g,myhand,h1,h2,h3,played,-1,-1,-1,-1,decl_score,opp_score,0);
	}
	else if (playtsize % PLAYER == 1) {
	  int P3 = playtricks[playtsize-1];
	  pts = AOS->solve(g,myhand,h1,h2,h3,played|(ONE<<P3),-1,-1,-1,P3,decl_score,opp_score,0);
	}
	else if (playtsize % PLAYER == 2) {
	  int P2 = playtricks[playtsize-2], P3 = playtricks[playtsize-1];
	  pts = AOS->solve(g,myhand,h1,h2,h3,played|(ONE<<P2)|(ONE<<P3),-1,-1,P2,P3,decl_score,opp_score,0);
	}
	else if (playtsize % PLAYER == 3) {
	  int P1 = playtricks[playtsize-3], P2 = playtricks[playtsize-2], P3 = playtricks[playtsize-1];
	  pts = AOS->solve(g,myhand,h1,h2,h3,played|(ONE<<P1)|(ONE<<P2)|(ONE<<P3),-1,P1,P2,P3,decl_score,opp_score,0);
	}
      }
      else if (playerpos == 1) {
	if (playtsize % PLAYER == 0) {
	  pts = AOS->solve(g,h1,myhand,h2,h3,played,-1,-1,-1,-1,decl_score,opp_score,1);
	}
	else if (playtsize % PLAYER == 1) {
	  int P0 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,myhand,h2,h3,played|(ONE<<P0),P0,-1,-1,-1,decl_score,opp_score,1);
	}
	else if (playtsize % PLAYER == 2) {
	  int P3 = playtricks[playtsize-2], P0 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,myhand,h2,h3,played|(ONE<<P0)|(ONE<<P3),P0,-1,-1,P3,decl_score,opp_score,1);
	}
	else if (playtsize % PLAYER == 3) {
	  int P2 = playtricks[playtsize-3], P3 = playtricks[playtsize-2], P0 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,myhand,h2,h3,played|(ONE<<P2)|(ONE<<P3)|(ONE<<P0),P0,-1,P2,P3,decl_score,opp_score,1);
	}
      }
      else if (playerpos == 2) {
	if (playtsize % PLAYER == 0) {
	  pts = AOS->solve(g,h1,h2,myhand,h3,played,-1,-1,-1,-1,decl_score,opp_score,2);	    
	}
	else if (playtsize % PLAYER == 1) {
	  int P1 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,h2,myhand,h3,played|(ONE<<P1),-1,P1,-1,-1,decl_score,opp_score,2);
	}
	else if (playtsize % PLAYER == 2) {
	  int P0 = playtricks[playtsize-2], P1 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,h2,myhand,h3,played|(ONE<<P0)|(ONE<<P1),P0,P1,-1,-1,decl_score,opp_score,2);
	}
	else if (playtsize % PLAYER == 3) {
	  int P3 = playtricks[playtsize-3], P0 = playtricks[playtsize-2], P1 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,h2,myhand,h3,played|(ONE<<P0)|(ONE<<P1)|(ONE<<P3),P0,P1,-1,P3,decl_score,opp_score,2);
	}
      }
      else if (playerpos == 3) {
	if (playtsize % PLAYER == 0) {
	  pts = AOS->solve(g,h1,h2,h3,myhand,played,-1,-1,-1,-1,decl_score,opp_score,3);	    
	}
	else if (playtsize % PLAYER == 1) {
	  int P2 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,h2,h3,myhand,played|(ONE<<P2),-1,-1,P2,-1,decl_score,opp_score,3);	    
	}
	else if (playtsize % PLAYER == 2) {
	  int P1 = playtricks[playtsize-2], P2 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,h2,h3,myhand,played|(ONE<<P2)|(ONE<<P1),-1,P1,P2,-1,decl_score,opp_score,3);
	}
	else if (playtsize % PLAYER == 3) {
	  int P0 = playtricks[playtsize-3], P1 = playtricks[playtsize-2], P2 = playtricks[playtsize-1];
	  pts = AOS->solve(g,h1,h2,h3,myhand,played|(ONE<<P0)|(ONE<<P1)|(ONE<<P2),P0,P1,P2,-1,decl_score,opp_score,3);
	}
      }
      int index = AOS->proposed_card;
      if (index != -1) {
	if (index > DECKSIZE || index < 0) { cout << "ERROR wrong index" << endl; exit(1); } 
	suggested_cards[index] += pts; 
	results[worlds++] = (pts > g.limit);
	if (pts > g.limit) {
	  suggested_cards[index] += 10;
	  winning_cards[index]++;
	}
      }
    }
    return;
  }
  int firstcard = CTZ(rest);
  card bitcard = ONE<<firstcard;
  if (count(h1) < sampleh1 && !(bitcard & not1)) 
    generate_recursive1(rest & ~bitcard, h1 | bitcard, h2, h3, not1, not2, not3, turn);
  if (count(h3) < sampleh3 && !(bitcard & not3)) 
    generate_recursive1(rest & ~bitcard, h1, h2, h3 | bitcard, not1, not2, not3, turn);
}

void KnowledgeBase::count_recursive1(card rest, card h1, card h2, card h3, card not1, card not2, card not3, int turn) {
  if (count(rest) == 0) {
    //    cout << "!!! <" << count(h1) << "," << count(h2) << "," << count(h3) << ";" << count(not1) << "," << count(not2) << "," << count(not3) << ">";    countrek = countrek + ONE;
    countrek++;
    return;
  }
  //  cout << "<" << count(h1) << "," << count(h2) << "," << count(h3) << ";" << count(not1) << "," << count(not2) << "," << count(not3) << ">";    countrek = countrek + ONE;
  int firstcard = CTZ(rest);
  card bitcard = ONE<<firstcard;
  if (count(h1) < sampleh1 && !(bitcard & not1)) 
    count_recursive1(rest & ~bitcard, h1 | bitcard, h2, h3, not1, not2, not3, turn);
  if (count(h3) < sampleh3 && !(bitcard & not3)) 
    count_recursive1(rest & ~bitcard, h1, h2, h3 | bitcard, not1, not2, not3, turn);
}

void KnowledgeBase::generate_winning_recursive1(card rest, card h1, card h2, card h3, card not1, card not2, card not3, int turn, card testcard) {
  if (count(rest) == 0) {
    int pts = -1;
    if (turn % 2 == 1) { // only for opponents     
      if (playerpos == 0) {
	if (playtsize % PLAYER == 0) {
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,myhand,h1,h2,h3,played,-1,-1,-1,-1,decl_score,opp_score,0,ind);
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 1) {
	  int P3 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,myhand,h1,h2,h3,played|(ONE<<P3),-1,-1,-1,P3,decl_score,opp_score,0,ind);
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 2) {
	  int P2 = playtricks[playtsize-2], P3 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,myhand,h1,h2,h3,played|(ONE<<P2)|(ONE<<P3),-1,-1,P2,P3,decl_score,opp_score,0,ind);
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 3) {
	  int P1 = playtricks[playtsize-3], P2 = playtricks[playtsize-2], P3 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,myhand,h1,h2,h3,played|(ONE<<P1)|(ONE<<P2)|(ONE<<P3),-1,P1,P2,P3,decl_score,opp_score,0,ind);
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
      }
      else if (playerpos == 1) {
	if (playtsize % PLAYER == 0) {
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,myhand,h2,h3,played,-1,-1,-1,-1,decl_score,opp_score,1,ind);
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 1) {
	  int P0 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,myhand,h2,h3,played|(ONE<<P0),P0,-1,-1,-1,decl_score,opp_score,1,ind);
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 2) {
	  int P3 = playtricks[playtsize-2], P0 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,myhand,h2,h3,played|(ONE<<P0)|(ONE<<P3),P0,-1,-1,P3,decl_score,opp_score,1,ind);
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 3) {
	  int P2 = playtricks[playtsize-3], P3 = playtricks[playtsize-2], P0 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,myhand,h2,h3,played|(ONE<<P2)|(ONE<<P3)|(ONE<<P0),P0,-1,P2,P3,decl_score,opp_score,1,ind);
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
      }
      else if (playerpos == 2) {
	if (playtsize % PLAYER == 0) {
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,myhand,h3,played,-1,-1,-1,-1,decl_score,opp_score,2,ind);
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 1) {
	  int P1 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,myhand,h3,played|(ONE<<P1),-1,P1,-1,-1,decl_score,opp_score,2,ind);
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 2) {
	  int P0 = playtricks[playtsize-2], P1 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,myhand,h3,played|(ONE<<P0)|(ONE<<P1),P0,P1,-1,-1,decl_score,opp_score,2,ind);
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 3) {
	  int P3 = playtricks[playtsize-3], P0 = playtricks[playtsize-2], P1 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,myhand,h3,played|(ONE<<P0)|(ONE<<P1)|(ONE<<P3),P0,P1,-1,P3,decl_score,opp_score,2,ind);
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
      }
      else if (playerpos == 3) {
	if (playtsize % PLAYER == 0) {
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,h3,myhand,played,-1,-1,-1,-1,decl_score,opp_score,3,ind);
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 1) {
	  int P2 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,h3,myhand,played|(ONE<<P2),-1,-1,P2,-1,decl_score,opp_score,3,ind);  
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 2) {
	  int P1 = playtricks[playtsize-2], P2 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,h3,myhand,played|(ONE<<P2)|(ONE<<P1),-1,P1,P2,-1,decl_score,opp_score,3,ind);
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 3) {
	  int P0 = playtricks[playtsize-3], P1 = playtricks[playtsize-2], P2 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,h3,myhand,played|(ONE<<P0)|(ONE<<P1)|(ONE<<P2),P0,P1,P2,-1,decl_score,opp_score,3,ind);
	    counted_wins[ind] += 1-pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
      }
    } 
    else { // only for declarer party
      if (playerpos == 0) {
	if (playtsize % PLAYER == 0) {
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,myhand,h1,h2,h3,played,-1,-1,-1,-1,decl_score,opp_score,0,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 1) {
	  int P3 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,myhand,h1,h2,h3,played|(ONE<<P3),-1,-1,-1,P3,decl_score,opp_score,0,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 2) {
	  int P2 = playtricks[playtsize-2], P3 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,myhand,h1,h2,h3,played|(ONE<<P2)|(ONE<<P3),-1,-1,P2,P3,decl_score,opp_score,0,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 3) {
	  int P1 = playtricks[playtsize-3], P2 = playtricks[playtsize-2], P3 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,myhand,h1,h2,h3,played|(ONE<<P1)|(ONE<<P2)|(ONE<<P3),-1,P1,P2,P3,decl_score,opp_score,0,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
      }
      else if (playerpos == 1) {
	if (playtsize % PLAYER == 0) {
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,myhand,h2,h3,played,-1,-1,-1,-1,decl_score,opp_score,1,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 1) {
	  int P0 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,myhand,h2,h3,played|(ONE<<P0),P0,-1,-1,-1,decl_score,opp_score,1,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 2) {
	  int P3 = playtricks[playtsize-2], P0 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,myhand,h2,h3,played|(ONE<<P0)|(ONE<<P3),P0,-1,-1,P3,decl_score,opp_score,1,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 3) {
	  int P2 = playtricks[playtsize-3], P3 = playtricks[playtsize-2], P0 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,myhand,h2,h3,played|(ONE<<P2)|(ONE<<P3)|(ONE<<P0),P0,-1,P2,P3,decl_score,opp_score,1,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
      }
      else if (playerpos == 2) {
	if (playtsize % PLAYER == 0) {
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,myhand,h3,played,-1,-1,-1,-1,decl_score,opp_score,2,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 1) {
	  int P1 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,myhand,h3,played|(ONE<<P1),-1,P1,-1,-1,decl_score,opp_score,2,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 2) {
	  int P0 = playtricks[playtsize-2], P1 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,myhand,h3,played|(ONE<<P0)|(ONE<<P1),P0,P1,-1,-1,decl_score,opp_score,2,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 3) {
	  int P3 = playtricks[playtsize-3], P0 = playtricks[playtsize-2], P1 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,myhand,h3,played|(ONE<<P0)|(ONE<<P1)|(ONE<<P3),P0,P1,-1,P3,decl_score,opp_score,2,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
      }
      else if (playerpos == 3) {
	if (playtsize % PLAYER == 0) {
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,h3,myhand,played,-1,-1,-1,-1,decl_score,opp_score,3,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 1) {
	  int P2 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,h3,myhand,played|(ONE<<P2),-1,-1,P2,-1,decl_score,opp_score,3,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 2) {
	  int P1 = playtricks[playtsize-2], P2 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,h3,myhand,played|(ONE<<P2)|(ONE<<P1),-1,P1,P2,-1,decl_score,opp_score,3,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
	else if (playtsize % PLAYER == 3) {
	  int P0 = playtricks[playtsize-3], P1 = playtricks[playtsize-2], P2 = playtricks[playtsize-1];
	  while (testcard) {
	    int ind = CTZ(testcard);
	    pts = AOS->execute(g,h1,h2,h3,myhand,played|(ONE<<P0)|(ONE<<P1)|(ONE<<P2),P0,P1,P2,-1,decl_score,opp_score,3,ind);
	    counted_wins[ind] += pts;
	    testcard &= ~(ONE<<ind);
	  }
	}
      }
    }
    return;
  }
  int firstcard = CTZ(rest);
  card bitcard = ONE<<firstcard;
  if (count(h1) < sampleh1 && !(bitcard & not1)) 
    generate_winning_recursive1(rest & ~bitcard, h1 | bitcard, h2, h3, not1, not2, not3, turn, testcard);
  if (count(h3) < sampleh3 && !(bitcard & not3)) 
    generate_winning_recursive1(rest & ~bitcard, h1, h2, h3 | bitcard, not1, not2, not3, turn, testcard);
}


void KnowledgeBase::generate_recursive(card rem, card rest, card h1, card h2, card h3, card not1, card not2, card not3, int turn) {
  if (count(rest) == 0) {
    /*
        cout << "[" << count(rem) << "," << count(rest) << ":" << count(h1) << "," << count(h2) << "," << count(h3) << ";" << count(not1) << "," << count(not2) << "," << count(not3) << "]" << endl;;
    g.print_vector(rem,trumpmask); cout << "|";
    g.print_vector(h2,trumpmask);cout << "|";
    cout << "(" << turn << ")";
    cout << endl;
    */
    generate_recursive1(rem, h1, h2, h3, not1, not2, not3, turn); // determines best
    return;
  }
  int firstcard = CTZ(rest);
  card bitcard = ONE<<firstcard;
  if ((count(h2)<sampleh2) && !(bitcard & not2)) 
    generate_recursive(rem, rest & ~bitcard, h1, h2 | bitcard, h3, not1, not2, not3, turn);
  if (count(rem) < sampleh1 + sampleh3) 
    generate_recursive(rem | bitcard,rest & ~bitcard, h1, h2, h3, not1, not2, not3, turn);
}

void KnowledgeBase::generate_worlds_recursive(card rem, card rest, card h1, card h2, card h3, card not1, card not2, card not3, int turn) {
  if (count(rest) == 0) {
    generate_worlds_recursive1(rem, h1, h2, h3, not1, not2, not3, turn);
    return;
  }
  int firstcard = CTZ(rest);
  card bitcard = ONE<<firstcard;
  if ((count(h2) < sampleh2) && !(bitcard & not2)) 
    generate_worlds_recursive(rem, rest & ~bitcard, h1, h2 | bitcard, h3, not1, not2, not3, turn);
  if (count(rem) < sampleh1 + sampleh3) 
    generate_worlds_recursive(rem | bitcard,rest & ~bitcard, h1, h2, h3, not1, not2, not3, turn);
}

void KnowledgeBase::generate_winning_recursive(card rem, card rest, card h1, card h2, card h3, card not1, card not2, card not3, int turn, card testcard) {
  if (count(rest) == 0) {
    generate_winning_recursive1(rem, h1, h2, h3, not1, not2, not3, turn, testcard);
    return;
  }
  int firstcard = CTZ(rest);
  card bitcard = ONE<<firstcard;
  if ((count(h2) < sampleh2) && !(bitcard & not2)) 
    generate_winning_recursive(rem, rest & ~bitcard, h1, h2 | bitcard, h3, not1, not2, not3, turn, testcard);
  if (count(rem) < sampleh1 + sampleh3) 
    generate_winning_recursive(rem | bitcard,rest & ~bitcard, h1, h2, h3, not1, not2, not3, turn, testcard);
}

void KnowledgeBase::count_recursive(card rem, card rest, card h1, card h2, card h3, card not1, card not2, card not3, int turn){
  if (count(rest) == 0) {
    count_recursive1(rem, h1, h2, h3, not1, not2, not3, turn);
    return;
  }
  int firstcard = CTZ(rest);
  card bitcard = ONE<<firstcard;
  if ((count(h2)<sampleh2) && !(bitcard & not2)) 
    count_recursive(rem, rest & ~bitcard, h1, h2 | bitcard, h3, not1, not2, not3, turn);
  if (count(rem) < sampleh1 + sampleh3) 
    count_recursive(rem | bitcard,rest & ~bitcard, h1, h2, h3, not1, not2, not3, turn);
}

 
long long KnowledgeBase::count_hands(card allopponent, card notinhand1, card notinhand2, card notinhand3, int turn) {
  card h1=0, h2=0, h3=0;
  countrek = 0;

  /*
  cout << "remaining cards " << count(allopponent) << endl;
  cout << "remaining cards " << count(allopponent) << endl;
  
  g.print_vector(allopponent,trumpmask); cout << "|";
  g.print_vector(notinhand1,trumpmask);cout << "|";
  g.print_vector(notinhand2,trumpmask);cout << "|";
  g.print_vector(notinhand3,trumpmask);cout << "|" << "(" << turn << ")";
  cout << "[" << sampleh1 << "," << sampleh2 << "," << sampleh3 << "]";
  cout << endl;
  */
  count_recursive(0,allopponent,h1,h2,h3,notinhand1,notinhand2,notinhand3,turn);
  //  cout << "result of counting " << endl;
  return countrek;
}


void KnowledgeBase::generate_hands(card allopponent, card notinhand1, card notinhand2, card notinhand3, int turn) {
  card h1=0, h2=0, h3=0;
  worlds = 0;
  /*
  cout << " generate hands ";
  g.print_vector(allopponent,trumpmask);  cout << " h1 " << endl;
  g.print_vector(h1,trumpmask);  cout << " h2 " << endl;
  g.print_vector(h2,trumpmask);  cout << " h3 " << endl;
  g.print_vector(h3,trumpmask);  cout << " !h1 " << endl;
  g.print_vector(notinhand1,trumpmask);  cout << " !h2 " << endl;
  g.print_vector(notinhand2,trumpmask);  cout << " !h3 " << endl;
  g.print_vector(notinhand3,trumpmask); cout << "|" << "(" << turn << ")";
    cout << "[" << sampleh1 << "," << sampleh2 << "," << sampleh3 << "]";
  cout << endl;
  cout << endl;
  */
  generate_recursive(0,allopponent,h1,h2,h3,notinhand1,notinhand2,notinhand3,turn);
}

void KnowledgeBase::generate_winning_hands(card allopponent, card notinhand1, card notinhand2, card notinhand3, int turn, card testcard) {
  card h1 = 0, h2 = 0, h3 = 0;
  generate_winning_recursive(0,allopponent,h1,h2,h3,notinhand1,notinhand2,notinhand3,turn,testcard);
}



long long KnowledgeBase::countBelief() {
  //  cout << "counting belief P" << playerpos << "; knowledge vectors (all,notfirst,notsecond,notthird): " << endl;
  long long countrekursive = 0;
  for(int i=0;i<PLAYER;i++)
    partial[i] = 0;
  for (int i=0;i<MAXTRICKS;i++) {
    for(int j=0;j<PLAYER;j++)
      if (playtable[j][i] != -1) partial[j] |= (ONE<<playtable[j][i]);
  }
  //  cout << "[" << count(partial[0]) << "," << count(partial[1]) << "," << count(partial[2]) << "," << count(partial[3]) << "]";  

  //  cout << "[" << count(allopponent[0]) << "," << count(allopponent[1]) << "," << count(allopponent[2]) << "," << count(allopponent[3]) << "]";  
  allopponent[0] &= ~g.skat;
  allopponent[1] &= ~g.skat;
  allopponent[2] &= ~g.skat;
  allopponent[3] &= ~g.skat;
  cout << "[" << count(allopponent[0]) << "," << count(allopponent[1]) << "," << count(allopponent[2]) << "," << count(allopponent[3]) << "]";
  
  if (playerpos == 0) {
    sampleh1 = MAXTRICKS - count(partial[1]); sampleh2 = MAXTRICKS - count(partial[2]); sampleh3 = MAXTRICKS - count(partial[3]);
    countrekursive = count_hands(allopponent[0],nothaving[1][0],nothaving[2][0],nothaving[3][0],0);
  }
  if (playerpos == 1) {
    sampleh1 = MAXTRICKS - count(partial[0]); sampleh2 = MAXTRICKS - count(partial[2]); sampleh3 = MAXTRICKS - count(partial[3]);
    countrekursive = count_hands(allopponent[1],nothaving[0][1],nothaving[2][1],nothaving[3][1],1);
  }
  if (playerpos == 2) {
    sampleh1 = MAXTRICKS - count(partial[0]); sampleh2 = MAXTRICKS - count(partial[1]); sampleh3 = MAXTRICKS - count(partial[3]);
    countrekursive = count_hands(allopponent[2],nothaving[0][2],nothaving[1][2],nothaving[3][2],2);
  }
  if (playerpos == 3) {
    sampleh1 = MAXTRICKS - count(partial[0]); sampleh2 = MAXTRICKS - count(partial[1]); sampleh3 = MAXTRICKS - count(partial[2]);
    countrekursive = count_hands(allopponent[3],nothaving[0][3],nothaving[1][3],nothaving[2][3],3);
  }  
  //  cout << " " << countrekursive << " possible worlds " << endl;
  //  if (countrekursive == 0) cout << "WARNING " << endl;
  return countrekursive;   
}

void KnowledgeBase::updateBelief(int trickpos, int trickCard1, int trickCard2) {
  int issuer = trickpos;
  #ifdef SHOWBELIEF
  cout << " update belief, player " << playerpos << " issuer " << trickpos << " table " << card_name[trickCard1] << " " << card_name [trickCard2] << " " << endl;
  #endif
  card hand = myhand;
  // obeyer 1, issuer 0
  if (trickpos == 0) { // P0 issues trick
    int P0 = trickCard1, P1 = trickCard2;
    if (trumpmask & (ONE<<P0)) { // trump issued
      if (~trumpmask & (ONE<<P1)) DropTrump(playerpos,1,0,P1,P0);      
      else ObeyTrump(playerpos,1,0,P1,P0);
    }
    else {
      int suit = g.Suit[P0];
      card suitmask = g.Mask[suit];
      if (~suitmask & (ONE<<P1)) DropSuit(playerpos,1,0,P1,P0);
      else ObeySuit(playerpos,1,0,P1,P0);
    }
  }
  // obeyer 2, issuer 1
  if (trickpos == 1) { // P1 issues trick
    int P1 = trickCard1, P2 = trickCard2;      
    if (trumpmask & (ONE<<P1)) {
      if (~trumpmask & (ONE<<P2)) DropTrump(playerpos,2,1,P2,P1);
      else ObeyTrump(playerpos,2,1,P2,P1);
    }
    else {
      int suit = g.Suit[P1];
      card suitmask = g.Mask[suit];
      if (~suitmask & (ONE<<P2)) DropSuit(playerpos,2,1,P2,P1);
      else ObeySuit(playerpos,2,1,P2,P1);
    }
  }
  // obeyer 3, issuer 2,
  if (trickpos == 2) { // P2 issues trick
    int P3 = trickCard2, P2 = trickCard1;
    if (trumpmask & (ONE<<P2)) {
      if (~trumpmask & (ONE<<P3)) DropTrump(playerpos,3,2,P3,P2);
      else ObeyTrump(playerpos,3,2,P3,P2);
    }
    else {
      int suit = g.Suit[P3];
      card suitmask = g.Mask[suit];
      if (~suitmask & (ONE<<P3)) DropSuit(playerpos,3,2,P3,P2);
      else ObeySuit(playerpos,3,2,P3,P2);
    }
  }
  // obeyer 0, issuer 3,
  if (trickpos == 3) { // P3 issues trick
    int P0 = trickCard2, P3 = trickCard1;
    if (trumpmask & (ONE<<P3)) {
      if (~trumpmask & (ONE<<P0)) DropTrump(playerpos,0,3,P0,P3);
      else ObeyTrump(playerpos,0,3,P0,P3);
    }
    else {
      int suit = g.Suit[P3];
      card suitmask = g.Mask[suit];
      if (~suitmask & (ONE<<P0)) DropSuit(playerpos,0,3,P0,P3);
      else ObeySuit(playerpos,0,3,P0,P3);
    }
  }

  allopponent[playerpos] &= ~played;
  nothaving[0][playerpos] &= ~played;
  nothaving[1][playerpos] &= ~played;
  nothaving[2][playerpos] &= ~played;
  nothaving[3][playerpos] &= ~played;
}

void KnowledgeBase::updateBelief(int trickpos, int trickCard1, int trickCard2, int trickCard3) {
  int issuer = trickpos;
#ifdef SHOWBELIEF
  cout << " update belief, player " << playerpos << " issuer " << trickpos << " table " << card_name[trickCard1] << " " << card_name[trickCard2] << " " << card_name[trickCard3] << endl;
#endif
  card hand = myhand;
  // obeyer 2, issuer 0, inter1 1, 
  if (trickpos == 0) { // AS issues trick
    int P0 = trickCard1, P1 = trickCard2, P2 = trickCard3;
    if (trumpmask & (ONE<<P0)) { // trump played
	if (~trumpmask & (ONE<<P2)) DropTrump(playerpos,2,0,1,P2,P0,P1);
	else ObeyTrump(playerpos,2,0,1,P2,P0,P1);
    }
    else {
      int suit = g.Suit[P0];
      card suitmask = g.Mask[suit];
      if (~suitmask & (ONE<<P2)) DropSuit(playerpos,2,0,1,P2,P0,P1);
      else ObeySuit(playerpos,2,0,1,P2,P0,P1);
    }
  }
  // obeyer 3, issuer 1, inter1 2, 
  if (trickpos == 1) { // P1 issues trick
    int P1 = trickCard1, P2 = trickCard2, P3 = trickCard3;
    if (trumpmask & (ONE<<P1)) {
      if (~trumpmask & (ONE<<P3)) DropTrump(playerpos,3,1,2,P3,P1,P2);
      else ObeyTrump(playerpos,3,1,2,P3,P1,P2);
    }
    else {
      int suit = g.Suit[P1];
      card suitmask = g.Mask[suit];
      if (~suitmask & (ONE<<P3)) DropSuit(playerpos,3,1,2,P3,P1,P2);
      else ObeySuit(playerpos,3,1,2,P3,P1,P2);
    }
  }
  // obeyer 0, issuer 2, inter1 3, 
  if (trickpos == 2) { // P2 issues trick
    int P2 = trickCard1, P3 = trickCard2, P0 = trickCard3;      
    if (trumpmask & (ONE<<P2)) {
      if (~trumpmask & (ONE<<P0)) DropTrump(playerpos,0,2,3,P0,P2,P3);
      else ObeyTrump(playerpos,0,2,3,P0,P2,P3);
    }
    else {
      int suit = g.Suit[P2];
      card suitmask = g.Mask[suit];
      if (~suitmask & (ONE<<P0)) DropSuit(playerpos,0,2,3,P0,P2,P3);
      else ObeySuit(playerpos,0,2,3,P0,P2,P3);
    }
  }
  // obeyer 1, issuer 3, inter1 0, 
  if (trickpos == 3) { // P3 issues trick
    int P3 = trickCard1, P0 = trickCard2, P1 = trickCard1;      
    if (trumpmask & (ONE<<P3)) {
      if (~trumpmask & (ONE<<P1)) DropTrump(playerpos,1,3,0,P1,P3,P0);
      else ObeyTrump(playerpos,1,3,0,P1,P3,P0);
    }
    else {
      int suit = g.Suit[P3];
      card suitmask = g.Mask[suit];
      if (~suitmask & (ONE<<P1)) DropSuit(playerpos,1,3,0,P1,P3,P0);
      else ObeySuit(playerpos,1,3,0,P1,P3,P0);
    }
  }

  allopponent[playerpos] &= ~played;
  nothaving[0][playerpos] &= ~played;
  nothaving[1][playerpos] &= ~played;
  nothaving[2][playerpos] &= ~played;
  nothaving[3][playerpos] &= ~played;
}

void KnowledgeBase::updateBelief(int trickpos, int trickCard1, int trickCard2, int trickCard3, int trickCard4) {
  int issuer = trickpos;
#ifdef SHOWBELIEF
  cout << " update belief, player " << playerpos << " issuer " << trickpos << " table " << card_name[trickCard1] << " " << card_name[trickCard2] << " " << card_name[trickCard3] << " " << card_name[trickCard4] << endl;
#endif

  card hand = myhand;
  // obeyer 3, issuer 0, inter1 1, inter2 2
  if (trickpos == 0) { // P0 issues trick
    int P0 = trickCard1, P1 = trickCard2, P2 = trickCard3, P3 = trickCard3;
    if (trumpmask & (ONE<<P0)) { // trump played
      if (~trumpmask & (ONE<<P3)) DropTrump(playerpos,3,0,1,2,P3,P0,P1,P2);
      else ObeyTrump(playerpos,3,0,1,2,P3,P0,P1,P2);
    }
    else {
      int suit = g.Suit[P0];
      card suitmask = g.Mask[suit];
      if (~suitmask & (ONE<<P3)) DropSuit(playerpos,3,0,1,2,P3,P0,P1,P2);
      else ObeySuit(playerpos,3,0,1,2,P3,P0,P1,P2);
    }
  }
  // obeyer 0, issuer 1, inter1 2, inter2 3
  if (trickpos == 1) { // P1 issues trick
    int P0 = trickCard4, P1 = trickCard1, P2 = trickCard2, P3 = trickCard3;
    if (trumpmask & (ONE<<P1)) {
      if (~trumpmask & (ONE<<P0)) DropTrump(playerpos,0,1,2,3,P0,P1,P2,P3);
      else ObeyTrump(playerpos,0,1,2,P0,P1,P2);
    }
    else {
      int suit = g.Suit[P1];
      card suitmask = g.Mask[suit];
      if (~suitmask & (ONE<<P0)) DropSuit(playerpos,0,1,2,P0,P1,P2);
      else ObeySuit(playerpos,0,1,2,3,P0,P1,P2,P3);
      if (~suitmask & (ONE<<P0)) DropSuit(playerpos,0,1,2,P0,P1,P2);
      else ObeySuit(playerpos,0,1,2,3,P0,P1,P2,P3);
    }
  }
  // obeyer 1, issuer 2, inter1 3, inter2 0
  if (trickpos == 2) { // P2 issues trick
    int P0 = trickCard3, P1 = trickCard4, P2 = trickCard1, P3 = trickCard2;      
    if (trumpmask & (ONE<<P2)) {
      if (~trumpmask & (ONE<<P1)) DropTrump(playerpos,1,2,3,0,P1,P2,P3,P0);
      else ObeyTrump(playerpos,1,2,3,0,P1,P2,P3,P0);
    }
    else {
      int suit = g.Suit[P2];
      card suitmask = g.Mask[suit];
      if (~suitmask & (ONE<<P1)) DropSuit(playerpos,1,2,3,0,P1,P2,P3,P0);
      else ObeySuit(playerpos,1,2,0,P1,P2,P0);
    }
  }
  // obeyer 2, issuer 3, inter1 0, inter2 1
  if (trickpos == 3) { 
    int P0 = trickCard2, P1 = trickCard3, P2 = trickCard1, P3 = trickCard1;      
    if (trumpmask & (ONE<<P3)) {
      if (~trumpmask & (ONE<<P2)) DropTrump(playerpos,3,2,0,1,P3,P2,P0,P1);
      else ObeyTrump(playerpos,3,2,0,1,P3,P2,P0,P1);
    }
    else {
      int suit = g.Suit[P3];
      card suitmask = g.Mask[suit];
      if (~suitmask & (ONE<<P1)) DropSuit(playerpos,3,2,0,1,P3,P2,P0,P1);
      else ObeySuit(playerpos,3,2,0,P1,P2,P0);
    }
  }

  allopponent[playerpos] &= ~played;
  nothaving[0][playerpos] &= ~played;
  nothaving[1][playerpos] &= ~played;
  nothaving[2][playerpos] &= ~played;
  nothaving[3][playerpos] &= ~played;
}

void KnowledgeBase::updateBelief(int trickpos, int trickCard1) {
  int issuer = trickpos;
#ifdef SHOWBELIEF
  cout << " update belief, player " << playerpos << " issuer " << trickpos << " table " << card_name[trickCard1] << endl;
#endif
  card hand = myhand;
  
  allopponent[playerpos] &= ~played;
  nothaving[0][playerpos] &= ~played;
  nothaving[1][playerpos] &= ~played;
  nothaving[2][playerpos] &= ~played;
  nothaving[3][playerpos] &= ~played;
}
 
void KnowledgeBase::DropSuit(int player, int obeyer, int issuer, int missing, int ocard, int icard, int mcard) {
#ifdef SHOWSTEFAN
  cout << "P" << player << " sees card " << SHOW(ocard) << " in HH not obeying suit card " << SHOW(icard) << " intermediate " << SHOW(mcard) << endl;
#endif
  int suit = g.Suit[icard];
  card suitmask = g.Mask[suit] & ~trumpmask;
  card adding = 0;

  if (count(played) <= BSDEPTH + DIFFERENCE) { 
    if (trumpmask != 0 && issuer == 2 && g.win(mcard,ocard,trumpmask) == mcard && g.win(icard,mcard,trumpmask) == mcard) {
#ifdef SHOWSTEFAN
      cout << "P" << 0 <<  " leads, "; g.print_vector(g.lower_value(ocard,trumpmask),trumpmask);
#endif
      int osuit = g.Suit[ocard];
      card osuitmask = g.Mask[osuit];
      adding = g.lower_value(ocard,trumpmask) & osuitmask & ~trumpmask & allopponent[player] & ~played;
#ifdef SHOWSTEFAN
	cout << " removing "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
    }
  }
  adding |= suitmask & ~trumpmask & allopponent[player] & ~played;  
#ifdef SHOWREMOVING
  cout << " removing ds/3 "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
  nothaving[obeyer][player] |= adding;
}

void KnowledgeBase::ObeySuit(int player, int obeyer, int issuer, int missing, int ocard, int icard, int mcard) {
#ifdef SHOWSTEFAN
  cout << "P" << player << " sees card " << SHOW(ocard) << " in HH obeying suit card " << SHOW(icard) << " intermediate " << SHOW(mcard) << endl;
#endif
  
  card adding = 0;
  int suit = g.Suit[ocard];
  card suitmask = g.Mask[suit];

  if (count(played) <= BSDEPTH + DIFFERENCE) { // return; // dont assume obey rules later than endgame play
    //    cout << "checking " << issuer << leads(icard,ocard,trumpmask) << leads(icard,mcard,trumpmask) << endl;
    if (issuer == 0 && g.win(icard,ocard,trumpmask) == icard && g.win(icard,mcard,trumpmask) == icard) {
#ifdef SHOWSTEFAN
      cout << "P" << issuer <<  " leads, ";
#endif
      adding = g.lower_value(ocard,trumpmask) & suitmask & ~trumpmask & allopponent[player] & ~played;
#ifdef SHOWSTEFAN
      cout << " removing "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
    }
    if (issuer == 0 && g.win(icard,mcard,trumpmask) == mcard && g.win(mcard,ocard,trumpmask) == mcard) {
#ifdef SHOWSTEFAN
      cout << "P" << issuer <<  " leads, ";
#endif
      adding = g.higher_value(ocard,trumpmask) & suitmask & ~trumpmask & allopponent[player] & ~played;
#ifdef SHOWSTEFAN
      cout << " removing "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
    }
    if (issuer == 1 && g.win(icard,ocard,trumpmask) == icard && g.win(icard,mcard,trumpmask) == icard) {
#ifdef SHOWSTEFAN
      cout << "P" << issuer <<  " leads, ";
#endif
      adding = g.lower_value(ocard,trumpmask) & suitmask & ~trumpmask & allopponent[player] & ~played;
#ifdef SHOWSTEFAN
      cout << " removing "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
    }
    if (issuer == 1 && g.win(icard,mcard,trumpmask) == mcard && g.win(mcard,ocard,trumpmask) == mcard) {
#ifdef SHOWSTEFAN
      cout << "P" << missing <<  " leads, ";
#endif
      adding = g.lower_value(ocard,trumpmask) & suitmask & ~trumpmask & allopponent[player] & ~played;
#ifdef SHOWSTEFAN
      cout << " removing "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
    }
    if (issuer == 2 && g.win(icard,ocard,trumpmask) == icard && g.win(icard,mcard,trumpmask) == icard) {
#ifdef SHOWSTEFAN
     cout << "P" << issuer <<  " leads, ";
#endif
     adding = g.higher_value(ocard,trumpmask) & suitmask & ~trumpmask & allopponent[player] & ~played;
#ifdef SHOWSTEFAN
     cout << " removing "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
   }
    if (issuer == 2 && g.win(icard,mcard,trumpmask) == mcard && g.win(mcard,ocard,trumpmask) == mcard) {
#ifdef SHOWSTEFAN
     cout << "P" << missing <<  " leads, ";
#endif
     adding = g.lower_value(ocard,trumpmask) & suitmask & ~trumpmask & allopponent[player] & ~played;
#ifdef SHOWSTEFAN
     cout << " removing "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
   }
  }
#ifdef SHOWREMOVING
  cout << " removing os/3 "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
  nothaving[obeyer][player] |= adding;
}

void KnowledgeBase::DropTrump(int player, int obeyer, int issuer, int missing, int ocard, int icard, int mcard) {
#ifdef SHOWSTEFAN
  cout << "P" << player << " sees card " << SHOW(ocard) << " in HH not obeying trump card " << SHOW(icard) << " intermediate " << SHOW(mcard) << endl;
#endif
  card adding = 0;

  if (count(played) <= BSDEPTH + DIFFERENCE) {
    //    cout << card_name[leads(icard,ocard,trumpmask)] << card_name[leads(icard,mcard,trumpmask)] << endl;
    if (issuer == 0 && g.win(icard,ocard,trumpmask) == icard && g.win(icard,mcard,trumpmask) == icard) {
#ifdef SHOWSTEFAN
      cout << "P" << issuer <<  " leads, ";
#endif
      if ((ONE<<ocard)) {
	int osuit = g.Suit[ocard];
	card osuitmask = g.Mask[osuit];	
	adding = g.lower_value(ocard,trumpmask) & osuitmask & ~trumpmask & allopponent[player] & ~played;
#ifdef SHOWSTEFAN
	cout << " removing "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
      }
    }
    
    if (issuer == 2 && g.win(mcard,ocard,trumpmask) == mcard && g.win(icard,mcard,trumpmask) == mcard) {
#ifdef SHOWSTEFAN
      cout << "P" << 0 <<  " leads, ";
#endif
      if ((ONE<<ocard)) {
	int osuit = g.Suit[ocard];
	card osuitmask = g.Mask[osuit];
	adding = g.lower_value(ocard,trumpmask) & osuitmask & ~trumpmask & allopponent[player] & ~played;
#ifdef SHOWSTEFAN
	cout << " removing "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
      }
    }
  }
  adding |= trumpmask & allopponent[player] & ~played;

#ifdef SHOWREMOVING
  cout << " removing dt/3 "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
  nothaving[obeyer][player] |= adding;
}

void KnowledgeBase::ObeyTrump(int player, int obeyer, int issuer, int missing, int ocard, int icard, int mcard) {
#ifdef SHOWSTEFAN
  cout << "P" << player << " sees card " << SHOW(ocard) << " in HH obeying trump card " << SHOW(icard)  << " intermediate " << SHOW(mcard) << endl;
#endif
  card adding = 0;

  if (count(played) <= BSDEPTH + DIFFERENCE) {
    if (issuer == 0) {
      if (g.win(icard,ocard,trumpmask) == icard && g.win(icard,mcard,trumpmask) == icard) {
	#ifdef SHOWSTEFAN
	cout << "P" << issuer << " " << card_name[ocard] << " " << BSDEPTH + DIFFERENCE <<  " leads, ";
#endif
	
	//    if (player == 0) adding = g.lower_value(ocard,trumpmask) & trumpmask & allopponent[0];
	if (player == 1) adding |= g.lower_value(ocard,trumpmask) & trumpmask & allopponent[1] & ~played;
	if (player == 2) adding |= g.lower_value(ocard,trumpmask) & trumpmask & allopponent[2] & ~played;
	#ifdef SHOWSTEFAN
	cout << " removing "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
	#endif
      }
    }
    if (issuer == 2) {
      if (g.win(mcard,ocard,trumpmask) == mcard && g.win(icard,mcard,trumpmask) == mcard) {
#ifdef SHOWSTEFAN
	cout << "P" << issuer <<  " leads, ";
#endif
      //    if (player == 0) adding = lower_value(ocard,trumpmask) & trumpmask & allopponent[0];
	if (player == 1) adding |= g.lower_value(ocard,trumpmask) & trumpmask & allopponent[1];
	if (player == 2) adding |= g.lower_value(ocard,trumpmask) & trumpmask & allopponent[2];
#ifdef SHOWSTEFAN
      cout << " removing "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
      }
    }
  }
#ifdef SHOWREMOVING
  cout << " removing ot/3 "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
  nothaving[obeyer][player] |= adding;
}

void KnowledgeBase::DropSuit(int player, int obeyer, int issuer, int ocard, int icard) {
#ifdef SHOWSTEFAN
  cout << "P" << player << " sees card " << SHOW(ocard) << " in MH not obeying suit card " << SHOW(icard) << endl;
#endif
  int suit = g.Suit[icard];
  card suitmask = g.Mask[suit] & ~trumpmask;
  card adding = 0;
  if (trumpmask != 0 && g.win(icard,ocard,trumpmask) == icard && issuer == 0) { // AS issues and leads, I drop
    int osuit = g.Suit[ocard];
    card osuitmask = g.Mask[osuit];
#ifdef SHOWSTEFAN
    cout << "P" << issuer <<  " leads, wimmel no ace , ";
#endif
    adding = g.higher_value(ocard,trumpmask) & osuitmask & ~trumpmask & allopponent[player];
  }
  
  adding |= suitmask & ~trumpmask & allopponent[player];
 
#ifdef SHOWSTEFAN
  cout << " removing ds/2 "; g.print_vector(adding,trumpmask); cout << " from player " << obeyer << " of card " << card_name[ocard] << endl;
#endif
  nothaving[obeyer][player] |= adding;
}

void KnowledgeBase::ObeySuit(int player, int obeyer, int issuer, int ocard, int icard) {
#ifdef SHOWSTEFAN
  cout << "P" << player << " sees card " << SHOW(ocard) << " in MH obeying suit card " << SHOW(icard) << endl;
#endif
  card adding = 0;
  int suit = g.Suit[icard];
  card suitmask = g.Mask[suit];

  if (g.win(icard,ocard,trumpmask) == icard && issuer == 0) {
    if (((ONE<<ocard)) == 0) {
#ifdef SHOWSTEFAN
      cout << "P" << issuer <<  " leads, ";
#endif
      adding = g.lower_value(ocard,trumpmask) & suitmask & ~trumpmask & allopponent[player];
    }
  }
#ifdef SHOWREMOVING
  cout << " removing os/2 "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
  nothaving[obeyer][player] |= adding;
}

void KnowledgeBase::DropTrump(int player, int obeyer, int issuer, int ocard, int icard) {
#ifdef SHOWSTEFAN
  cout << "P" << player << " sees card " << card_name[ocard] << " in MH not obeying trump card " << card_name[icard] << endl;
#endif
  card adding = 0;
  if (g.win(icard,ocard,trumpmask) == icard && issuer == 0) {
    int suit = g.Suit[ocard];
    card suitmask = g.Mask[suit];
    adding = g.higher_value(ocard,trumpmask) & suitmask & ~trumpmask & allopponent[player];
  }
  adding |= trumpmask & allopponent[player];

#ifdef SHOWREMOVING
  cout << " removing dt/2 "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
  nothaving[obeyer][player] |= adding;
}

void KnowledgeBase::ObeyTrump(int player, int obeyer, int issuer, int ocard, int icard) {
#ifdef SHOWSTEFAN
  cout << "P" << player << " sees card " << SHOW(ocard) << " in MH obeying trump card " << SHOW(icard) << endl;
#endif
  card adding = 0;
  if (issuer == 0) {
    if (g.win(icard,ocard,trumpmask) == icard) {
#ifdef SHOWSTEFAN
      cout << "P" << issuer <<  " leads, ";
#endif
      adding = g.lower_value(ocard,trumpmask) & trumpmask & allopponent[player];
#ifdef SHOWREMOVING
      cout << " removing "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
    }
#ifdef SHOWREMOVING
    cout << " removing ot/2 "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
    nothaving[obeyer][player] |= adding;
  }
}

void KnowledgeBase::DropTrump(int player, int obeyer, int issuer, int missing1, int missing2, int ocard, int icard, int m1card, int m2card) {
#ifdef SHOWSTEFAN
  cout << "P" << player << " sees card " << SHOW(ocard) << " in HH not obeying trump card " << SHOW(icard) << endl;
#endif
  card adding = 0;
  adding |= trumpmask & allopponent[player];
#ifdef SHOWREMOVING
  cout << " removing dt/4 "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
  nothaving[obeyer][player] |= adding;
}

void KnowledgeBase::ObeyTrump(int player, int obeyer, int issuer, int missing1, int missing2, int ocard, int icard, int m1card, int m2card) {
#ifdef SHOWSTEFAN
  cout << "P" << player << " sees card " << SHOW(ocard) << " in HH obeying trump card " << SHOW(icard) << endl;
#endif
  card adding = 0;
#ifdef SHOWREMOVING
  cout << " removing ot/4 "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
  nothaving[obeyer][player] |= adding;
}

void KnowledgeBase::DropSuit(int player, int obeyer, int issuer, int missing1, int missing2, int ocard, int icard, int m1card, int m2card) {
#ifdef SHOWSTEFAN
  cout << "P" << player << " sees card " << SHOW(ocard) << " in HH not obeying suit card " << SHOW(icard) << endl;
#endif
  card adding = 0;
  int suit = g.Suit[icard];
  card suitmask = g.Mask[suit] & ~trumpmask;
#ifdef SHOWREMOVING
  cout << " removing ds/4 "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
  adding |= suitmask & ~trumpmask & allopponent[player];
  nothaving[obeyer][player] |= adding;
}

void KnowledgeBase::ObeySuit(int player, int obeyer, int issuer, int missing1, int missing2, int ocard, int icard, int m1card, int m2card) {
#ifdef SHOWSTEFAN
  cout << "P" << player << " sees card " << SHOW(ocard) << " in HH obeying suit card " << SHOW(icard) << endl;
#endif
   card adding = 0;
#ifdef SHOWREMOVING
  cout << " removing os/4 "; g.print_vector(adding,trumpmask); cout << " from P" << obeyer << endl;
#endif
  nothaving[obeyer][player] |= adding;
}


void KnowledgeBase::sizes(int issuer, int cardno, int ctn[3]) {
   if (issuer == 0) ctn[0] = (PLAYSIZE-cardno)/4;
   if (issuer == 0) ctn[1] = (PLAYSIZE-cardno+1)/4;
   if (issuer == 0) ctn[2] = (PLAYSIZE-cardno+2)/4;
   if (issuer == 0) ctn[3] = (PLAYSIZE-cardno+3)/4;
   
   if (issuer == 1) ctn[1] = (PLAYSIZE-cardno)/4;
   if (issuer == 1) ctn[2] = (PLAYSIZE-cardno+1)/4;
   if (issuer == 1) ctn[3] = (PLAYSIZE-cardno+2)/4;
   if (issuer == 1) ctn[0] = (PLAYSIZE-cardno+3)/4;

   if (issuer == 2) ctn[2] = (PLAYSIZE-cardno)/4;
   if (issuer == 2) ctn[3] = (PLAYSIZE-cardno+1)/4;
   if (issuer == 2) ctn[0] = (PLAYSIZE-cardno+2)/4;
   if (issuer == 2) ctn[1] = (PLAYSIZE-cardno+2)/4;

   if (issuer == 2) ctn[3] = (PLAYSIZE-cardno)/4;
   if (issuer == 2) ctn[0] = (PLAYSIZE-cardno+1)/4;
   if (issuer == 2) ctn[1] = (PLAYSIZE-cardno+2)/4;
   if (issuer == 2) ctn[2] = (PLAYSIZE-cardno+2)/4;

}


void KnowledgeBase::convert() { // requires played to determine the size of the hands
  card hands[PLAYER];
  card origtriplehand, triplehand;
  card hand = myhand;
  card msorskat = 0, asorskat = 0;
  int player = playerpos;
  int cardsfixed[PLAYER];
  int issuer, cnt[PLAYER];

  for (int i=0;i<PLAYER;i++) cardsfixed[0] = 37;

  for (int j=0;j<PLAYER;j++) {
    for (int i=0;i<MAXTRICKS;i++) p[j][i] = 37;
  }

  for (int i=0;i<PLAYER;i++) knowhand[i] = 0;

  for (int i=0;i<PLAYER;i++)
    if (player == i)
      issuer = (count(played)+(PLAYER-i)) % PLAYER;
  
  sizes(issuer,count(played),cnt);

  for (int j=0;j<PLAYER;j++) {
    for (int i=0;i<DECKSIZE;i++) {
      belief[j][i] = 0;
    }
  }
  
  if (player == 0) {
    origtriplehand = triplehand = allopponent[0];
    hands[0] = hand & ~played;
    hands[1] = nothaving[2][0] & nothaving[3][0] & ~played & ~hands[0];
    hands[2] = nothaving[1][0] & nothaving[3][0] & ~played & ~hands[0];
    hands[3] = nothaving[2][0] & nothaving[1][0] & ~played & ~hands[0];
    asorskat = nothaving[2][0] & ~hands[0] & ~hands[1] & ~played;
    msorskat = nothaving[3][0] & ~hands[2] & ~hands[1] & ~played;
    triplehand &= ~(hands[0] | hands[2] | hands[1] | asorskat | msorskat) & ~played;
  }
  if (player == 1) {
    origtriplehand = triplehand = allopponent[1];
    hands[1] = hand & ~played;
    hands[0] = nothaving[2][1] & nothaving[3][1] & ~played & ~hands[1];
    hands[2] = nothaving[0][1] & nothaving[3][1] & ~played & ~hands[1];
    asorskat = nothaving[2][1] & ~hands[0] & ~hands[1] & ~played;
    msorskat = nothaving[0][1] & ~hands[2] & ~hands[1] & ~played;
    triplehand &= ~(hands[0] | hands[2] | hands[1] | asorskat | msorskat) & ~played;
  }   
  if (player == 2) {
    origtriplehand = triplehand = allopponent[2];
    hands[2] = hand & ~played;
    hands[0] = nothaving[1][2] & nothaving[3][2] & ~played & ~hands[2]; 
    hands[1] = nothaving[0][2] & nothaving[3][2] & ~played & ~hands[2];
    asorskat = nothaving[1][2] & ~hands[0] & ~hands[2] & ~played;
    msorskat = nothaving[0][2] & ~hands[1] & ~hands[2] & ~played;
    triplehand &= ~(hands[0] | hands[1] | hands[2] | asorskat | msorskat) & ~played;
  }
  if (player == 3) {
    origtriplehand = triplehand = allopponent[3];
    hands[2] = hand & ~played;
    hands[0] = nothaving[1][3] & nothaving[2][3] & ~played & ~hands[2]; 
    hands[1] = nothaving[0][3] & nothaving[1][3] & ~played & ~hands[2];
    asorskat = nothaving[1][3] & ~hands[0] & ~hands[2] & ~played;
    msorskat = nothaving[0][3] & ~hands[1] & ~hands[2] & ~played;
    triplehand &= ~(hands[0] | hands[1] | hands[2] | asorskat | msorskat) & ~played;
  }  
  if (player) {
    /*
    cout << "-----------------" << player << " sees ----------------"<< endl;
    cout << ";H"; g.print_vector(hand,trumpmask);
      cout << ";"; g.print_vector(hands[0],trumpmask);
      cout << ";"; g.print_vector(hands[1],trumpmask);
      cout << ";"; g.print_vector(hands[2],trumpmask);
      cout << ";p0 "; g.print_vector(asorskat & msorskat, trumpmask);
      cout << ";th "; g.print_vector(triplehand,trumpmask);
      //      cout << ";allopp "; g.print_vector(origtriplehand,trumpmask); 
      cout << ";asorsk "; g.print_vector(asorskat,trumpmask); 
      cout << ";msorsk "; g.print_vector(msorskat,trumpmask);
    cout << "-----------------" << endl;
    */
    for (int i=0;i<DECKSIZE;i++) {
      card bitindex = (ONE<<i);
      if (played & bitindex) {
	belief[0][i] = 0;
	belief[1][i] = 0;
	belief[2][i] = 0;
	belief[3][i] = 0;
      }
      else if (hands[0] & bitindex) {
	belief[0][i] = 100; 
	belief[1][i] = 0;
	belief[2][i] = 0;
	belief[3][i] = 0;
      }
      else if (hands[1] & bitindex) {
	//	cout << "got you " << i << endl;
	belief[0][i] = 0;
	belief[1][i] = 100; 
	belief[2][i] = 0;
	belief[3][i] = 0;
      }
      else if (hands[2] & bitindex) {
	belief[0][i] = 0;
	belief[1][i] = 0;
	belief[2][i] = 100; 
	belief[3][i] = 0;
      }
      else if (asorskat & bitindex) {
	if (player == 1) {
	  belief[0][i] = 83.33;
	  belief[1][i] = 0;
	  belief[2][i] = 0;
	  belief[3][i] = 16.67;
	}
	if (player == 2) {
	  belief[0][i] = 83.33;
	  belief[1][i] = 0;
	  belief[2][i] = 0;
	  belief[3][i] = 16.67;
	}
      }
      else if (msorskat & bitindex) {
	if (player == 1) {
	  belief[0][i] = 0;
	  belief[1][i] = 0;
	  belief[2][i] = 83.33;
	  belief[3][i] = 16.67;
	}
	if (player == 2) {
	  belief[0][i] = 0;
	  belief[1][i] = 83.33;
	  belief[2][i] = 0;
	  belief[3][i] = 16.67;
	}
      }
      else if (triplehand & bitindex) {
	if (player == 1) {
	  belief[0][i] = 45.45;
	  belief[1][i] = 0;
	  belief[2][i] = 45.45;
	  belief[3][i] = 9.10;
	}
	if (player == 2) {
	  belief[0][i] = 45.45;
	  belief[1][i] = 45.45;
	  belief[2][i] = 0;
	  belief[3][i] = 9.10;
	}
      }
      else {
	belief[0][i] = -1;
	belief[1][i] = -1;
	belief[2][i] = -1;
	belief[3][i] = -1;
      }
    }
  }

  for (int i=0;i<DECKSIZE;i++) {
    if ((ONE<<i) & asorskat & msorskat) {
      belief[0][i] = 0;
      belief[1][i] = 0;
      belief[2][i] = 0;
      belief[3][i] = 100;
    }
  }
  if (count(asorskat & msorskat) == 2) {
    for (int i=0;i<DECKSIZE;i++) {
      if (belief[3][i] != 100) belief[3][i] = 0;
    }
  }  
  for (int k=0;k<3;k++) { // pidgeon-hole principle
    int c=0;
    for (int i=0;i<DECKSIZE;i++) {
      if ((int)belief[0][i] > 0) c++;
    }
    if (c == cnt[0] && c != 0 && player != 0) {
      //    cout << "KNOW ALL CARDS P0 " << cnt[0] << endl;
      for (int j=0;j<DECKSIZE;j++) {
	if ((int)belief[0][j] > 0) {
	  belief[0][j] = 100;
	  belief[1][j] = 0;
	  belief[2][j] = 0;
	  belief[3][j] = 0;
	}
      }
    }
    c = 0;
    for (int i=0;i<DECKSIZE;i++) {
      if (belief[1][i] > 0) c++;
    }
    if (c == cnt[1] && c != 0 && player != 1) {
      ///    cout << "KNOW ALL CARDS P1 " << cnt[1] << endl;
      for (int j=0;j<DECKSIZE;j++) {
	if ((int)belief[1][j] > 0) {
	  belief[1][j] = 100;
	  belief[0][j] = 0;
	  belief[2][j] = 0;
	  belief[3][j] = 0;
	}
      }
    }
    c = 0;
    for (int i=0;i<DECKSIZE;i++) {
      if (belief[2][i] > 0) c++;
    }
    if (c == cnt[2] && c!= 0 && player != 2) {
      //    cout << "KNOW ALL CARDS P2 " << cnt[2] << endl;
      for (int j=0;j<DECKSIZE;j++) {
	if ((int)belief[2][j] > 0) {
	  belief[2][j] = 100;
	  belief[0][j] = 0;
	  belief[1][j] = 0;
	  belief[3][j] = 0;
	}
      }
    }
  }

  for (int i=0;i<DECKSIZE;i++) {
    for (int j=0;j<PLAYER;j++) {
      if (belief[j][i] != 100) {
	double sum = belief[1][i] + belief[2][i] + belief[3][i] + belief[0][i];;
	if (sum != 0) {
	  belief[0][i] = 100.0 * belief[0][i] / sum;
	  belief[1][i] = 100.0 * belief[1][i] / sum;
	  belief[2][i] = 100.0 * belief[2][i] / sum;
	  belief[3][i] = 100.0 * belief[3][i] / sum;
	}
      }
    }
  }
  for (int i=0;i<DECKSIZE;i++) {
    for (int j=0;j<PLAYER;j++) {
      if (belief[j][i] == 100) p[j][cardsfixed[j]++] = i;
    }
  }
  for (int j=0;j<PLAYER;j++) {  
    for (int i=cardsfixed[j];i<cnt[j];i++) p[j][i] = 36;
    for (int i=cnt[j];i<HANDSIZE;i++) p[j][i] = 37;
  }
  for (int i=0;i<DECKSIZE;i++) {
      for (int j=0;j<PLAYER;j++) {
	if (belief[0][i] == 100) knowhand[j] |= (ONE<<i);
      }
  }
}

void KnowledgeBase::print() {
  convert();
  int player = playerpos;
  cout << "Belief-P"<< player << endl;
  for (int j=0;j<PLAYER;j++) {
    cout << " ;P;" << j; 
    for (int i=0;i<HANDSIZE;i++)
      cout << card_name[p[j][i]] << ";";
    cout << endl;
  }
  for (int i=0;i<DECKSIZE;i++)
    cout << "; " << card_name[i]; cout << endl;
  for (int j=0;j<PLAYER;j++) {
    cout << "P" << j<<";";
    for (int i=0;i<DECKSIZE;i++)
      std::cout << std::setw(3) << (int)belief[j][i] << ";";
    cout << endl;
  }
}

