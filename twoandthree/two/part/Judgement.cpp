#include <bits/stdc++.h>
#include "../baseclass/LL.h"

using namespace std;
using namespace ll_grammar;
using namespace base_grammar;
using namespace public_tool;

//����Ԥ�����������ж�
void judgement(GrammarLL *grammarLL) {
    string inputString;
    bool t = true;

    cout << "��������������" << endl;
    cout << "$...$��ʾһ������,��$id$��ʾΪid" << endl;
    cout << "���뵥����#�Խ�������" << endl;

    while (true) {
        cout << "������������ʽ,��#��β,���뵥����#Ϊ�˳�:";
        cin >> inputString;
        if (inputString == "#") {
            break;
        }

        //������˫�ַ���ת��;
        vector<string> changeString;
        parseElem(inputString, changeString);

        //����ַ����±�
        stack<int> s;
        //���ջ
        vector<int> sprint;
        //ջ�ĸ�ʽ
        char formatStack[] = "%-25s";
        //��ĸ��ʽ�ĸ�ʽ
        char formatCharStack[] = "%+25s";
        //������ָ�ʽ
        string strTemp;
        //����İ��ո�ʽת����Ľ��
        char change[100];

        //׼������
        vector<int>::const_iterator x;
        vector<int>::const_iterator y;
        //ָ����Ե�����
        int index = 0;
        int expIndex;

        //��ʼ��stack;
        s.push(grammarLL->findName("#"));
        sprint.push_back(grammarLL->findName("#"));

        s.push(0);
        sprint.push_back(0);

        string out = "";
        while (s.top() != grammarLL->findName("#")) {
            strTemp = "";
            //ջд����
            for (int i : sprint) {
                strTemp = strTemp + grammarLL->elements[i].name + " ";
            }
            sprintf(change, formatStack, strTemp.c_str());
            out = out + change;

            strTemp = "";
            for (int i = index; i < changeString.size(); i++) {
                strTemp = strTemp + changeString[i];
            }

            sprintf(change, formatCharStack, strTemp.c_str());
            out = out + change + "\t";

            y = find(grammarLL->finallyCharIndex.begin(), grammarLL->finallyCharIndex.end(),
                     grammarLL->findName(changeString[index]));
            //���ս��һ�����ҵ��������ս����һ�����ҵ�
            if (y == grammarLL->finallyCharIndex.end()) {
                out = out + "error\n";
                t = false;
                break;
            }

            if (grammarLL->elements[s.top()].isFinally) {
                //���ջ�����ս��
                if (*y == s.top()) {
                    sprint.pop_back();
                    out = out + "pop(" + grammarLL->elements[*y].name + "), next(" + to_string(index) + ")\n";
                    s.pop();
                    index++;

                } else {
                    out = out + "error\n";
                    t = false;
                    break;
                }
            } else {
                x = find(grammarLL->notFinallyCharIndex.begin(), grammarLL->notFinallyCharIndex.end(), s.top());
                expIndex = grammarLL->predictTable[x - grammarLL->notFinallyCharIndex.begin()][y -
                                                                                               grammarLL->finallyCharIndex.begin()];

                if (expIndex == -1) {
                    out = out + "error\n";
                    t = false;
                    break;
                }

                //�׳�ջ;
                sprint.pop_back();
                out = out + "pop(" + grammarLL->elements[*x].name + "),push(";
                s.pop();

                //ѹ��ջ;
                vector<int> &exp = grammarLL->elements[*x].expressionOfSet[expIndex].expression;
                for (int i = exp.size() - 1; i >= 0; i--) {
                    if (exp[i] == -1) {
                        continue;
                    }
                    out = out + grammarLL->elements[exp[i]].name + " ";
                    s.push(exp[i]);
                    sprint.push_back(exp[i]);
                }
                out = out + ")\n";
            }
        }
        cout<<out;
        if (t){
            cout<<"success"<< endl;
        }
    }
}