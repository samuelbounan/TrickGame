// #ifdef RULES_CONTREE
#ifndef __CONTREE
#define __CONTREE

#include <algorithm>
#include <iterator>

#include "../rules.h"

void sort_high(card *hand, int idx, const card &suit);
void unsort_high(card *hand, int idx, const card &suit);

#endif
// #endif