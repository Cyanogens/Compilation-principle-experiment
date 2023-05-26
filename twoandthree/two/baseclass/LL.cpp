#include"LL.h"
#include <bits/stdc++.h>

using namespace std;
using namespace ll_grammar;
using namespace public_tool;

//LL����
ElemLeft::ElemLeft(string name, int key, bool isFinally) : Elem(move(name),key, isFinally) {}


bool ElemLeft::extractLeftFactor(vector<int> &result, int &begin, int &count) {

    //����
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
    //���ж��ٸ�Ԫ�������ǰ׺
    count = 1;

    //�ж��Ƿ���������
    for (int i = 0; i < this->expressionOfSet.size() - 1; i++) {
        if (expressionOfSet[i][0] == expressionOfSet[i + 1][0]) {
            begin = i;
            break;
        }
    }
    //û�н���if�У�˵�����û�й�����������
    if (begin == -1) {
        return false;
    }

    //��ʼ������,��һ��ǰ׺һ�������
    //���������ӳ���
    for (int i = begin; i < this->expressionOfSet.size() - 1; i++) {
        //ͳ���м������ʽ��������
        if (expressionOfSet[i][0] == expressionOfSet[i + 1][0]) {
            count++;
        } else {
            break;
        }
    }

    //�Ƚ���һ��ѹ��ջ
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

//��ӽ�name��ӵ����У����ҽ���elem�����ɶ�Ӧ��Ԫ�أ���ӵ�elems��
int GrammarLL::addElement(const string &name, bool isFinally) {
    this->nameOfKeys[name] = elements.size();
    ElemLeft t(name, elements.size(), isFinally);
    elements.push_back(t);
    return elements.size() - 1;
}

//�޸�Ԫ��
void GrammarLL::updateElement(int elemIndex, int begin, int end, vector<int> &factory) {

    stringstream ss;
    ss << elements.size();
    int newIndex = addElement(elements[elemIndex].name + ss.str(), false);

    //�ȶ��µ�ʽ����ӱ��ʽ,��begin��end��ӵ�
    //�����մ���ֻ�е�һ��begin���������,���û���������,��begin�����ó�����Ϊ����
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

    //�޸���Ԫ��
    //��begin������½��
    elements[elemIndex].expressionOfSet[begin].expression.clear();
    for (int &i : factory) {
        elements[elemIndex].expressionOfSet[begin].expression.push_back(i);
    }
    elements[elemIndex].expressionOfSet[begin].expression.push_back(newIndex);

    //�����Ԫ����ǰ
    int moveDistance = end - begin;
    for (int i = end; i < elements[elemIndex].expressionOfSet.size(); i++) {
        elements[elemIndex].expressionOfSet[i - moveDistance + 1]
                = elements[elemIndex].expressionOfSet[i];
    }

    //���Ԫ��,�����moveDistance-1��
    for (int i = 0; i < moveDistance - 1; i++) {
        elements[elemIndex].expressionOfSet.pop_back();
    }
}

//LL�ķ�
//���캯��������֮��,�൱����������
/**
 * ���캯��
 */
GrammarLL::GrammarLL() {
    vector<vector<string>> vectorGenrates;
    vector<string> inputs;
    string input;

    //�����������ʽ,��һ���Ƿ��ս���ţ�������ǲ���ʽ
    cout << "������ʽ" << endl;
    cout << "$...$��ʾһ������,��$id$��ʾΪid" << endl;
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
            //�ַ�ת��
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
            elements[findName(vectorGenrates[i][0])].addExpression(Express(exp));
        }
    }
}