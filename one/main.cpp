#include <bits/stdc++.h>
#include "part/main.h"

using namespace std;
using namespace base_class;

int main() {
    char c;
    do {
        Graph nfa = toMakeNFA();
        Graph dfa = toMakeDFA(nfa);
        test(dfa);
        cout<<"按任意键继续，#键退出"<<endl;
        cin>>c;
    } while (c!='#');
}