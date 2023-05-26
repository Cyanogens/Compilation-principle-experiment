#ifndef THREE_MAIN_H
#define THREE_MAIN_H

#include "../baseclass/LR.h"

using namespace lr_grammar;

GrammarLR *livePrefixes();
void action(GrammarLR *grammarLr);
void judgement(GrammarLR *grammarLr);

#endif
