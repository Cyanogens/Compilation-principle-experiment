#ifndef MAIN_H
#define MAIN_H

#include "../baseclass/BaseClass.h"

using namespace base_class;

Graph toMakeNFA();
Graph toMakeDFA(Graph &nfa);
void test(Graph dfa);

#endif
