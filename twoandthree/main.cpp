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
    cout<<"��ѡ����е�ʵ�鲢������Ӧ�ı��"<<endl;
    cout<<"1.LL"<<endl;
    cout<<"2.LR"<<endl;
    cout<<"�ڴ�����:";
    int i;
    cin>>i;
    if (i == 1){
        LL();
    }else if(i == 2){
        LR();
    }
}