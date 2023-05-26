/**
 * �ʷ�����
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
    cout << "�����:";
    cin >> exp;
    cout << "���̣�";
    int p = dfa->startState;
    cout << p << "->";
    //���ݶ�����ת
    for (char chars : exp) {
        int c; //�ַ��������ַ������е�λ��
        //�ַ����ɱ�����
        if (!findAbChar(chars, dfa, c)) {
            cout << "ʧ��:" << chars << "�����ַ���������" << endl;
            return;
        }
        //���Ա�����
        p = dfa->skips[p][c];
        cout << p << "->";
        //�޷���ת
        if (p == MAX) {
            cout << "ʧ��:����" << chars << "��ת����" << endl;
            return;
        }
    }
    //ȫ���ַ���ȡ�ɹ������ܵ����յ�
    if (findS(dfa->endSet, p)) //�ƶ����յ㴦
        cout << "�ɹ�" << endl;
    else
        cout << "ʧ�ܣ�δ�����յ�" << endl;
}

void test(Graph dfa) {
    lexicalAnalysis(&dfa);
    cout << endl << "��ʼ��";
}