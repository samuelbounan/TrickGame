// Belote playing program using expert knowledge, expert games, belief states,
// and-or search (c) 2022, Stefan Edelkamp

#ifndef __CARDS_H
#define __CARDS_H

// bitvector

#ifndef FROMFILE
#include "../card.h"
#endif

typedef unsigned int card;  // for bitvector representation of cards

static card ALLBITS = 0;

static const int PLAYER = 4;
static const int CARDTYPES = 8;
static const int TRUMPCARDTYPES = 8;
static const int SUITSIZE = 8;
static const int SUITCOUNT = 4;
static const int MAXTRICKS = 8;
static const int HANDSIZE = 8;
static const int DECKSIZE = 32;
static const int PLAYSIZE = 32;
static const int WORDSIZE = 32;
static const int printing = 2;

#ifdef DISPLAY
#define SHOW(x) card_name[x]
#else
#define SHOW(x) x
#endif

#define logging  // printf ("\n"); printf

#define ZERO 0

#ifdef FROMFILE
#define ONE 1
#endif

#define CLZ __builtin_clz
#define CTZ __builtin_ctz
#define POPCOUNT __builtin_popcount

#define COLHEART "\u001b[31m\u2665\u001b[0m"
#define COLDIAMOND "\u001b[33m\u25C6\u001b[0m"
#define COLSPADES "\u001b[32m\u2660\u001b[0m"

// unicode of suits for console representation
static char suitname[SUITCOUNT + 2][80] = {"\u2663",   COLSPADES, COLHEART,
                                           COLDIAMOND, "_",       "X"};

// unicode of cards for console representation
static char card_name[DECKSIZE + SUITCOUNT + 2][80] = {
    "\u001b[37m\u2663A\u001b[0m",
    "\u001b[37m\u2663T\u001b[0m",
    "\u001b[37m\u2663K\u001b[0m",
    "\u001b[37m\u2663Q\u001b[0m",
    "\u001b[37m\u2663J\u001b[0m",
    "\u001b[37m\u26639\u001b[0m",
    "\u001b[37m\u26638\u001b[0m",
    "\u001b[37m\u26637\u001b[0m",
    "\u001b[32m\u2660A\u001b[0m",
    "\u001b[32m\u2660T\u001b[0m",
    "\u001b[32m\u2660K\u001b[0m",
    "\u001b[32m\u2660Q\u001b[0m",
    "\u001b[32m\u2660J\u001b[0m",
    "\u001b[32m\u26609\u001b[0m",
    "\u001b[32m\u26608\u001b[0m",
    "\u001b[32m\u26607\u001b[0m",
    "\u001b[31m\u2665A\u001b[0m",
    "\u001b[31m\u2665T\u001b[0m",
    "\u001b[31m\u2665K\u001b[0m",
    "\u001b[31m\u2665Q\u001b[0m",
    "\u001b[31m\u2665J\u001b[0m",
    "\u001b[31m\u26659\u001b[0m",
    "\u001b[31m\u26658\u001b[0m",
    "\u001b[31m\u26657\u001b[0m",
    "\u001b[33m\u25C6A\u001b[0m",
    "\u001b[33m\u25C6T\u001b[0m",
    "\u001b[33m\u25C6K\u001b[0m",
    "\u001b[33m\u25C6Q\u001b[0m",
    "\u001b[33m\u25C6J\u001b[0m",
    "\u001b[33m\u25C69\u001b[0m",
    "\u001b[33m\u25C68\u001b[0m",
    "\u001b[33m\u25C67\u001b[0m",
    "\u2663_",
    "\u2660_",
    "\u2665_",
    "\u25C6_",
    "__",
    "XX"};
/*
static char card_name[DECKSIZE+6][80] = {
  "\u001b[47m\u001b[30m\u2663A\u001b[0m",
"\u001b[47m\u001b[30m\u2663T\u001b[0m", "\u001b[47m\u001b[30m\u2663K\u001b[0m",
"\u001b[47m\u001b[30m\u2663Q\u001b[0m", "\u001b[47m\u001b[30m\u2663J\u001b[0m",
"\u001b[47m\u001b[30m\u26639\u001b[0m", "\u001b[47m\u001b[30m\u26638\u001b[0m",
"\u001b[47m\u001b[30m\u26637\u001b[0m",
  "\u001b[47m\u001b[34m\u2660A\u001b[0m",
"\u001b[47m\u001b[34m\u2660T\u001b[0m", "\u001b[47m\u001b[34m\u2660K\u001b[0m",
"\u001b[47m\u001b[34m\u2660Q\u001b[0m", "\u001b[47m\u001b[34m\u2660J\u001b[0m",
"\u001b[47m\u001b[34m\u26609\u001b[0m", "\u001b[47m\u001b[34m\u26608\u001b[0m",
"\u001b[47m\u001b[34m\u26607\u001b[0m",
  "\u001b[47m\u001b[31m\u2665A\u001b[0m",
"\u001b[47m\u001b[31m\u2665T\u001b[0m", "\u001b[47m\u001b[31m\u2665K\u001b[0m",
"\u001b[47m\u001b[31m\u2665Q\u001b[0m", "\u001b[47m\u001b[31m\u2665J\u001b[0m",
"\u001b[47m\u001b[31m\u26659\u001b[0m", "\u001b[47m\u001b[31m\u26658\u001b[0m",
"\u001b[47m\u001b[31m\u26657\u001b[0m",
  "\u001b[47m\u001b[35m\u25C6A\u001b[0m",
"\u001b[47m\u001b[35m\u25C6T\u001b[0m", "\u001b[47m\u001b[35m\u25C6K\u001b[0m",
"\u001b[47m\u001b[35m\u25C6Q\u001b[0m", "\u001b[47m\u001b[35m\u25C6J\u001b[0m",
"\u001b[47m\u001b[35m\u25C69\u001b[0m", "\u001b[47m\u001b[35m\u25C68\u001b[0m",
"\u001b[47m\u001b[35m\u25C67\u001b[0m",
  "\u2663_","\u2660_","\u2665_","\u25C6_",
  "__","XX"
};
*/
enum {  // default encoding for quit decision on smallest card etc
  kras,
  kr10,
  krko,
  krda,
  krbu,
  kr09,
  kr08,
  kr07,
  pias,
  pi10,
  piko,
  pida,
  pibu,
  pi09,
  pi08,
  pi07,
  heas,
  he10,
  heko,
  heda,
  hebu,
  he09,
  he08,
  he07,
  kaas,
  ka10,
  kako,
  kada,
  kabu,
  ka09,
  ka08,
  ka07
};

static int suitorder[DECKSIZE] = {
    kras, kr10, krko, krda, krbu, kr09, kr08, kr07, pias, pi10, piko,
    pida, pibu, pi09, pi08, pi07, heas, he10, heko, heda, hebu, he09,
    he08, he07, kaas, ka10, kako, kada, kabu, ka09, ka08, ka07};

// mapping trump to null ordering
static int trumporder[DECKSIZE] = {
    krbu, kr09, kras, kr10, krko, krda, kr08, kr07, pibu, pi09, pias,
    pi10, piko, pida, pi08, pi07, hebu, he09, heas, he10, heko, heda,
    he08, he07, kabu, ka09, kaas, ka10, kako, kada, ka08, ka07};

// eyes alias point value of cards wrt default order
static int suitval[DECKSIZE] = {11, 10, 4,  3,  2, 0,  0,  0, 11, 10, 4,
                                3,  2,  0,  0,  0, 11, 10, 4, 3,  2,  0,
                                0,  0,  11, 10, 4, 3,  2,  0, 0,  0};

// eyes alias point value of cards wrt default order
static int trumpval[DECKSIZE] = {11, 10, 4,  3,  20, 14, 0,  0,  11, 10, 4,
                                 3,  20, 14, 0,  0,  11, 10, 4,  3,  20, 14,
                                 0,  0,  11, 10, 4,  3,  20, 14, 0,  0};

static int Suit[DECKSIZE] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,
                             2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3};

static int Card[DECKSIZE] = {kras, kr10, krko, krda, krbu, kr09, kr08, kr07,
                             pias, pi10, piko, pida, pibu, pi09, pi08, pi07,
                             heas, he10, heko, heda, hebu, he09, he08, he07,
                             kaas, ka10, kako, kada, kabu, ka09, ka08, ka07};

#define VALUE(x) (((ONE << x) & trumpmask) ? trumpval[x] : suitval[x])
#define COST(a, b, c, d) (VALUE(a) + VALUE(b) + VALUE(c) + VALUE(d))

// for each color bit mask
static card mask[SUITCOUNT] = {ZERO, ZERO, ZERO, ZERO};
// static card mask[SUITCOUNT] = {ALL << 0,ALL << SUITSIZE, ALL << 2*SUITSIZE,
// ALL << 3*SUITSIZE};

// bitvalue of cards
static card CA = (ONE << kras), CK = (ONE << krko), CQ = (ONE << krda),
            CJ = (ONE << krbu), CT = (ONE << kr10), C9 = (ONE << kr09),
            C8 = (ONE << kr08), C7 = (ONE << kr07), SA = (ONE << pias),
            SK = (ONE << piko), SQ = (ONE << pida), SJ = (ONE << pibu),
            ST = (ONE << pi10), S9 = (ONE << pi09), S8 = (ONE << pi08),
            S7 = (ONE << pi07), HA = (ONE << heas), HK = (ONE << heko),
            HQ = (ONE << heda), HJ = (ONE << hebu), HT = (ONE << he10),
            H9 = (ONE << he09), H8 = (ONE << he08), H7 = (ONE << he07),
            DA = (ONE << kaas), DK = (ONE << kako), DQ = (ONE << kada),
            DJ = (ONE << kabu), DT = (ONE << ka10), D9 = (ONE << ka09),
            D8 = (ONE << ka08), D7 = (ONE << ka07);

// bitvector of card groups
static const card ACES =
    ((ONE << kras) | (ONE << pias) | (ONE << heas) | (ONE << kaas));
static const card KINGS =
    ((ONE << krko) | (ONE << piko) | (ONE << heko) | (ONE << kako));
static const card QUEENS =
    ((ONE << krda) | (ONE << pida) | (ONE << heda) | (ONE << kada));
static const card JACKS =
    ((ONE << krbu) | (ONE << pibu) | (ONE << hebu) | (ONE << kabu));
static const card TENS =
    ((ONE << kr10) | (ONE << pi10) | (ONE << he10) | (ONE << ka10));
static const card NINES =
    ((ONE << kr09) | (ONE << pi09) | (ONE << he09) | (ONE << ka09));
static const card EIGHTS =
    ((ONE << kr08) | (ONE << pi08) | (ONE << he08) | (ONE << ka08));
static const card SEVENS =
    ((ONE << kr07) | (ONE << pi07) | (ONE << he07) | (ONE << ka07));

#define DIAMONDS                                                   \
  ((ONE << ka07) | (ONE << ka08) | (ONE << ka09) | (ONE << ka10) | \
   (ONE << kabu) | (ONE << kada) | (ONE << kako) | (ONE << kaas))
#define HEARTS                                                     \
  ((ONE << he07) | (ONE << he08) | (ONE << he09) | (ONE << he10) | \
   (ONE << hebu) | (ONE << heda) | (ONE << heko) | (ONE << heas))
#define SPADES                                                     \
  ((ONE << pi07) | (ONE << pi08) | (ONE << pi09) | (ONE << pi10) | \
   (ONE << pibu) | (ONE << pida) | (ONE << piko) | (ONE << pias))
#define CLUBS                                                      \
  ((ONE << kr07) | (ONE << kr08) | (ONE << kr09) | (ONE << kr10) | \
   (ONE << krbu) | (ONE << krda) | (ONE << krko) | (ONE << kras))

static const card abovesuit[CARDTYPES] = {ACES,  TENS,  KINGS,  QUEENS,
                                          JACKS, NINES, EIGHTS, SEVENS};
static const card abovetrump[SUITSIZE] = {JACKS, NINES,  ACES,   TENS,
                                          KINGS, QUEENS, EIGHTS, SEVENS};

#endif
