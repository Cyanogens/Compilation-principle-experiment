/**
 * 词法分析
 *
 * @author Cyanogen
 * @date 2023-03-20 23:25
 */
#include <bits/stdc++.h>
#include "../baseclass/BaseClass.h"

using namespace std;
using namespace base_class;

bool findS(StatesSet set_, int s) {
    for (int i = 0; i < set_.length; i++)
        if (set_.stateSet[i] == s) return 1;
    return 0;
}

bool findAbChar(char a, Graph *dfa, int &c) {
    for (int i = 0; i < dfa->absorbChar.size(); i++)
        if (dfa->absorbChar[i] == a) {
            c = i;
            return 1;
        }
    return 0;
}

void lexicalAnalysis(Graph *dfa) {
    string exp;
    cout << "输入词:";
    cin >> exp;
    cout << "过程：";
    int p = dfa->startState;
    cout << p << "->";
    //根据读入跳转
    for (char chars : exp) {
        int c; //字符在吸收字符数组中的位置
        //字符不可被接受
        if (!findAbChar(chars, dfa, c)) {
            cout << "失败:" << chars << "输入字符不被接受" << endl;
            return;
        }
        //可以被接受
        p = dfa->skips[p][c];
        cout << p << "->";
        //无法跳转
        if (p == MAX) {
            cout << "失败:吸收" << chars << "跳转出错" << endl;
            return;
        }
    }
    //全部字符读取成功：若能到达终点
    if (findS(dfa->endSet, p)) //移动到终点处
        cout << "成功" << endl;
    else
        cout << "失败：未到达终点" << endl;
}

void test(Graph dfa) {
    lexicalAnalysis(&dfa);
    cout << endl << "开始：";
}