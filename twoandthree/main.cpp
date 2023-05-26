#include "./two/part/main.h"
#include "./three/part/main.h"

using namespace std;

void LL() {
    GrammarLL *grammarLL = eliminate();
    creatTable(grammarLL);
    judgement(grammarLL);
}

void LR() {
    GrammarLR *grammarLR = livePrefixes();
    action(grammarLR);
    judgement(grammarLR);
}

int main() {
    cout<<"请选择进行的实验并输入相应的编号"<<endl;
    cout<<"1.LL"<<endl;
    cout<<"2.LR"<<endl;
    cout<<"在此输入:";
    int i;
    cin>>i;
    if (i == 1){
        LL();
    }else if(i == 2){
        LR();
    }
}