//
// Created by 彭湘粤 on 2023/5/15.
//
#include <bits/stdc++.h>
#include "../baseclass/BaseClass.h"

using namespace std;
using namespace base_class;

/**
 * 向状态集中添加元素
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
 * 求集合的空闭包 空跳转
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
    cout << "闭包:" << endl;
    for (int i = 0; i < Set.length; i++) {
        cout << Set.stateSet[i] ;
        if (i < Set.length - 1){
            cout<< ", ";
        }
    }
    cout<<endl;
}

//计算从当前状态集出发，经过a字符，能到的下一个状态集
void smoveTo(StatesSet Set, StatesSet &nextSet, int absorbChar, Graph &nfa) {
    //对每个状态都计算经过a得到的新状态
    for (int i = 0; i < Set.length; i++) {
        int nfa_i = Set.stateSet[i];  //在nfa中的状态
        if (nfa.skips[nfa_i][absorbChar] != MAX && nfa.skips[nfa_i][absorbChar] != -1) {
            addStoSet(nextSet, nfa.skips[nfa_i][absorbChar]);
        } //有跳转
    }
}

//判断当前状态集是否存在，若存在则返回新状态，若不存在则返回0
int ifExist(StatesSet *Sets, StatesSet Set, int setsLength) {
    //找到长度相等的进行比对
    int T; //判断是否有相同的状态集
    for (int i = 0; i < setsLength; i++) {
        if (Sets[i].length == Set.length) {
            T = 1; //先假设该集合是
            for (int j = 0; j < Set.length; j++) {
                int t = 0; //判断是否找到相同元素
                for (int k = 0; k < Sets[i].length; k++) {
                    if (Set.stateSet[j] == Sets[i].stateSet[k]) {
                        t = 1;
                        break;
                    }
                }
                if (t == 0) {
                    T = 0;
                    break;
                } //找不到，该集合不是
            }
            if (T == 1) return i;
        }
    }
    return 0;
}

/**
 * NFA转为DFA
 * @param nfa
 * @param dfa
 * @param sets
 */
void DFA(Graph &nfa, Graph &dfa, StatesSet *sets) {
    for (int i = 0; i < dfa.stateSize; i++) {
        //计算当前状态的跳转状态集 c为吸收字符
        for (int c = 0; c < dfa.absorbChar.size(); c++) {
            StatesSet nextSet;
            smoveTo(sets[i], nextSet, c, nfa);//计算Smove(j,char)
            if (nextSet.length == 0) {
                continue;
            } //该状态不吸收该字符
            emptyClosure(nextSet, nfa.emptySkips);//计算Smove的空闭包
            //判断该状态集是否已经存在,如果不存在则作为新状态集加入dfa
            int S = ifExist(sets, nextSet, dfa.stateSize);
            if (!S) {
                S = dfa.stateSize;
                sets[S] = nextSet;
                dfa.stateSize++;
            }
            //在dfa上添加当前状态吸收字符的跳转
            dfa.skips[i][c] = S;
        }
    }
}
//判断集合中是否有某个元素
bool find(StatesSet sets, int s) {
    for (int i = 0; i < sets.length; i++) {
        if (sets.stateSet[i] == s) return 1;
    }
    return 0;
}

Graph DFA_(Graph &nfa) {
    //初始化dfa
    auto *dfa = new Graph(nfa.absorbChar);
    dfa->type = "DFA";
    StatesSet sets[MAX];//新旧状态表
    //先求起始状态的空闭包
    StatesSet beginSet;
    addStoSet(beginSet, nfa.startState);
    emptyClosure(beginSet, nfa.emptySkips);
    //将起始状态集作为DFA的起始状态加入
    dfa->startState = 0;
    //将起始状态集加入状态集与新状态的对照表中
    sets[dfa->stateSize] = beginSet;
    dfa->stateSize++;
    DFA(nfa, *dfa, sets);
    //找到dfa的所有终点状态
    int end_s = nfa.endState;
    for (int i = 0; i < dfa->stateSize; i++) {
        if (find(sets[i], end_s)) {
            addStoSet(dfa->endSet, i);
        }
    }
    return *dfa;
}

//将终点集根据吸收的字符划分
bool crackEnd(Graph &dfa, int *newStates, int &findNum) {
    StatesSet new_end_sets;
    //将终态分出:构造nfa到dfa，每个状态只吸收一个符号,找到每个符号对应的终态集，将其标为相同的状态
    if (dfa.endSet.length == 1) {
        newStates[(dfa.endSet.stateSet[0])] = dfa.endSet.stateSet[0];
        findNum++;
        addStoSet(new_end_sets, dfa.endSet.stateSet[0]);
    } else {
        int not_find_num = dfa.endSet.length; //未找到的元素
        for (int c = 0; c < dfa.absorbChar.size(); c++) {
            int T = MAX;  //表示还未找到第一个终态
            for (int i = dfa.stateSize - 1; i >= 0; i--) { //从后往前找
                if (find(dfa.endSet, dfa.skips[i][c])) { //在接受该字符的跳转里找到终态
                    if (T == MAX) {
                        T = dfa.skips[i][c];
                        addStoSet(new_end_sets, T);
                    }
                    newStates[(dfa.skips[i][c])] = T; //用第一个发现的终态 作为该吸收的终态
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

//分裂，分裂成功返回1，不可再分返回0
bool crack(Graph &dfa, int *newStates, int &findNum) {
    for (int i = 0; i < dfa.stateSize; i++) {
        if (newStates[i] != MAX) continue;
        for (int c = 0; c < dfa.absorbChar.size(); c++) {
            int T = 1;//表示是否可以区分
            for (int j = 0; j < dfa.stateSize; j++) {
                if (i == j) continue;
                if (newStates[(dfa.skips[i][c])] == newStates[(dfa.skips[j][c])]) { //跳转的结果在一个集合
                    T = 0;
                    break;
                }
            }
            //可再分
            if (T == 1) {
                newStates[i] = i;
                findNum++;
                return 1;
            }
        }
    }
    return 0;//表示不可再分
}
//判断数组前n个是否出现过当前字符
bool findInt(const int *array, int n, int s) {
    for (int i = 0; i < n; i++) {
        if (array[i] == s) return true;
        return false;
    }
}

//查找旧状态为s的新状态
int sInArray(const int *array, int s, int length) {
    for (int i = 0; i < length; i++) {
        if (array[i] == s) return i;
    }
}

//整理合并后的dfa
Graph *combine(const Graph& dfa, int *old_states) {
    //初始化dfa
    auto *min_dfa = new Graph(dfa.absorbChar);
    min_dfa->type = "DFA";
    int new_states[MAX];
    //将没有出现过的状态加入
    for (int i = 0; i < dfa.stateSize; i++) {
        if (findInt(old_states, i, old_states[i])) continue;//判断之前是否出现过
        for (int c = 0; c < dfa.absorbChar.size(); c++) {
            min_dfa->skips[min_dfa->stateSize][c] = dfa.skips[i][c];  //复制进去
        }
        new_states[min_dfa->stateSize] = old_states[i];//保存老状态
        min_dfa->stateSize++;
    }
    //将所有状态调整
    for (int i = 0; i < min_dfa->stateSize; i++) {
        for (int c = 0; c < min_dfa->absorbChar.size(); c++)
            min_dfa->skips[i][c] = sInArray(new_states, min_dfa->skips[i][c], min_dfa->stateSize);
    }
    //记录起点
    min_dfa->startState = sInArray(new_states, dfa.startState, min_dfa->stateSize);
    //终结状态作调整
    for (int i = 0; i < dfa.endSet.length; i++) {
        addStoSet(min_dfa->endSet, sInArray(new_states, dfa.endSet.stateSet[i], min_dfa->stateSize));
    }
    return min_dfa;
}

//最简化DFA
Graph minDFA(Graph dfa) {
    //新状态
    int find_num = 0; //确定新状态的状态数
    int new_states[MAX];
    for (int i = 0; i < dfa.stateSize; i++) {
        new_states[i] = MAX;
    }
    //将终态分出:构造nfa到dfa，每个状态只吸收一个符号,找到每个符号对应的终态集，将其标位相同的状态
    crackEnd(dfa, new_states, find_num);
    //将每个可区分的状态区分出来 条件：未找完或者还可以继续区分
    while (find_num != dfa.stateSize && crack(dfa, new_states, find_num)) { ; }
    //将剩余状态划分为一个状态
    if (find_num != dfa.stateSize) {
        int T = MAX;
        for (int i = 0; i < dfa.stateSize; i++) {
            if (new_states[i] != MAX) continue; //已经找到的
            if (T == MAX) T = i;
            new_states[i] = T;
        }
    }
    //将新旧状态不同的 修改符号
    for (int i = 0; i < dfa.stateSize; i++) {
        if (i == new_states[i]) continue;
        for (int j = 0; j < dfa.stateSize; j++) {
            for (int c = 0; c < dfa.absorbChar.size(); c++) {
                if (dfa.skips[j][c] == i) dfa.skips[j][c] = new_states[i];
            }
        }
    }
    dfa.startState = new_states[dfa.startState];//将改变的起点记录下来
    Graph *p =combine(dfa, new_states);
    return *p;
}

Graph toMakeDFA(Graph &nfa) {
    //由NFA构造DFA
    cout << endl << "由NFA转为DFA" << endl;
    Graph dfa = DFA_(nfa);
    Graph min_dfa = minDFA(dfa);
    min_dfa.visitGraph();
    return dfa;
}

