/* Base*/

#ifndef __BASE
#define __BASE

#include <unistd.h>  // sleep

#include <ctime>     // for seed
#include <iostream>  // cout
#include <list>      // for lists
#include <random>    // for random generation
using namespace std;

#define D(x) cout << " " << #x << ": " << x << endl;

/* ADAPT TO THE GAME */
#define N_PLAYERS 2
#define N_ROUNDS 4
#define N_CARDS 10
#define N_SUITS 1
#define SIZE_HAND 4
#define MAX_SCORE N_ROUNDS
#define PRINTING 6
#define N_TEAMS 2
///////////////////////

#if N_CARDS <= 32
typedef unsigned int card;  // reprezents several cards (bit vector)
#define CTZ __builtin_ctz
#define POPCOUNT __builtin_popcount
#elif N_CARDS <= 64
typedef unsigned long long int card;
#define CTZ __builtin_ctzll
#define POPCOUNT __builtin_popcountll
#endif
extern card deck;  // set of all cards
#define ONE (card)1

static std::random_device rd;
static std::mt19937 mt{rd()};

/**
 * @brief prints card in bit vector representation
 *
 * @param c card to print
 */
void print_vector(const card &c);

/**
 * @brief transforms a set of cards into a list of every card in it
 *
 * @param x set of cards
 * @return list<card> enumeration of x
 */
list<card> set_cards(card x);

/**
 * @brief cards strictly higher than c
 *
 * @param c
 * @return card
 */
card higher(const card &c);

/**
 * @brief cards strictly lower than c
 *
 * @param c
 * @return card
 */
card lower(const card &c);

/**
 * @brief returns a list of card dealt according to the distribution
 *
 * @param deck cards to deal
 * @param distribution {2,3} will return a hand of 2 cards followed by a hand of
 * 3
 * @return list<card> hands dealt
 */
list<card> deal(card deck, const list<int> distribution, int seed);

#endif