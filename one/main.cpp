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
        cout<<"�������������#���˳�"<<endl;
        cin>>c;
    } while (c!='#');
}