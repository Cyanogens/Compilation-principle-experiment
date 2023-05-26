#include"LR.h"
#include <bits/stdc++.h>

#include <utility>

using namespace std;
using namespace lr_grammar;
using namespace public_tool;

//project��ʵ��
Project::Project(int elemKey, int expressIndex, int point) {
    this->elemKey = elemKey;
    this->expressIndex = expressIndex;
    this->projectPoint = point;
}

bool Project::operator<(const Project &project) const {

    //this<projectΪtrue
    if (elemKey != project.elemKey){
        return elemKey<project.elemKey;
    }

    //����˵��elemKey��ͬ
    if (expressIndex != project.expressIndex){
        return expressIndex < project.expressIndex;
    }

    //����˵��expressIndex��ͬ
    if (projectPoint != project.projectPoint){
        return projectPoint < project.projectPoint;
    }

    //�����˵�����������
    return false;

}

bool Project::operator==(const Project &project) const {

    bool result = false;

    if (elemKey == project.elemKey && expressIndex == project.expressIndex
        && projectPoint == project.projectPoint) {
        result = true;
    }
    return result;
}

bool Project::operator!=(const Project &project) const {
    return !(*this == project);
}


//LR�����⻯
//���ʽ
ExpressLR::ExpressLR(int *expressions, int length) : Express(expressions, length) {}

ExpressLR::ExpressLR(const vector<int> &expression) : Express(expression) {}


//LR����
ElemRight::ElemRight(string name, int key, bool isFinally) : Elem(move(name), key, isFinally) {}

bool ElemRight::addExpression(const ExpressLR &expression) {

    this->expressionOfSet.push_back(expression);
    return true;
}


//��Ŀ����
//LR���е�
DFAState::DFAState(const vector<Project> &project, int index, int elemCount) {
    for (auto i : project) {
        this->projects.push_back(i);
    }
    sort(this->projects.begin(), this->projects.end());
    this->statesCount = index;

    this->next = new int[elemCount];
    this->elementCount = elemCount;
    for (int i = 0; i < elemCount; i++) {
        next[i] = -1;
    }
}

DFAState::DFAState(const DFAState &a) {
    for (auto project : a.projects) {
        this->projects.push_back(project);
    }
    this->statesCount = a.statesCount;

    this->next = new int[a.elementCount];
    this->elementCount = a.elementCount;
    for (int i = 0; i < elementCount; i++) {
        next[i] = a.next[i];
    }
}

bool DFAState::operator==(vector<Project> &b) {
    //������Ŀ���ϵ�������һ��
    if (this->projects.size() != b.size()) {
        return false;
    }
    bool flag = true;

    sort(b.begin(), b.end(), [=](Project &a, Project &b) {
        return a < b;
    });

    for (int i = 0; i < b.size(); i++) {
        if (projects[i] != b[i]) {
            flag = false;
            break;
        }
    }
    return flag;
}

DFAState::~DFAState() {
    delete[] this->next;
}


//LR�ķ�
//���캯��������֮��,�൱����������
GrammarLR::GrammarLR() {
    vector<vector<string>> vectorGenrates;
    vector<string> inputs;
    string input;


    //�����������ʽ,��һ���Ƿ��ս���ţ�������ǲ���ʽ
    cout << "������ʽ" << endl;
    cout << "$...$��ʾһ������,��$id$��ʾΪid" << endl;
    cout << "$��ʾת��,��?$��ʾ$" << endl;
    cout << "���뵥����#�Խ�������" << endl;
    while (true) {
        cout<<"��������ʽ:";
        cin >> input;
        if (input == "#") {
            break;
        }
        inputs.push_back(input);
        input.clear();
    }

    for (auto & in : inputs) {
        vector<string> temp;
        //����������
        parseProductions(in, temp);
        vectorGenrates.push_back(temp);
    }

    //���������ӵ�find����,��ʼ�ķ��ս������length��
    for (int i = 0; i < inputs.size(); i++) {
        addElement(vectorGenrates[i][0], false);
    }

    //һ������������ʽ,����ӵ�find����
    //��������ս���ͷ��ս��
    vector<string> resultOfName;
    for (int i = 0; i < inputs.size(); i++) {
        //��genrates[i]���з���
        for (int j = 1; j < vectorGenrates[i].size(); j++) {
            parseElem(vectorGenrates[i][j], resultOfName);
            vector<int> exp;
            for (auto &m : resultOfName) {
                int index = findName(m);
                if (index == -2) {
                    index = addElement(m, true);
                }
                exp.push_back(index);
            }
            //�����е��ս������ս�����м�¼
            elements[findName(vectorGenrates[i][0])].addExpression(ExpressLR(exp));
        }
    }
}

//��ӽ�name��ӵ����У����ҽ���elem�����ɶ�Ӧ��Ԫ�أ���ӵ�elems��
int GrammarLR::addElement(const string &name, bool isFinally) {
    this->nameOfKeys[name] = elements.size();
    ElemRight t(name, elements.size(), isFinally);
    elements.push_back(t);
    return elements.size() - 1;
}


