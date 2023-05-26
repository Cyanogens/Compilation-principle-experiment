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

//��ձհ�,��result���ϵĿձհ�������ӣ��������ӵ�result����
void closure(vector<Project> &result, GrammarLR* grammarLR) {

    int elementsKey, express, projectPoint;
    vector<Project> temp;

    for (int i = 0; i < result.size(); i++) {

        //�Ƚ���Ҫ��ֵ��ֵ
        elementsKey = result[i].elemKey;
        express = result[i].expressIndex;
        projectPoint = result[i].projectPoint;
        //���ж��ǲ����ڽ�β
        if (projectPoint == grammarLR->elements[elementsKey].expressionOfSet[express].expression.size()) continue;

        int newElemKey = grammarLR->elements[elementsKey].expressionOfSet[express][projectPoint];
        //���ս�������ù�
        if (grammarLR->elements[newElemKey].isFinally) continue;

        //�����ս����
        //�Ƚ�������ֵ���������������ϲ�
        temp.clear();
        for (int j = 0; j < grammarLR->elements[newElemKey].expressionOfSet.size(); j++) {
            temp.emplace_back(newElemKey, j, 0);
        }
        connectVector(result, temp);
    }
}

//smove���㣬�õ�����һ��״̬����,��input״̬����elem[elemIndex]��õ���DFA��Ŀ����
void smove(DFAState &input, int elemIndex, vector<Project> &result, GrammarLR* grammarLR) {
    //������һ��
    result.clear();
    int elementsKey, express, projectPoint;

    for (auto &project : input.projects) {

        //�Ƚ���Ҫ��ֵ��ֵ
        elementsKey = project.elemKey;
        express = project.expressIndex;
        projectPoint = project.projectPoint;

        //���ж��ǲ����ڽ�β
        if (projectPoint == grammarLR->elements[elementsKey].expressionOfSet[express].expression.size()) continue;

        //�����ڽ�β�����ǲ��Ƕ�Ӧ���ַ�
        if (grammarLR->elements[elementsKey].expressionOfSet[express][projectPoint] == elemIndex) {
            result.emplace_back(elementsKey, express, projectPoint + 1);
        }
    }
}
//����DFA
void createDfaStates(GrammarLR* grammarLR) {

    //��ʼ����,��Ŀ�ĵ���±�����һ��
    //��ĿA->.abb ���λ��Ϊ0
    vector<Project> projects;
    projects.emplace_back(grammarLR->findName(grammarLR->elements[0].name + "'"), 0, 0);
    closure(projects,grammarLR);

    grammarLR->states.emplace_back(projects, 0, grammarLR->elements.size());


    //��ʼ����
    //S0��0�±꣬����������
    for (int stateIndex = 0; stateIndex < grammarLR->states.size(); stateIndex++) {

        //ÿһ��״̬��smove��Ҫ����һ�£������ÿһ����Ŀ���ϣ�����������е�Ԫ��
        for (int i = 0; i < grammarLR->states[stateIndex].projects.size(); i++) {

            Project temp = grammarLR->states[stateIndex].projects[i];

            //����һ�����ƽ�����û��smove��
            if (temp.projectPoint == grammarLR->elements[temp.elemKey].expressionOfSet[temp.expressIndex].expression.size()) {
                continue;
            }
            //.���������Ԫ��
            int elemIndex = grammarLR->elements[temp.elemKey].expressionOfSet[temp.expressIndex][temp.projectPoint];

            //���ж����Ԫ���ǲ����Ѿ�������,����-1�Ѿ�������
            if (grammarLR->states[stateIndex].next[elemIndex] != -1) {
                continue;
            }

            //����smove;
            smove(grammarLR->states[stateIndex], elemIndex, projects, grammarLR);

            //����ձհ�
            closure(projects, grammarLR);

            int countIndex = grammarLR->states.size();
            //�������״̬�ǲ����Ѿ����ֹ���
            for (int j = 0; j < grammarLR->states.size(); j++) {

                if (grammarLR->states[j] == projects) {
                    countIndex = j;
                    break;
                }
            }

            //��״̬����Ҫ��push
            if (countIndex == grammarLR->states.size()) {
                grammarLR->states.emplace_back(projects, countIndex, grammarLR->elements.size());
            }

            //��ԭ����״̬���е���
            grammarLR->states[stateIndex].next[elemIndex] = countIndex;
        }
    }
}
GrammarLR *livePrefixes() {
    GrammarLR *grammarLR = new GrammarLR();
    grammarLR->addElement("#", true);
    //����ع��ķ�
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
    //����DFA״̬��
    createDfaStates(grammarLR);
    return grammarLR;
}
