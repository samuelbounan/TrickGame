// Bridge playing program using expert knowledge, expert games, belief states, and-or search
// (c) 2022, Stefan Edelkamp

#ifndef __CARDS_H
#define __CARDS_H

// bitvector 

#ifndef FROMFILE
#include "../card.h"
#endif

typedef unsigned long long card; // for bitvector representation of cards

static card ALLBITS = 0;

static const int SUITSIZE = 13;
static const int MAXTRICKS = 13;
static const int HANDSIZE = 13;
static const int DECKSIZE = 52;
static const int PLAYSIZE = 52;
static const int WORDSIZE = 64;
static const int PLAYER = 4;
static const int SUITCOUNT = 4;
static const int CARDTYPES = 13;
static const int TRUMPCARDTYPES = 13;

#define COLHEART "\u001b[31m\u2665\u001b[0m"
#define COLDIAMOND "\u001b[33m\u25C6\u001b[0m"
#define COLSPADES "\u001b[32m\u25C6\u001b[0m"					   

// unicode of suits for console representation
static char suitname[6][80] = { "\u2663",COLSPADES,COLHEART,COLDIAMOND,"_","X" };

enum { // default encoding
  kras,krko,krda,krbu,kr10,kr09,kr08,kr07,kr06,kr05,kr04,kr03,kr02,
  pias,piko,pida,pibu,pi10,pi09,pi08,pi07,pi06,pi05,pi04,pi03,pi02,
  heas,heko,heda,hebu,he10,he09,he08,he07,he06,he05,he04,he03,he02,
  kaas,kako,kada,kabu,ka10,ka09,ka08,ka07,ka06,ka05,ka04,ka03,ka02,
};

// mapping trump to default ordering
static int suitorder[DECKSIZE] = {
  kras,krko,krda,krbu,kr10,kr09,kr08,kr07,kr06,kr05,kr04,kr03,kr02,
  pias,piko,pida,pibu,pi10,pi09,pi08,pi07,pi06,pi05,pi04,pi03,pi02,
  heas,heko,heda,hebu,he10,he09,he08,he07,he06,he05,he04,he03,he02,
  kaas,kako,kada,kabu,ka10,ka09,ka08,ka07,ka06,ka05,ka04,ka03,ka02,
};

// mapping trump to default ordering
static int trumporder[DECKSIZE] = {
  kras,krko,krda,krbu,kr10,kr09,kr08,kr07,kr06,kr05,kr04,kr03,kr02,
  pias,piko,pida,pibu,pi10,pi09,pi08,pi07,pi06,pi05,pi04,pi03,pi02,
  heas,heko,heda,hebu,he10,he09,he08,he07,he06,he05,he04,he03,he02,
  kaas,kako,kada,kabu,ka10,ka09,ka08,ka07,ka06,ka05,ka04,ka03,ka02,
};

// unicode of cards for console representation
static char card_name[DECKSIZE+6][80] = {
  "\u001b[37m\u2663A\u001b[0m", "\u001b[37m\u2663K\u001b[0m", "\u001b[37m\u2663Q\u001b[0m", "\u001b[37m\u2663J\u001b[0m", "\u001b[37m\u2663T\u001b[0m", "\u001b[37m\u26639\u001b[0m", "\u001b[37m\u26638\u001b[0m", "\u001b[37m\u26637\u001b[0m", "\u001b[37m\u26636\u001b[0m", "\u001b[37m\u26635\u001b[0m", "\u001b[37m\u26634\u001b[0m", "\u001b[37m\u26633\u001b[0m", "\u001b[37m\u26632\u001b[0m",
  "\u001b[32m\u2660A\u001b[0m", "\u001b[32m\u2660K\u001b[0m", "\u001b[32m\u2660Q\u001b[0m", "\u001b[32m\u2660J\u001b[0m", "\u001b[32m\u2660T\u001b[0m", "\u001b[32m\u26609\u001b[0m", "\u001b[32m\u26608\u001b[0m", "\u001b[32m\u26607\u001b[0m", "\u001b[32m\u26606\u001b[0m", "\u001b[32m\u26605\u001b[0m", "\u001b[32m\u26604\u001b[0m", "\u001b[32m\u26603\u001b[0m", "\u001b[32m\u26602\u001b[0m",
  "\u001b[31m\u2665A\u001b[0m", "\u001b[31m\u2665K\u001b[0m", "\u001b[31m\u2665Q\u001b[0m", "\u001b[31m\u2665J\u001b[0m", "\u001b[31m\u2665T\u001b[0m", "\u001b[31m\u26659\u001b[0m", "\u001b[31m\u26658\u001b[0m", "\u001b[31m\u26657\u001b[0m", "\u001b[31m\u26656\u001b[0m", "\u001b[31m\u26655\u001b[0m", "\u001b[31m\u26654\u001b[0m", "\u001b[31m\u26653\u001b[0m", "\u001b[31m\u26652\u001b[0m",
  "\u001b[33m\u25C6A\u001b[0m", "\u001b[33m\u25C6K\u001b[0m", "\u001b[33m\u25C6Q\u001b[0m", "\u001b[33m\u25C6J\u001b[0m", "\u001b[33m\u25C6T\u001b[0m", "\u001b[33m\u25C69\u001b[0m", "\u001b[33m\u25C68\u001b[0m", "\u001b[33m\u25C67\u001b[0m", "\u001b[33m\u25C66\u001b[0m", "\u001b[33m\u25C65\u001b[0m", "\u001b[33m\u25C64\u001b[0m", "\u001b[33m\u25C63\u001b[0m", "\u001b[33m\u25C62\u001b[0m",
  "\u2663_","\u2660_","\u2665_","\u25C6_",
  "__","XX"
};

/*
// unicode of cards for console representation
static char card_name[DECKSIZE+6][80] = {
  "\u2663A", "\u2663K", "\u2663D", "\u2663B", "\u2663Z", "\u26639", "\u26638", "\u26637", "\u26636", "\u26635", "\u26634", "\u26633", "\u26632",
  "\u2660A", "\u2660K", "\u2660D", "\u2660B", "\u2660Z", "\u26609", "\u26608", "\u26607", "\u26606", "\u26605", "\u26604", "\u26603", "\u26602",
  "\u2665A", "\u2665K", "\u2665D", "\u2665B", "\u2665Z", "\u26659", "\u26658", "\u26657", "\u26656", "\u26655", "\u26654", "\u26653", "\u26652",
  "\u25C6A", "\u25C6K", "\u25C6D", "\u25C6B", "\u25C6Z", "\u25C69", "\u25C68", "\u25C67", "\u25C66", "\u25C65", "\u25C64", "\u25C63", "\u25C62",
  "\u2663_","\u2660_","\u2665_","\u25C6_",
  "__","XX"
};
*/
//#define logging printf

#define logging // printf

#define ZERO 0ULL 
#ifdef FROMFILE
#define ONE 1ULL 
#endif
//#define ALL 8191ULL 
//#define ALLBITS 4503599627370495ULL

#define CLZ __builtin_clzll
#define CTZ __builtin_ctzll
#define POPCOUNT __builtin_popcountll

static int val[DECKSIZE] =
{
 0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0,
 0,0,0,0,0,0,0,0,0,0,0,0,0
};

static int Card[DECKSIZE] =
{
  kras,krko,krda,krbu,kr10,kr09,kr08,kr07,kr06,kr05,kr04,kr03,kr02,
  pias,piko,pida,pibu,pi10,pi09,pi08,pi07,pi06,pi05,pi04,pi03,pi02,
  heas,heko,heda,hebu,he10,he09,he08,he07,he06,he05,he04,he03,he02,
  kaas,kako,kada,kabu,ka10,ka09,ka08,ka07,ka06,ka05,ka04,ka03,ka02
};

static int Suit[DECKSIZE] =
{
 0,0,0,0,0,0,0,0,0,0,0,0,0,
 1,1,1,1,1,1,1,1,1,1,1,1,1,
 2,2,2,2,2,2,2,2,2,2,2,2,2,
 3,3,3,3,3,3,3,3,3,3,3,3,3
};

#define COST(a,b,c,d) (1)
#define VALUE(x) (val[x])

static card mask[SUITCOUNT] = {ZERO,ZERO,ZERO,ZERO};

// for each color bit mask
//static card mask[4] = {ALL << 0,ALL << SUITSIZE, ALL << 2*SUITSIZE, ALL << 3*SUITSIZE};

// bitvalue of cards
static card
  CA = (ONE<<kras),CK = (ONE<<krko),CQ = (ONE<<krda),CJ = (ONE<<krbu),
  CT = (ONE<<kr10),C9 = (ONE<<kr09),C8 = (ONE<<kr08),C7 = (ONE<<kr07),
  C6 = (ONE<<ka06),C5 = (ONE<<ka05),C4 = (ONE<<ka04),C3 = (ONE<<kr03),C2 = (ONE<<kr02),
  SA = (ONE<<pias),SK = (ONE<<piko),SQ = (ONE<<pida),SJ = (ONE<<pibu),
  ST = (ONE<<pi10),S9 = (ONE<<pi09),S8 = (ONE<<pi08),S7 = (ONE<<pi07),
  S6 = (ONE<<pi06),S5 = (ONE<<pi05),S4 = (ONE<<pi04),S3 = (ONE<<pi03),S2 = (ONE<<pi02),
  HA = (ONE<<heas),HK = (ONE<<heko),HQ = (ONE<<heda),HJ = (ONE<<hebu),
  HT = (ONE<<he10),H9 = (ONE<<he09),H8 = (ONE<<he08),H7 = (ONE<<he07),
  H6 = (ONE<<he06),H5 = (ONE<<he05),H4 = (ONE<<he04),H3 = (ONE<<he03),H2 = (ONE<<he02),
  DA = (ONE<<kaas),DK = (ONE<<kako),DQ = (ONE<<kada),DJ = (ONE<<kabu),
  DT = (ONE<<ka10),D9 = (ONE<<ka09),D8 = (ONE<<ka08),D7 = (ONE<<ka07),
  D6 = (ONE<<ka06),D5 = (ONE<<ka05),D4 = (ONE<<ka04),D3 = (ONE<<ka03),D2 = (ONE<<ka02);


// bitvector of card groups
static const card ACES =   ((ONE<<kras)|(ONE<<pias)|(ONE<<heas)|(ONE<<kaas));
static const card KINGS =  ((ONE<<krko)|(ONE<<piko)|(ONE<<heko)|(ONE<<kako));
static const card QUEENS = ((ONE<<krda)|(ONE<<pida)|(ONE<<heda)|(ONE<<kada));
static const card JACKS =  ((ONE<<krbu)|(ONE<<pibu)|(ONE<<hebu)|(ONE<<kabu));
static const card TENS =   ((ONE<<kr10)|(ONE<<pi10)|(ONE<<he10)|(ONE<<ka10));
static const card NINES =  ((ONE<<kr09)|(ONE<<pi09)|(ONE<<he09)|(ONE<<ka09));
static const card EIGHTS =  ((ONE<<kr08)|(ONE<<pi08)|(ONE<<he08)|(ONE<<ka08));
static const card SEVENS =  ((ONE<<kr07)|(ONE<<pi07)|(ONE<<he07)|(ONE<<ka07));
static const card SIXS =  ((ONE<<kr06)|(ONE<<pi06)|(ONE<<he06)|(ONE<<ka06));
static const card FIVES =  ((ONE<<kr05)|(ONE<<pi05)|(ONE<<he05)|(ONE<<ka05));
static const card FOURS =  ((ONE<<kr04)|(ONE<<pi04)|(ONE<<he04)|(ONE<<ka04));
static const card THREES =  ((ONE<<kr03)|(ONE<<pi03)|(ONE<<he03)|(ONE<<ka03));
static const card TWOS =  ((ONE<<kr02)|(ONE<<pi02)|(ONE<<he02)|(ONE<<ka02));

#define DIAMONDS ((ONE<<ka02)|(ONE<<ka03)|(ONE<<ka04)|(ONE<<ka05)|(ONE<<ka06)|(ONE<<ka07)|(ONE<<ka08)|(ONE<<ka09)|(ONE<<ka10)|(ONE<<kabu)|(ONE<<kada)|(ONE<<kako)|(ONE<<kaas))
#define HEARTS   ((ONE<<he02)|(ONE<<he03)|(ONE<<he04)|(ONE<<he05)|(ONE<<he06)|(ONE<<he07)|(ONE<<he08)|(ONE<<he09)|(ONE<<he10)|(ONE<<hebu)|(ONE<<heda)|(ONE<<heko)|(ONE<<heas))
#define SPADES   ((ONE<<pi02)|(ONE<<pi03)|(ONE<<pi04)|(ONE<<pi05)|(ONE<<pi06)|(ONE<<pi07)|(ONE<<pi08)|(ONE<<pi09)|(ONE<<pi10)|(ONE<<pibu)|(ONE<<pida)|(ONE<<piko)|(ONE<<pias))
#define CLUBS    ((ONE<<kr02)|(ONE<<kr03)|(ONE<<kr04)|(ONE<<kr05)|(ONE<<kr06)|(ONE<<kr07)|(ONE<<kr08)|(ONE<<kr09)|(ONE<<kr10)|(ONE<<krbu)|(ONE<<krda)|(ONE<<krko)|(ONE<<kras))


#ifdef DISPLAY
 #define SHOW(x) card_name[x]
#else
 #define SHOW(x) x
#endif

static const card abovesuit[CARDTYPES] = {ACES,KINGS,QUEENS,JACKS,TENS,NINES,EIGHTS,SEVENS,SIXS,FIVES,FOURS,THREES,TWOS};
static const card abovetrump[TRUMPCARDTYPES] = {ACES,KINGS,QUEENS,JACKS,TENS,NINES,EIGHTS,SEVENS,SIXS,FIVES,FOURS,THREES,TWOS};



#endif