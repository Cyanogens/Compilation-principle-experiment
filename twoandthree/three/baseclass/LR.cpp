#include"LR.h"
#include <bits/stdc++.h>

#include <utility>

using namespace std;
using namespace lr_grammar;
using namespace public_tool;

//project的实现
Project::Project(int elemKey, int expressIndex, int point) {
    this->elemKey = elemKey;
    this->expressIndex = expressIndex;
    this->projectPoint = point;
}

bool Project::operator<(const Project &project) const {

    //this<project为true
    if (elemKey != project.elemKey){
        return elemKey<project.elemKey;
    }

    //这里说明elemKey相同
    if (expressIndex != project.expressIndex){
        return expressIndex < project.expressIndex;
    }

    //这里说明expressIndex相同
    if (projectPoint != project.projectPoint){
        return projectPoint < project.projectPoint;
    }

    //这里就说明两个相等了
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


//LR的特殊化
//表达式
ExpressLR::ExpressLR(int *expressions, int length) : Express(expressions, length) {}

ExpressLR::ExpressLR(const vector<int> &expression) : Express(expression) {}


//LR符号
ElemRight::ElemRight(string name, int key, bool isFinally) : Elem(move(name), key, isFinally) {}

bool ElemRight::addExpression(const ExpressLR &expression) {

    this->expressionOfSet.push_back(expression);
    return true;
}


//项目集合
//LR特有的
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
    //两个项目集合的数量不一样
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


//LR文法
//构造函数，重中之中,相当于主函数了
GrammarLR::GrammarLR() {
    vector<vector<string>> vectorGenrates;
    vector<string> inputs;
    string input;


    //解析各个表达式,第一个是非终结符号，后面的是产生式
    cout << "输入表达式" << endl;
    cout << "$...$表示一个符号,如$id$表示为id" << endl;
    cout << "$表示转义,如?$表示$" << endl;
    cout << "输入单独的#以结束输入" << endl;
    while (true) {
        cout<<"请输入表达式:";
        cin >> input;
        if (input == "#") {
            break;
        }
        inputs.push_back(input);
        input.clear();
    }

    for (auto & in : inputs) {
        vector<string> temp;
        //拆出输入输出
        parseProductions(in, temp);
        vectorGenrates.push_back(temp);
    }

    //添加输入添加到find表中,开始的非终结符就是length个
    for (int i = 0; i < inputs.size(); i++) {
        addElement(vectorGenrates[i][0], false);
    }

    //一个个解析产生式,会添加到find表中
    //添加所有终结符和非终结符
    vector<string> resultOfName;
    for (int i = 0; i < inputs.size(); i++) {
        //对genrates[i]进行分析
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
            //将所有的终结符与非终结符进行记录
            elements[findName(vectorGenrates[i][0])].addExpression(ExpressLR(exp));
        }
    }
}

//添加将name添加到表中，并且将在elem中生成对应的元素，添加到elems中
int GrammarLR::addElement(const string &name, bool isFinally) {
    this->nameOfKeys[name] = elements.size();
    ElemRight t(name, elements.size(), isFinally);
    elements.push_back(t);
    return elements.size() - 1;
}


