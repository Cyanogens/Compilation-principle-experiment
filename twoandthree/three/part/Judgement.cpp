#include <bits/stdc++.h>
#include "../baseclass/LR.h"

using namespace std;
using namespace lr_grammar;
using namespace base_grammar;
using namespace public_tool;

/**
 * ջԪ��
 */
struct stackElem {
    //���ջ������״̬
    int states;
    int elemKey;
};

/**
 * ���ݹ�Լ������ж�
 *
 * @param grammarLr
 * @return
 */
void judgement(GrammarLR *grammarLr) {

    cout << "��������������" << endl;
    cout << "$...$��ʾһ������,��$id$��ʾΪid" << endl;
    cout << "���뵥����#�Խ�������" << endl;

    string testString;
    while (true) {
        cout << "������������ʽ,���뵥����#Ϊ�˳�:";
        cin >> testString;
        if (testString == "#"){
            break;
        }
        vector<string> ipString;
        //��������ַ������д���
        parseElem(testString, ipString);
        //ָ��ջ
        stack<stackElem> s;
        //�������
        vector<stackElem> elemPrint;
        //ջ�ĸ�ʽ
        char formatS[] = "%-25s";
        //��ĸ��ʽ�ĸ�ʽ
        char formatC[] = "%+15s         ";
        string bufS;
        //��ʽ��
        char buffer[200];
        stringstream sstream;
        string tmp;
        //����Ĵ�
        string text;
        //����ָ��
        int ipIndex = 0;

        stackElem e{};
        //#�ǿ�ʼ�ַ�
        e.elemKey = grammarLr->findName("#");
        //0�ǳ�̬
        e.states = 0;
        s.push(e);
        elemPrint.push_back(e);
        ReduceByExpress r{};
        int st;
        while (grammarLr->findName(ipString[ipIndex]) != -2
                && (grammarLr->actionTable[e.states][grammarLr->findName(ipString[ipIndex])].type == REDUCE
                || grammarLr->actionTable[e.states][grammarLr->findName(ipString[ipIndex])].type == SMOVE)) {
            e = s.top();

            //������ջ����
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

            //����Ҳ�ջ����
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
                    //��Լ����
                    //���еĶ���
                    text += "r:";
                    r = grammarLr->actionTable[e.states][grammarLr->findName(ipString[ipIndex])].choose.R;
                    sstream.clear();
                    sstream << grammarLr->actionTable[e.states][grammarLr->findName(ipString[ipIndex])].choose.state;
                    sstream >> buffer;
                    text += buffer;
                    text += "\n";

                    //��ջ
                    for (int i = 0;
                         i < grammarLr->elements[r.elemIndex].expressionOfSet[r.expressIndex].expression.size(); i++) {
                        s.pop();
                        elemPrint.pop_back();
                    }
                    //�滻�ս������
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

                    //��ջ����
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
        //��ʣ�µĽ������
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