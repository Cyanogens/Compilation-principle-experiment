#include"LL.h"
#include <bits/stdc++.h>

using namespace std;
using namespace ll_grammar;
using namespace public_tool;

//LL符号
ElemLeft::ElemLeft(string name, int key, bool isFinally) : Elem(move(name),key, isFinally) {}


bool ElemLeft::extractLeftFactor(vector<int> &result, int &begin, int &count) {

    //排序
    sort(this->expressionOfSet.begin(), this->expressionOfSet.end(),[](const Express& a, const Express& b){

        int maxSize = a.expression.size()<b.expression.size() ? a.expression.size() : b.expression.size();

        for (int i = 0; i < maxSize; i++){
            if (a.expression[i] < b.expression[i]) {
                return true;
            }
            if (a.expression[i] > b.expression[i]){
                return false;
            }
        }
        return a.expression.size() < b.expression.size();
    });

    result.clear();
    begin = -1;
    //是有多少个元素有这个前缀
    count = 1;

    //判断是否有左因子
    for (int i = 0; i < this->expressionOfSet.size() - 1; i++) {
        if (expressionOfSet[i][0] == expressionOfSet[i + 1][0]) {
            begin = i;
            break;
        }
    }
    //没有进入if中，说明这个没有公共左因子了
    if (begin == -1) {
        return false;
    }

    //开始计算了,第一个前缀一定是最长的
    //计算左因子长度
    for (int i = begin; i < this->expressionOfSet.size() - 1; i++) {
        //统计有几个表达式有左因子
        if (expressionOfSet[i][0] == expressionOfSet[i + 1][0]) {
            count++;
        } else {
            break;
        }
    }

    //先将第一个压入栈
    result.push_back(expressionOfSet[begin][0]);

    for (int i = 1; i < this->expressionOfSet[begin].expression.size(); i++) {
        if (this->expressionOfSet[begin][i] == this->expressionOfSet[begin + count - 1][i]) {
            result.push_back(this->expressionOfSet[begin][i]);
        } else {
            break;
        }
    }
    return true;
}

bool ElemLeft::addExpression(const Express &expression) {
    this->expressionOfSet.push_back(expression);
    return true;
}

//添加将name添加到表中，并且将在elem中生成对应的元素，添加到elems中
int GrammarLL::addElement(const string &name, bool isFinally) {
    this->nameOfKeys[name] = elements.size();
    ElemLeft t(name, elements.size(), isFinally);
    elements.push_back(t);
    return elements.size() - 1;
}

//修改元素
void GrammarLL::updateElement(int elemIndex, int begin, int end, vector<int> &factory) {

    stringstream ss;
    ss << elements.size();
    int newIndex = addElement(elements[elemIndex].name + ss.str(), false);

    //先对新的式子添加表达式,将begin到end添加的
    //产生空串的只有第一个begin有这个倾向,别的没有这个倾向,将begin单独拿出来作为特例
    vector<int> t;
    t.clear();
    if (elements[elemIndex].expressionOfSet[begin].expression.size() == factory.size()) {
        t.push_back(-1);
    } else {
        for (int i = factory.size();
             i < elements[elemIndex].expressionOfSet[begin].expression.size(); i++) {
            t.push_back(elements[elemIndex].expressionOfSet[begin][i]);
        }
    }
    elements[newIndex].addExpression(t);

    for (int i = begin + 1; i < end; i++) {
        t.clear();
        for (int j = factory.size(); j < elements[elemIndex].expressionOfSet[i].expression.size(); j++) {
            t.push_back(elements[elemIndex].expressionOfSet[i][j]);
        }
        elements[newIndex].addExpression(t);
    }

    //修改老元素
    //在begin下面放下结果
    elements[elemIndex].expressionOfSet[begin].expression.clear();
    for (int &i : factory) {
        elements[elemIndex].expressionOfSet[begin].expression.push_back(i);
    }
    elements[elemIndex].expressionOfSet[begin].expression.push_back(newIndex);

    //后面的元素提前
    int moveDistance = end - begin;
    for (int i = end; i < elements[elemIndex].expressionOfSet.size(); i++) {
        elements[elemIndex].expressionOfSet[i - moveDistance + 1]
                = elements[elemIndex].expressionOfSet[i];
    }

    //清楚元素,共清楚moveDistance-1个
    for (int i = 0; i < moveDistance - 1; i++) {
        elements[elemIndex].expressionOfSet.pop_back();
    }
}

//LL文法
//构造函数，重中之中,相当于主函数了
/**
 * 构造函数
 */
GrammarLL::GrammarLL() {
    vector<vector<string>> vectorGenrates;
    vector<string> inputs;
    string input;

    //解析各个表达式,第一个是非终结符号，后面的是产生式
    cout << "输入表达式" << endl;
    cout << "$...$表示一个符号,如$id$表示为id" << endl;
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
            //字符转义
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
            elements[findName(vectorGenrates[i][0])].addExpression(Express(exp));
        }
    }
}