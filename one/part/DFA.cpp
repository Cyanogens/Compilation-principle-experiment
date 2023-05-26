//
// Created by ������ on 2023/5/15.
//
#include <bits/stdc++.h>
#include "../baseclass/BaseClass.h"

using namespace std;
using namespace base_class;

/**
 * ��״̬�������Ԫ��
 * @param Set
 * @param s
 * @return
 */
bool addStoSet(StatesSet &Set, int s) {
    for (int i = 0; i < Set.length; i++) {
        if (Set.stateSet[i] == s) return false;
    }
    Set.stateSet[Set.length] = s;
    Set.length++;
    return true;
}

/**
 * �󼯺ϵĿձհ� ����ת
 * @param Set
 * @param emptySkips
 */
void emptyClosure(StatesSet &Set, int emptySkips[][MAX]) {
    for (int i = 0; i < Set.length; i++) {
        int p = Set.stateSet[i];
        for (int j = 0; j < emptySkips[p][0]; j++) {
            addStoSet(Set, emptySkips[p][j + 1]);
        }
    }
    cout << "�հ�:" << endl;
    for (int i = 0; i < Set.length; i++) {
        cout << Set.stateSet[i] ;
        if (i < Set.length - 1){
            cout<< ", ";
        }
    }
    cout<<endl;
}

//����ӵ�ǰ״̬������������a�ַ����ܵ�����һ��״̬��
void smoveTo(StatesSet Set, StatesSet &nextSet, int absorbChar, Graph &nfa) {
    //��ÿ��״̬�����㾭��a�õ�����״̬
    for (int i = 0; i < Set.length; i++) {
        int nfa_i = Set.stateSet[i];  //��nfa�е�״̬
        if (nfa.skips[nfa_i][absorbChar] != MAX && nfa.skips[nfa_i][absorbChar] != -1) {
            addStoSet(nextSet, nfa.skips[nfa_i][absorbChar]);
        } //����ת
    }
}

//�жϵ�ǰ״̬���Ƿ���ڣ��������򷵻���״̬�����������򷵻�0
int ifExist(StatesSet *Sets, StatesSet Set, int setsLength) {
    //�ҵ�������ȵĽ��бȶ�
    int T; //�ж��Ƿ�����ͬ��״̬��
    for (int i = 0; i < setsLength; i++) {
        if (Sets[i].length == Set.length) {
            T = 1; //�ȼ���ü�����
            for (int j = 0; j < Set.length; j++) {
                int t = 0; //�ж��Ƿ��ҵ���ͬԪ��
                for (int k = 0; k < Sets[i].length; k++) {
                    if (Set.stateSet[j] == Sets[i].stateSet[k]) {
                        t = 1;
                        break;
                    }
                }
                if (t == 0) {
                    T = 0;
                    break;
                } //�Ҳ������ü��ϲ���
            }
            if (T == 1) return i;
        }
    }
    return 0;
}

/**
 * NFAתΪDFA
 * @param nfa
 * @param dfa
 * @param sets
 */
void DFA(Graph &nfa, Graph &dfa, StatesSet *sets) {
    for (int i = 0; i < dfa.stateSize; i++) {
        //���㵱ǰ״̬����ת״̬�� cΪ�����ַ�
        for (int c = 0; c < dfa.absorbChar.size(); c++) {
            StatesSet nextSet;
            smoveTo(sets[i], nextSet, c, nfa);//����Smove(j,char)
            if (nextSet.length == 0) {
                continue;
            } //��״̬�����ո��ַ�
            emptyClosure(nextSet, nfa.emptySkips);//����Smove�Ŀձհ�
            //�жϸ�״̬���Ƿ��Ѿ�����,�������������Ϊ��״̬������dfa
            int S = ifExist(sets, nextSet, dfa.stateSize);
            if (!S) {
                S = dfa.stateSize;
                sets[S] = nextSet;
                dfa.stateSize++;
            }
            //��dfa����ӵ�ǰ״̬�����ַ�����ת
            dfa.skips[i][c] = S;
        }
    }
}
//�жϼ������Ƿ���ĳ��Ԫ��
bool find(StatesSet sets, int s) {
    for (int i = 0; i < sets.length; i++) {
        if (sets.stateSet[i] == s) return 1;
    }
    return 0;
}

Graph DFA_(Graph &nfa) {
    //��ʼ��dfa
    auto *dfa = new Graph(nfa.absorbChar);
    dfa->type = "DFA";
    StatesSet sets[MAX];//�¾�״̬��
    //������ʼ״̬�Ŀձհ�
    StatesSet beginSet;
    addStoSet(beginSet, nfa.startState);
    emptyClosure(beginSet, nfa.emptySkips);
    //����ʼ״̬����ΪDFA����ʼ״̬����
    dfa->startState = 0;
    //����ʼ״̬������״̬������״̬�Ķ��ձ���
    sets[dfa->stateSize] = beginSet;
    dfa->stateSize++;
    DFA(nfa, *dfa, sets);
    //�ҵ�dfa�������յ�״̬
    int end_s = nfa.endState;
    for (int i = 0; i < dfa->stateSize; i++) {
        if (find(sets[i], end_s)) {
            addStoSet(dfa->endSet, i);
        }
    }
    return *dfa;
}

//���յ㼯�������յ��ַ�����
bool crackEnd(Graph &dfa, int *newStates, int &findNum) {
    StatesSet new_end_sets;
    //����̬�ֳ�:����nfa��dfa��ÿ��״ֻ̬����һ������,�ҵ�ÿ�����Ŷ�Ӧ����̬���������Ϊ��ͬ��״̬
    if (dfa.endSet.length == 1) {
        newStates[(dfa.endSet.stateSet[0])] = dfa.endSet.stateSet[0];
        findNum++;
        addStoSet(new_end_sets, dfa.endSet.stateSet[0]);
    } else {
        int not_find_num = dfa.endSet.length; //δ�ҵ���Ԫ��
        for (int c = 0; c < dfa.absorbChar.size(); c++) {
            int T = MAX;  //��ʾ��δ�ҵ���һ����̬
            for (int i = dfa.stateSize - 1; i >= 0; i--) { //�Ӻ���ǰ��
                if (find(dfa.endSet, dfa.skips[i][c])) { //�ڽ��ܸ��ַ�����ת���ҵ���̬
                    if (T == MAX) {
                        T = dfa.skips[i][c];
                        addStoSet(new_end_sets, T);
                    }
                    newStates[(dfa.skips[i][c])] = T; //�õ�һ�����ֵ���̬ ��Ϊ�����յ���̬
                    not_find_num--;
                    findNum++;
                }
                if (not_find_num == 0) break;
            }
            if (not_find_num == 0) break;
        }
    }
    dfa.endSet = new_end_sets;
}

//���ѣ����ѳɹ�����1�������ٷַ���0
bool crack(Graph &dfa, int *newStates, int &findNum) {
    for (int i = 0; i < dfa.stateSize; i++) {
        if (newStates[i] != MAX) continue;
        for (int c = 0; c < dfa.absorbChar.size(); c++) {
            int T = 1;//��ʾ�Ƿ��������
            for (int j = 0; j < dfa.stateSize; j++) {
                if (i == j) continue;
                if (newStates[(dfa.skips[i][c])] == newStates[(dfa.skips[j][c])]) { //��ת�Ľ����һ������
                    T = 0;
                    break;
                }
            }
            //���ٷ�
            if (T == 1) {
                newStates[i] = i;
                findNum++;
                return 1;
            }
        }
    }
    return 0;//��ʾ�����ٷ�
}
//�ж�����ǰn���Ƿ���ֹ���ǰ�ַ�
bool findInt(const int *array, int n, int s) {
    for (int i = 0; i < n; i++) {
        if (array[i] == s) return true;
        return false;
    }
}

//���Ҿ�״̬Ϊs����״̬
int sInArray(const int *array, int s, int length) {
    for (int i = 0; i < length; i++) {
        if (array[i] == s) return i;
    }
}

//����ϲ����dfa
Graph *combine(const Graph& dfa, int *old_states) {
    //��ʼ��dfa
    auto *min_dfa = new Graph(dfa.absorbChar);
    min_dfa->type = "DFA";
    int new_states[MAX];
    //��û�г��ֹ���״̬����
    for (int i = 0; i < dfa.stateSize; i++) {
        if (findInt(old_states, i, old_states[i])) continue;//�ж�֮ǰ�Ƿ���ֹ�
        for (int c = 0; c < dfa.absorbChar.size(); c++) {
            min_dfa->skips[min_dfa->stateSize][c] = dfa.skips[i][c];  //���ƽ�ȥ
        }
        new_states[min_dfa->stateSize] = old_states[i];//������״̬
        min_dfa->stateSize++;
    }
    //������״̬����
    for (int i = 0; i < min_dfa->stateSize; i++) {
        for (int c = 0; c < min_dfa->absorbChar.size(); c++)
            min_dfa->skips[i][c] = sInArray(new_states, min_dfa->skips[i][c], min_dfa->stateSize);
    }
    //��¼���
    min_dfa->startState = sInArray(new_states, dfa.startState, min_dfa->stateSize);
    //�ս�״̬������
    for (int i = 0; i < dfa.endSet.length; i++) {
        addStoSet(min_dfa->endSet, sInArray(new_states, dfa.endSet.stateSet[i], min_dfa->stateSize));
    }
    return min_dfa;
}

//���DFA
Graph minDFA(Graph dfa) {
    //��״̬
    int find_num = 0; //ȷ����״̬��״̬��
    int new_states[MAX];
    for (int i = 0; i < dfa.stateSize; i++) {
        new_states[i] = MAX;
    }
    //����̬�ֳ�:����nfa��dfa��ÿ��״ֻ̬����һ������,�ҵ�ÿ�����Ŷ�Ӧ����̬���������λ��ͬ��״̬
    crackEnd(dfa, new_states, find_num);
    //��ÿ�������ֵ�״̬���ֳ��� ������δ������߻����Լ�������
    while (find_num != dfa.stateSize && crack(dfa, new_states, find_num)) { ; }
    //��ʣ��״̬����Ϊһ��״̬
    if (find_num != dfa.stateSize) {
        int T = MAX;
        for (int i = 0; i < dfa.stateSize; i++) {
            if (new_states[i] != MAX) continue; //�Ѿ��ҵ���
            if (T == MAX) T = i;
            new_states[i] = T;
        }
    }
    //���¾�״̬��ͬ�� �޸ķ���
    for (int i = 0; i < dfa.stateSize; i++) {
        if (i == new_states[i]) continue;
        for (int j = 0; j < dfa.stateSize; j++) {
            for (int c = 0; c < dfa.absorbChar.size(); c++) {
                if (dfa.skips[j][c] == i) dfa.skips[j][c] = new_states[i];
            }
        }
    }
    dfa.startState = new_states[dfa.startState];//���ı������¼����
    Graph *p =combine(dfa, new_states);
    return *p;
}

Graph toMakeDFA(Graph &nfa) {
    //��NFA����DFA
    cout << endl << "��NFAתΪDFA" << endl;
    Graph dfa = DFA_(nfa);
    Graph min_dfa = minDFA(dfa);
    min_dfa.visitGraph();
    return dfa;
}

