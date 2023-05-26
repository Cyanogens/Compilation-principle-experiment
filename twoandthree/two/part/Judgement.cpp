#include <bits/stdc++.h>
#include "../baseclass/LL.h"

using namespace std;
using namespace ll_grammar;
using namespace base_grammar;
using namespace public_tool;

//根据预测分析表进行判断
void judgement(GrammarLL *grammarLL) {
    string inputString;
    bool t = true;

    cout << "输入待分析的语句" << endl;
    cout << "$...$表示一个符号,如$id$表示为id" << endl;
    cout << "输入单独的#以结束输入" << endl;

    while (true) {
        cout << "输入待解析表达式,以#结尾,输入单独的#为退出:";
        cin >> inputString;
        if (inputString == "#") {
            break;
        }

        //将出现双字符串转换;
        vector<string> changeString;
        parseElem(inputString, changeString);

        //存放字符的下标
        stack<int> s;
        //输出栈
        vector<int> sprint;
        //栈的格式
        char formatStack[] = "%-25s";
        //字母格式的格式
        char formatCharStack[] = "%+25s";
        //计算各种格式
        string strTemp;
        //上面的按照格式转化后的结果
        char change[100];

        //准备工作
        vector<int>::const_iterator x;
        vector<int>::const_iterator y;
        //指向测试的数据
        int index = 0;
        int expIndex;

        //初始化stack;
        s.push(grammarLL->findName("#"));
        sprint.push_back(grammarLL->findName("#"));

        s.push(0);
        sprint.push_back(0);

        string out = "";
        while (s.top() != grammarLL->findName("#")) {
            strTemp = "";
            //栈写出来
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
            //非终结符一定能找到，但是终结符不一定能找到
            if (y == grammarLL->finallyCharIndex.end()) {
                out = out + "error\n";
                t = false;
                break;
            }

            if (grammarLL->elements[s.top()].isFinally) {
                //如果栈中是终结符
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

                //抛出栈;
                sprint.pop_back();
                out = out + "pop(" + grammarLL->elements[*x].name + "),push(";
                s.pop();

                //压入栈;
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