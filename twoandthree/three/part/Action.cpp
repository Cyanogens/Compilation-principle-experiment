#include <bits/stdc++.h>
#include "../baseclass/LR.h"

using namespace std;
using namespace lr_grammar;
using namespace base_grammar;
using namespace public_tool;

//�����ս���±�
void getTowCharIndex(GrammarLR *grammarLr) {
    vector<int> t;
    t.clear();

    for (int i = 0; i < grammarLr->elements.size(); i++) {
        if (grammarLr->elements[i].isFinally) {
            grammarLr->finallyCharIndex.push_back(i);
        } else {
            grammarLr->notFinallyCharIndex.push_back(i);
        }
        grammarLr->first.push_back(t);
        grammarLr->follow.push_back(t);
    }
}

void getFollowByFirst(GrammarLR *grammarLr, int elemIndex, ExpressLR express, int begin, vector<int> &result) {
    result.clear();
    vector<int> tmp;

    //�����һ���մ��Ļ�
    if (express[0] == -1) {
        result.push_back(elemIndex);
        return;
    }

    for (begin = begin; begin < express.expression.size(); begin++) {
        //����ղ����˳�
        if (!public_tool::isInVector(grammarLr->first[express[begin]], -1)) {
            public_tool::connectVector(result, grammarLr->first[express[begin]]);
            break;
        } else {
            clearNull(grammarLr->first[express[begin]], tmp);
            public_tool::connectVector(result, tmp);
        }
    }
    if (begin == express.expression.size()) {
        //result.push_back(-1);
        result.push_back(elemIndex);
    }
}

void adjustFollow(GrammarLR *grammarLr, int index) {
    //����ʹ��
    vector<int> tempFollow = grammarLr->follow[grammarLr->notFinallyCharIndex[index]];
    for (int &i : tempFollow) {
        if (!grammarLr->elements[i].isFinally) {
            //��ԭ����Follow�н�������ս��ɾ��
            vector<int>::const_iterator deleteIt = find(
                    grammarLr->follow[grammarLr->notFinallyCharIndex[index]].begin(),
                    grammarLr->follow[grammarLr->notFinallyCharIndex[index]].end(), i);
            grammarLr->follow[grammarLr->notFinallyCharIndex[index]].erase(deleteIt);

            //���������һ����
            if (grammarLr->notFinallyCharIndex[index] != i) {
                adjustFollow(grammarLr, i);

                public_tool::connectVector(grammarLr->follow[grammarLr->notFinallyCharIndex[index]],
                                           grammarLr->follow[i]);
            }
        }
    }
}

//����Follow����
void createFollowSet(GrammarLR *grammarLr) {

    vector<int> temp;
    //ÿ�����ս��
    for (int i = 0; i < grammarLr->notFinallyCharIndex.size(); i++) {
        //����Follw��Ҫ���Ǹ������ʽ��,������i�����ս���ĸ������ʽ
        for (int j = 0; j < grammarLr->elements[grammarLr->notFinallyCharIndex[i]].expressionOfSet.size(); j++) {
            //��j�����ʽ
            ExpressLR &expreesion = grammarLr->elements[grammarLr->notFinallyCharIndex[i]].expressionOfSet[j];
            for (int m = 0; m < expreesion.expression.size(); m++) {
                //����ǿմ��Ļ�
                if (expreesion[m] == -1) {
                    continue;
                }
                //���ս�����ü���
                if (grammarLr->elements[expreesion[m]].isFinally) continue;

                //���First����
                getFollowByFirst(grammarLr, i, expreesion, m + 1, temp);
                public_tool::connectVector(grammarLr->follow[expreesion[m]], temp);
            }
        }
    }
    //����follow���ϵ����
    for (int i = 0; i < grammarLr->notFinallyCharIndex.size(); i++) {
        adjustFollow(grammarLr, i);
    }
}

void calculateExpressionFirst(GrammarLR *grammarLr, int elemIndex, int expressionIndex, vector<int> &result,
                              bool choose = true);
//����elem[elemIndex]��first����,���ҽ��������First[elemIndex��]
//����ֵ�������true��˵��������ǿմ���elemIndex��=-1��;
//		  �����false��˵������ǿմ���elemIndex=-1,�����First����Ϊ��-1����
bool getFirst(GrammarLR *grammarLr, int elemIndex) {

    //�մ�
    if (elemIndex == -1) return false;

    //������ŵ�first�����Ѿ��������
    if (grammarLr->first[elemIndex].size() != 0) return true;

    //�ж��ǲ����ս�����ǵĻ�˵�����first����ֱ����
    if (grammarLr->elements[elemIndex].isFinally) {
        //���ս��,���Լ���Ӿ���
        grammarLr->first[elemIndex].push_back(elemIndex);
    } else {
        //�����ս������Ҫ����������ս����ÿһ�����ʽȻ���������ȥ
        //vector<int> t;
        vector<int> temp;
        grammarLr->firstTemp.clear();//����׼��
        //�����������ʽ�ı��ʽ
        for (int i = 0; i < grammarLr->elements[elemIndex].expressionOfSet.size(); i++) {
            //�����i�����ʽ
            calculateExpressionFirst(grammarLr, elemIndex, i, temp);
            public_tool::connectVector(grammarLr->first[elemIndex], temp);
        }
        //�������������û�п��ܳ��֣�
        //A->TAb|c|T
        //T->@|t
        //���еݹ���
        for (int i = 0; i < grammarLr->firstTemp.size(); i++) {
            calculateExpressionFirst(grammarLr, elemIndex, grammarLr->firstTemp[i], temp, false);
            public_tool::connectVector(grammarLr->first[elemIndex], temp);
        }

    }

    return true;
}

//����elems[elemIndex]��expressIndex�±�ı��ʽ��first���ϣ��������result����
void
calculateExpressionFirst(GrammarLR *grammarLr, int elemIndex, int expressionIndex, vector<int> &result, bool choose) {
    result.clear();
    for (int i = 0; i < grammarLr->elements[elemIndex].expressionOfSet[expressionIndex].expression.size(); i++) {
        if (grammarLr->elements[elemIndex].expressionOfSet[expressionIndex][i] == elemIndex) {

            //��������
            if (public_tool::isInVector(grammarLr->first[elemIndex], -1)) continue;
            else {
                if (choose) grammarLr->firstTemp.push_back(expressionIndex);
                break;
            }
        }
        if (getFirst(grammarLr, grammarLr->elements[elemIndex].expressionOfSet[expressionIndex][i])) {
            if (i == grammarLr->elements[elemIndex].expressionOfSet[expressionIndex].expression.size() - 1) {
                public_tool::connectVector(result,
                                           grammarLr->first[grammarLr->elements[elemIndex].expressionOfSet[expressionIndex][i]]);
            } else {
                vector<int> notNULL;//���մ���ȥ
                int length = grammarLr->first[grammarLr->elements[elemIndex].expressionOfSet[expressionIndex][i]].size();
                vector<int> tmpF = grammarLr->first[grammarLr->elements[elemIndex].expressionOfSet[expressionIndex][i]];
                for (int j = 0; j < length; j++) {
                    if (tmpF[j] != -1) {
                        notNULL.push_back(tmpF[j]);
                    }
                }
                public_tool::connectVector(result, notNULL);
            }

        } else {
            if (!public_tool::isInVector(result, -1)) result.push_back(-1);
            break;
        }

        //-1�������ʽ��������治�ü�����
        if (!public_tool::isInVector(
                grammarLr->first[grammarLr->elements[elemIndex].expressionOfSet[expressionIndex][i]], -1))
            break;

    }
}

//���First����
void createFirstSet(GrammarLR *grammarLr) {
    for (int i = 0; i < grammarLr->elements.size(); i++) {
        getFirst(grammarLr, i);
    }
}

void createActionTable(GrammarLR *grammarLr) {

    //��ʼ��һ�±�,��״̬����ô����У���Ԫ�ظ�����ô������У���ʼ�������з��ж���ERROR
    vector<Action> t;
    Action action{};
    action.type = ERROR;
    for (int i = 0; i < grammarLr->elements.size(); i++) {
        t.push_back(action);
    }

    for (int i = 0; i < grammarLr->states.size(); i++) {
        grammarLr->actionTable.push_back(t);
    }

    //��״̬Ϊ�����������
    for (int i = 0; i < grammarLr->states.size(); i++) {

        //ת�Ƶ�ֱ����ȡ����,����next����
        for (int j = 0; j < grammarLr->states[i].elementCount; j++) {

            //������-1˵�����Խ������ת��
            if (grammarLr->states[i].next[j] != -1) {

                if (grammarLr->actionTable[i][j].type != ERROR) {
                    cout << "��ͻ";
                    cout << grammarLr->actionTable[i][grammarLr->states[i].next[j]].type << "to" << " SMOVE";
                }
                grammarLr->actionTable[i][j].type = SMOVE;
                grammarLr->actionTable[i][j].choose.state = grammarLr->states[i].next[j];
            }
        }

        //��Լ��ʼ,��Ҫ�����ÿһ����Ŀ���жϵ��λ���ǲ��������
        for (int j = 0; j < grammarLr->states[i].projects.size(); j++) {
            Project tmpP = grammarLr->states[i].projects[j];
            if (grammarLr->elements[tmpP.elemKey].expressionOfSet[tmpP.expressIndex].expression.size() ==
                tmpP.projectPoint) {

                //��ʾ���ڱ��ʽ��󣬽��й�Լ
                //���ж��ǲ����ս����,�Ǿ���acc������REDUCE
                if (tmpP.elemKey == grammarLr->findName(grammarLr->elements[0].name + "'")) {
                    grammarLr->actionTable[i][grammarLr->findName("#")].type = ACC;
                    continue;

                }
                for (int fi = 0; fi < grammarLr->follow[tmpP.elemKey].size(); fi++) {

                    if (grammarLr->actionTable[i][grammarLr->follow[tmpP.elemKey][fi]].type != ERROR) {
                        cout << "��ͻ";
                        cout << grammarLr->actionTable[i][grammarLr->follow[tmpP.elemKey][fi]].type << "to"
                             << " REDUCE";
                    }
                    grammarLr->actionTable[i][grammarLr->follow[tmpP.elemKey][fi]].type = REDUCE;
                    grammarLr->actionTable[i][grammarLr->follow[tmpP.elemKey][fi]].choose.R.elemIndex = tmpP.elemKey;
                    grammarLr->actionTable[i][grammarLr->follow[tmpP.elemKey][fi]].choose.R.expressIndex = tmpP.expressIndex;
                }
            }
        }
    }
}

void action(GrammarLR *grammarLr) {

    //�������ɣ���5��ʱ��Ͳ���������µķ����ˣ�
    //ͬʱ�������ʼ��һ��First��Follw��
    getTowCharIndex(grammarLr);

    //��first����
    createFirstSet(grammarLr);

    //���һ�£�first����
    for (int i = 0; i < grammarLr->elements.size(); i++) {
        cout << "first[" << grammarLr->elements[i].name << "]={";
        for (int j = 0; j < grammarLr->first[i].size(); j++) {
            if (grammarLr->first[i][j] == -1) cout << "@ ";
            else {
                cout << grammarLr->elements[grammarLr->first[i][j]].name << " ";
            }
        }
        cout << "}\n";
    }

    //��follow����
    //��#�͵���ʼ����
    grammarLr->follow[0].push_back(grammarLr->findName("#"));
    createFollowSet(grammarLr);


    for (int i = 0; i < grammarLr->elements.size(); i++) {

        if (grammarLr->elements[i].isFinally) continue;

        cout << "follow[" << grammarLr->elements[i].name << "]={";
        for (int j = 0; j < grammarLr->follow[i].size(); j++) {
            if (grammarLr->follow[i][j] == -1) cout << "@ ";
            else {
                cout << grammarLr->elements[grammarLr->follow[i][j]].name << " ";
            }
        }
        cout << "}\n";
    }




    //����ָ�������ı�
    createActionTable(grammarLr);

    //�����ӡ����
    char formatS[] = "%+7s";//ջ�ĸ�ʽ
    string text = "";
    char buffer[8];
    for (int i = 0; i < grammarLr->elements.size(); i++) {
        sprintf(buffer, formatS, grammarLr->elements[i].name.c_str());
        text = text + buffer;
    }
    text = text + "\n";
    for (int i = 0; i < grammarLr->actionTable.size(); i++) {
        text = text + "\n" + to_string(i);
        for (int j = 0; j < grammarLr->actionTable[i].size(); j++) {
            string t;
            switch (grammarLr->actionTable[i][j].type) {
                case ERROR:
                    t = " ";
                    break;
                case REDUCE:
                    t = "R" + to_string(grammarLr->actionTable[i][j].choose.state);
                    break;
                case SMOVE:
                    t = "S" + to_string(grammarLr->actionTable[i][j].choose.state);
                    break;
                case ACC:
                    t = "ACC";
                    break;
                default:
                    break;
            }
            sprintf(buffer, formatS, t.c_str());
            text = text + buffer;
        }

    }
    cout << text << endl;
}

bool inVector(const vector<Project> &a, const Project &b) {
    bool result = false;
    for (auto i : a) {
        if (i == b) {
            result = true;
            break;
        }
    }
    return result;
}

void conncetVector(vector<Project> &a, const vector<Project> &b) {
    for (auto i : b) {
        if (!inVector(a, i)) {
            a.push_back(i);
        }
    }
}


