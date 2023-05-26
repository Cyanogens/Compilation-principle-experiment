#include <bits/stdc++.h>
#include "../baseclass/LR.h"

using namespace std;
using namespace lr_grammar;
using namespace base_grammar;
using namespace public_tool;


bool isInVector(const vector<Project>& a, const Project& b){
    bool result = false;
    for (int i = 0; i < a.size(); i++){
        if (a[i] == b){
            result = true;
            break;
        }
    }
    return result;
}

void connectVector(vector<Project> &a, const vector<Project> &b) {
    for (int i = 0; i < b.size(); i++) {
        if (!isInVector(a, b[i])) {
            a.push_back(b[i]);
        }
    }
}

//求空闭包,求result集合的空闭包，会添加，最后会增加到result里面
void closure(vector<Project> &result, GrammarLR* grammarLR) {

    int elementsKey, express, projectPoint;
    vector<Project> temp;

    for (int i = 0; i < result.size(); i++) {

        //先将需要的值赋值
        elementsKey = result[i].elemKey;
        express = result[i].expressIndex;
        projectPoint = result[i].projectPoint;
        //先判断是不是在结尾
        if (projectPoint == grammarLR->elements[elementsKey].expressionOfSet[express].expression.size()) continue;

        int newElemKey = grammarLR->elements[elementsKey].expressionOfSet[express][projectPoint];
        //是终结符，不用管
        if (grammarLR->elements[newElemKey].isFinally) continue;

        //不是终结符号
        //先将他给赋值过来，后来继续合并
        temp.clear();
        for (int j = 0; j < grammarLR->elements[newElemKey].expressionOfSet.size(); j++) {
            temp.emplace_back(newElemKey, j, 0);
        }
        connectVector(result, temp);
    }
}

//smove运算，得到的是一个状态集合,求input状态经过elem[elemIndex]后得到的DFA项目集合
void smove(DFAState &input, int elemIndex, vector<Project> &result, GrammarLR* grammarLR) {
    //结果清除一下
    result.clear();
    int elementsKey, express, projectPoint;

    for (auto &project : input.projects) {

        //先将需要的值赋值
        elementsKey = project.elemKey;
        express = project.expressIndex;
        projectPoint = project.projectPoint;

        //先判断是不是在结尾
        if (projectPoint == grammarLR->elements[elementsKey].expressionOfSet[express].expression.size()) continue;

        //不是在结尾看看是不是对应的字符
        if (grammarLR->elements[elementsKey].expressionOfSet[express][projectPoint] == elemIndex) {
            result.emplace_back(elementsKey, express, projectPoint + 1);
        }
    }
}
//构造DFA
void createDfaStates(GrammarLR* grammarLR) {

    //开始计算,项目的点的下标是下一个
    //项目A->.abb 点的位置为0
    vector<Project> projects;
    projects.emplace_back(grammarLR->findName(grammarLR->elements[0].name + "'"), 0, 0);
    closure(projects,grammarLR);

    grammarLR->states.emplace_back(projects, 0, grammarLR->elements.size());


    //开始计算
    //S0是0下标，持续增长的
    for (int stateIndex = 0; stateIndex < grammarLR->states.size(); stateIndex++) {

        //每一个状态的smove都要计算一下，针对是每一个项目集合，不用针对所有的元素
        for (int i = 0; i < grammarLR->states[stateIndex].projects.size(); i++) {

            Project temp = grammarLR->states[stateIndex].projects[i];

            //这是一个可移进后面没有smove了
            if (temp.projectPoint == grammarLR->elements[temp.elemKey].expressionOfSet[temp.expressIndex].expression.size()) {
                continue;
            }
            //.后面的数的元素
            int elemIndex = grammarLR->elements[temp.elemKey].expressionOfSet[temp.expressIndex][temp.projectPoint];

            //先判断这个元素是不是已经计算了,不是-1已经计算了
            if (grammarLR->states[stateIndex].next[elemIndex] != -1) {
                continue;
            }

            //计算smove;
            smove(grammarLR->states[stateIndex], elemIndex, projects, grammarLR);

            //计算空闭包
            closure(projects, grammarLR);

            int countIndex = grammarLR->states.size();
            //看看这个状态是不是已经出现过了
            for (int j = 0; j < grammarLR->states.size(); j++) {

                if (grammarLR->states[j] == projects) {
                    countIndex = j;
                    break;
                }
            }

            //新状态，需要被push
            if (countIndex == grammarLR->states.size()) {
                grammarLR->states.emplace_back(projects, countIndex, grammarLR->elements.size());
            }

            //给原来的状态进行调整
            grammarLR->states[stateIndex].next[elemIndex] = countIndex;
        }
    }
}
GrammarLR *livePrefixes() {
    GrammarLR *grammarLR = new GrammarLR();
    grammarLR->addElement("#", true);
    //添加拓广文法
    string name = grammarLR->elements[0].name + "'";
    int index = grammarLR->addElement(name, false);
    vector<int> exp;
    exp.push_back(0);
    grammarLR->elements[index].addExpression(exp);

    int num=1;
    for (int i = 0; i < grammarLR->elements.size(); i++) {

        if (!grammarLR->elements[i].isFinally) {
            cout << grammarLR->elements[i].name << "->";
            for (int j = 0; j < grammarLR->elements[i].expressionOfSet.size(); j++) {
                for (int m = 0; m < grammarLR->elements[i].expressionOfSet[j].expression.size(); m++) {
                    if (grammarLR->elements[i].expressionOfSet[j].expression[m] != -1) {
                        cout << grammarLR->elements[grammarLR->elements[i].expressionOfSet[j].expression[m]].name;
                    } else {
                        cout << "@";
                    }
                }
                if (i < grammarLR->elements.size() - 1) {
                    cout << "\t" << "(" << num++ << ")" << endl;
                }
                if (j < grammarLR->elements[i].expressionOfSet.size() - 1)
                    cout << " | ";
            }
            cout << endl;
        }
    }
    //生成DFA状态表
    createDfaStates(grammarLR);
    return grammarLR;
}
