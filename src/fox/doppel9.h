// Doppelkopf playing program using expert knowledge, expert games, belief states, and-or search
// (c) 2022, Stefan Edelkamp

#ifndef __CARDS_H
#define __CARDS_H

// bitvector 

#ifndef FROMFILE
#include "../card.h"
#endif

typedef unsigned long long card; // for bitvector representation of cards

static card ALLBITS = 0;

static const int CARDTYPES = 4;
static const int TRUMPCARDTYPES = 13;
static const int MAXTRICKS = 12;
static const int HANDSIZE = 12;
static const int DECKSIZE = 48;
static const int PLAYSIZE = 48;
static const int WORDSIZE = 64;
static const int SUITCOUNT = 4;
static const int PLAYER = 4;

// unicode of suits for console representation
static char suitname[6][80] = { "\u2663","\u2660","\u2665","\u25C6","_","X" };

enum {
      he101,he102,
      krda1,krda2,pida1,pida2,heda1,heda2,kada1,kada2,
      krbu1,krbu2,pibu1,pibu2,hebu1,hebu2,kabu1,kabu2,
      kaas1,kaas2,ka101,ka102,kako1,kako2,ka091,ka092,
      kras1,kras2,kr101,kr102,krko1,krko2,kr091,kr092,
      pias1,pias2,pi101,pi102,piko1,piko2,pi091,pi092,
      heas1,heas2,heko1,heko2,he091,he092
};

enum {
      he10,
      krda,pida,heda,kada,
      krbu,pibu,hebu,kabu,
      kaas,ka10,kako,ka09,
      kras,kr10,krko,kr09,
      pias,pi10,piko,pi09,
      heas,heko,he09,
};

// unicode of cards for console representation
static char card_name[DECKSIZE+6][80] =
  {
   "\u001b[31m\u2665Z\u001b[0m",   "\u001b[31m\u2665Z\u001b[0m",
   "\u2663D",   "\u2663D",
   "\u001b[32m\u2660D\u001b[0m",   "\u001b[32m\u2660D\u001b[0m",
   "\u001b[31m\u2665D\u001b[0m",   "\u001b[31m\u2665D\u001b[0m",
   "\u001b[33m\u25C6D\u001b[0m",   "\u001b[33m\u25C6D\u001b[0m",   
   "\u2663B",   "\u2663B",
   "\u001b[32m\u2660B\u001b[0m",   "\u001b[32m\u2660B\u001b[0m",
   "\u001b[31m\u2665B\u001b[0m",   "\u001b[31m\u2665B\u001b[0m",
   "\u001b[33m\u25C6B\u001b[0m",   "\u001b[33m\u25C6B\u001b[0m", 
   "\u001b[33m\u25C6A\u001b[0m",   "\u001b[33m\u25C6A\u001b[0m",
   "\u001b[33m\u25C6Z\u001b[0m",   "\u001b[33m\u25C6Z\u001b[0m",
   "\u001b[33m\u25C6K\u001b[0m",   "\u001b[33m\u25C6K\u001b[0m",
   "\u001b[33m\u25C69\u001b[0m",   "\u001b[33m\u25C69\u001b[0m",
   "\u2663A",   "\u2663A",
   "\u2663Z",   "\u2663Z",
   "\u2663K",   "\u2663K",
   "\u26639",   "\u26639", 
   "\u001b[32m\u2660A\u001b[0m",   "\u001b[32m\u2660A\u001b[0m",
   "\u001b[32m\u2660Z\u001b[0m",   "\u001b[32m\u2660Z\u001b[0m",
   "\u001b[32m\u2660K\u001b[0m",   "\u001b[32m\u2660K\u001b[0m",
   "\u001b[32m\u26609\u001b[0m",   "\u001b[32m\u26609\u001b[0m",
   "\u001b[31m\u2665A\u001b[0m",   "\u001b[31m\u2665A\u001b[0m",
   "\u001b[31m\u2665K\u001b[0m",   "\u001b[31m\u2665K\u001b[0m",
   "\u001b[31m\u26659\u001b[0m",   "\u001b[31m\u26659\u001b[0m", 
  "\u2663_","\u2660_","\u2665_","\u25C6_",
  "__","XX"
};

//#define logging printf

#define logging // printf

#define ZERO 0ULL 
#ifdef FROMFILE
#define ONE 1ULL 
#endif
//#define ALLBITS 281474976710655ULL

#define CLZ __builtin_clzll
#define CTZ __builtin_ctzll
#define POPCOUNT __builtin_popcountll

static int val[DECKSIZE] =
{
 10,10,
 3,3,3,3,3,3,3,3,
 2,2,2,2,2,2,2,2,
 11,11,10,10,4,4,0,0,
 11,11,10,10,4,4,0,0,
 11,11,10,10,4,4,0,0,
 11,11,4,4,0,0,
};

static int Card[DECKSIZE] =
{
 he10,he10,
 krda,krda,pida,pida,heda,heda,kada,kada,
 krbu,krbu,pibu,pibu,hebu,hebu,kabu,kabu,
 kaas,kaas,ka10,ka10,kako,kako,ka09,ka09,
 kras,kras,kr10,kr10,krko,krko,kr09,kr09,
 pias,pias,pi10,pi10,piko,piko,pi09,pi09,
 heas,heas,heko,heko,he09,he09
};

static int jacksorder[DECKSIZE] = 
{
 krbu1,krbu2,pibu1,pibu2,hebu1,hebu2,kabu1,kabu2,
 kras1,kras2,kr101,kr102,krko1,krko2,krda1,krda2,kr091,kr092,
 pias1,pias2,pi101,pi102,piko1,piko2,pida1,pida2,pi091,pi092,
 heas1,heas2,he101,he102,heko1,heko2,heda1,heda2,he091,he092,
 kaas1,kaas2,ka101,ka102,kako1,kako2,kada1,kada2,ka091,ka092,
};

static int queensorder[DECKSIZE] = 
{
 krda1,krda2,pida1,pida2,heda1,heda2,kada1,kada2,
 kras1,kras2,kr101,kr102,krko1,krko2,krbu1,krbu2,kr091,kr092,
 pias1,pias2,pi101,pi102,piko1,piko2,pibu1,pibu2,pi091,pi092,
 heas1,heas2,he101,he102,heko1,heko2,hebu1,hebu2,he091,he092,
 kaas1,kaas2,ka101,ka102,kako1,kako2,kabu1,kabu2,ka091,ka092,
};

static int Suit[DECKSIZE] =
{
 4,4,
 4,4,4,4,4,4,4,4,
 4,4,4,4,4,4,4,4,
 4,4,4,4,4,4,4,4,
 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1,
 2,2,2,2,2,2
};

static int SuitJacks[DECKSIZE] =
{
 2,2,
 0,0,1,1,2,2,3,3,
 4,4,4,4,4,4,4,4,
 3,3,3,3,3,3,3,3,
 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1,
 2,2,2,2,2,2
};


static int SuitQueens[DECKSIZE] =
{
 2,2,
 4,4,4,4,4,4,4,4,
 0,0,1,1,2,2,3,3,
 3,3,3,3,3,3,3,3,
 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1,
 2,2,2,2,2,2
};

static int SuitHearts[DECKSIZE] =
{
 4,4,
 4,4,4,4,4,4,4,4,
 4,4,4,4,4,4,4,4,
 3,3,3,3,3,3,3,3,
 0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1,
 4,4,4,4,4,4
};

static int SuitSpades[DECKSIZE] =
{
 4,4,
 4,4,4,4,4,4,4,4,
 4,4,4,4,4,4,4,4,
 3,3,3,3,3,3,3,3,
 0,0,0,0,0,0,0,0,
 4,4,4,4,4,4,4,4,
 2,2,2,2,2,2
};


static int SuitClubs[DECKSIZE] =
{
 4,4,
 4,4,4,4,4,4,4,4,
 4,4,4,4,4,4,4,4,
 3,3,3,3,3,3,3,3,
 4,4,4,4,4,4,4,4,
 1,1,1,1,1,1,1,1,
 2,2,2,2,2,2
};


#define COST(a,b,c,d) (val[a]+val[b]+val[c]+val[d])
#define VALUE(x) (val[x])

static card mask[SUITCOUNT] = {ZERO,ZERO,ZERO,ZERO};
/*
// for each color bit mask
static card mask[4] = {
		       255ULL << 26,
		       255ULL << 34,
		       63ULL << 42,
		       67108863ULL << 0,
};
*/

// bitvalue of cards
static card
  CA = (ONE<<kras1)|(ONE<<kras2),
  CK = (ONE<<krko1)|(ONE<<krko2),
  CQ = (ONE<<krda1)|(ONE<<krda2),
  CJ = (ONE<<krbu1)|(ONE<<krbu2),
  CT = (ONE<<kr101)|(ONE<<kr102),
  C9 = (ONE<<kr091)|(ONE<<kr092),
  SA = (ONE<<pias1)|(ONE<<pias2),
  SK = (ONE<<piko1)|(ONE<<piko2),
  SQ = (ONE<<pida1)|(ONE<<pida2),
  SJ = (ONE<<pibu1)|(ONE<<pibu2),
  ST = (ONE<<pi101)|(ONE<<pi102),
  S9 = (ONE<<pi091)|(ONE<<pi092),
  HA = (ONE<<heas1)|(ONE<<heas2),
  HK = (ONE<<heko1)|(ONE<<heko2),
  HQ = (ONE<<heda1)|(ONE<<heda2),
  HJ = (ONE<<hebu1)|(ONE<<hebu2),
  HT = (ONE<<he101)|(ONE<<he102),
  H9 = (ONE<<he091)|(ONE<<he092),
  DA = (ONE<<kaas1)|(ONE<<kaas2),
  DK = (ONE<<kako1)|(ONE<<kako2),
  DQ = (ONE<<kada1)|(ONE<<kada2),
  DJ = (ONE<<kabu1)|(ONE<<kabu2),
  DT = (ONE<<ka101)|(ONE<<ka102),
  D9 = (ONE<<ka091)|(ONE<<ka092);

// bitvector of card groups
static const card ACES =   ((ONE<<kras1)|(ONE<<pias1)|(ONE<<heas1)|(ONE<<kaas1)|(ONE<<kras2)|(ONE<<pias2)|(ONE<<heas2)|(ONE<<kaas2));
static const card KINGS =  ((ONE<<krko1)|(ONE<<piko1)|(ONE<<heko1)|(ONE<<kako1)|(ONE<<krko2)|(ONE<<piko2)|(ONE<<heko2)|(ONE<<kako2));
static const card QUEENS = ((ONE<<krda1)|(ONE<<pida1)|(ONE<<heda1)|(ONE<<kada1)|(ONE<<krda2)|(ONE<<pida2)|(ONE<<heda2)|(ONE<<kada2));
static const card JACKS =  ((ONE<<krbu1)|(ONE<<pibu1)|(ONE<<hebu1)|(ONE<<kabu1)|(ONE<<krbu2)|(ONE<<pibu2)|(ONE<<hebu2)|(ONE<<kabu2));
static const card TENS =   ((ONE<<kr101)|(ONE<<pi101)|(ONE<<he101)|(ONE<<ka101)|(ONE<<kr102)|(ONE<<pi102)|(ONE<<he102)|(ONE<<ka102));
static const card NINES =  ((ONE<<kr091)|(ONE<<pi091)|(ONE<<he091)|(ONE<<ka091)|(ONE<<kr092)|(ONE<<pi092)|(ONE<<he092)|(ONE<<ka092));

#define DIAMONDS ((ONE<<ka091)|(ONE<<ka101)|(ONE<<kabu1)|(ONE<<kada1)|(ONE<<kako1)|(ONE<<ka092)|(ONE<<kaas1)|(ONE<<ka102)|(ONE<<kabu2)|(ONE<<kada2)|(ONE<<kako2)|(ONE<<kaas2))
#define HEARTS   ((ONE<<he091)|(ONE<<he101)|(ONE<<hebu1)|(ONE<<heda1)|(ONE<<heko1)|(ONE<<he092)|(ONE<<heas1)|(ONE<<he102)|(ONE<<hebu2)|(ONE<<heda2)|(ONE<<heko2)|(ONE<<heas2))
#define SPADES   ((ONE<<pi091)|(ONE<<pi101)|(ONE<<pibu1)|(ONE<<pida1)|(ONE<<piko1)|(ONE<<pi092)|(ONE<<pias1)|(ONE<<pi102)|(ONE<<pibu2)|(ONE<<pida2)|(ONE<<piko2)|(ONE<<pias2))
#define CLUBS    ((ONE<<kr091)|(ONE<<kr101)|(ONE<<krbu1)|(ONE<<krda1)|(ONE<<krko1)|(ONE<<kr092)|(ONE<<kras1)|(ONE<<kr102)|(ONE<<krbu2)|(ONE<<krda2)|(ONE<<krko2)|(ONE<<kras2))

static const card abovesuit[CARDTYPES] = {ACES,TENS,KINGS,NINES};
static const card abovetrump[TRUMPCARDTYPES] = {HT,CQ,SQ,HQ,DQ,CJ,SJ,HJ,DJ,DA,DT,DK,D9}; 

static const card abovetrumpqueens[4] = {CQ,SQ,HQ,DQ};
static const card abovetrumpjacks[4] = {CQ,SQ,HQ,DQ};
static const card abovetrumpheart[12] = {HT,CQ,SQ,HQ,DQ,CJ,SJ,HJ,DJ,HA,HK,H9}; 
static const card abovetrumpspades[13] = {HT,CQ,SQ,HQ,DQ,CJ,SJ,HJ,DJ,SA,ST,SK,S9}; 
static const card abovetrumpclubs[13] = {HT,CQ,SQ,HQ,DQ,CJ,SJ,HJ,DJ,CA,CT,CK,C9}; 

#ifdef DISPLAY
 #define SHOW(x) card_name[x]
#else
 #define SHOW(x) x
#endif

#endif
