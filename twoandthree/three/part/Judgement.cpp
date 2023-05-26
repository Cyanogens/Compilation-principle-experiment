#include <bits/stdc++.h>
#include "../baseclass/LR.h"

using namespace std;
using namespace lr_grammar;
using namespace base_grammar;
using namespace public_tool;

/**
 * 栈元素
 */
struct stackElem {
    //左侧栈的数字状态
    int states;
    int elemKey;
};

/**
 * 根据规约表进行判断
 *
 * @param grammarLr
 * @return
 */
void judgement(GrammarLR *grammarLr) {

    cout << "输入待分析的语句" << endl;
    cout << "$...$表示一个符号,如$id$表示为id" << endl;
    cout << "输入单独的#以结束输入" << endl;

    string testString;
    while (true) {
        cout << "输入待解析表达式,输入单独的#为退出:";
        cin >> testString;
        if (testString == "#"){
            break;
        }
        vector<string> ipString;
        //对输入的字符串进行处理
        parseElem(testString, ipString);
        //指导栈
        stack<stackElem> s;
        //输出作用
        vector<stackElem> elemPrint;
        //栈的格式
        char formatS[] = "%-25s";
        //字母格式的格式
        char formatC[] = "%+15s         ";
        string bufS;
        //格式化
        char buffer[200];
        stringstream sstream;
        string tmp;
        //输出的串
        string text;
        //坐标指向
        int ipIndex = 0;

        stackElem e{};
        //#是开始字符
        e.elemKey = grammarLr->findName("#");
        //0是初态
        e.states = 0;
        s.push(e);
        elemPrint.push_back(e);
        ReduceByExpress r{};
        int st;
        while (grammarLr->findName(ipString[ipIndex]) != -2
                && (grammarLr->actionTable[e.states][grammarLr->findName(ipString[ipIndex])].type == REDUCE
                || grammarLr->actionTable[e.states][grammarLr->findName(ipString[ipIndex])].type == SMOVE)) {
            e = s.top();

            //输出左侧栈内容
            tmp = "";
            for (auto &i : elemPrint) {
                tmp += grammarLr->elements[i.elemKey].name;
                sstream.clear();
                sstream << i.states;
                sstream >> buffer;
                tmp += buffer;
            }
            sprintf(buffer, formatS, tmp.c_str());
            text += buffer;

            //输出右侧栈内容
            tmp = "";
            for (int i = ipIndex; i < ipString.size(); i++) {
                tmp += ipString[i];
            }
            sprintf(buffer, formatC, tmp.c_str());
            text += buffer;

            switch (grammarLr->actionTable[e.states][grammarLr->findName(ipString[ipIndex])].type) {
                case ERROR:
                    break;

                case REDUCE:
                    //归约操作
                    //进行的动作
                    text += "r:";
                    r = grammarLr->actionTable[e.states][grammarLr->findName(ipString[ipIndex])].choose.R;
                    sstream.clear();
                    sstream << grammarLr->actionTable[e.states][grammarLr->findName(ipString[ipIndex])].choose.state;
                    sstream >> buffer;
                    text += buffer;
                    text += "\n";

                    //出栈
                    for (int i = 0;
                         i < grammarLr->elements[r.elemIndex].expressionOfSet[r.expressIndex].expression.size(); i++) {
                        s.pop();
                        elemPrint.pop_back();
                    }
                    //替换终结符操作
                    e = s.top();
                    if (grammarLr->actionTable[e.states][r.elemIndex].type == ERROR) {
                        text += "error";
                        break;
                    }
                    e.elemKey = r.elemIndex;
                    e.states = grammarLr->actionTable[e.states][r.elemIndex].choose.state;
                    s.push(e);
                    elemPrint.push_back(e);
                    break;
                case SMOVE:
                    text += "s:";
                    st = grammarLr->actionTable[e.states][grammarLr->findName(ipString[ipIndex])].choose.state;
                    sstream.clear();
                    sstream << st;
                    sstream >> buffer;
                    text = text + buffer + "\n";

                    //入栈就行
                    e.elemKey = grammarLr->findName(ipString[ipIndex]);
                    e.states = st;
                    s.push(e);
                    elemPrint.push_back(e);
                    ipIndex++;
                    break;
                case ACC:
                    break;
                default:
                    break;
            }
        }
        //将剩下的进行输出
        tmp = "";
        for (auto &i : elemPrint) {
            tmp += grammarLr->elements[i.elemKey].name;
            sstream.clear();
            sstream << i.states;
            sstream >> buffer;
            tmp += buffer;
        }
        sprintf(buffer, formatS, tmp.c_str());
        text += buffer;

        tmp = "";
        for (int i = ipIndex; i < ipString.size(); i++) {
            tmp += ipString[i];
        }
        sprintf(buffer, formatC, tmp.c_str());
        text += buffer;

        if (grammarLr->actionTable[e.states][grammarLr->findName(ipString[ipIndex])].type != ACC) {
            text += "error\n";
        } else {
            text += "acc\n";
        }
        cout << text;
    }
}