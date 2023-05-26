#include <bits/stdc++.h>
#include "../baseclass/LL.h"

using namespace std;
using namespace ll_grammar;
using namespace base_grammar;
using namespace public_tool;

bool getFirst(int elemIndex, GrammarLL *grammarLeft);

//����elems[elemIndex]��expressIndex�±�ı��ʽ��first���ϣ��������result����
void
calculateExpressionFirst(int elemIndex, int expressionIndex, vector<int> &result, bool choose, GrammarLL *grammarLeft) {

    result.clear();

    for (int i = 0; i < grammarLeft->elements[elemIndex].expressionOfSet[expressionIndex].expression.size(); i++) {

        //�������������û�п��ܳ��֣�
        //A->TAb|c|T
        //T->@|t
        //���еݹ���
        if (grammarLeft->elements[elemIndex].expressionOfSet[expressionIndex][i] == elemIndex) {

            //��������
            if (isInVector(grammarLeft->first[elemIndex], -1)) continue;
            else {
                if (choose) {
                    grammarLeft->firstTemp.push_back(expressionIndex);
                }
                break;
            }

        }

        if (getFirst(grammarLeft->elements[elemIndex].expressionOfSet[expressionIndex][i], grammarLeft)) {

            if (i == grammarLeft->elements[elemIndex].expressionOfSet[expressionIndex].expression.size() - 1) {
                connectVector(result,
                              grammarLeft->first[grammarLeft->elements[elemIndex].expressionOfSet[expressionIndex][i]]);
            } else {
                vector<int> notNULL;//���մ���ȥ
                int length = grammarLeft->first[grammarLeft->elements[elemIndex].expressionOfSet[expressionIndex][i]].size();
                vector<int> tmpF = grammarLeft->first[grammarLeft->elements[elemIndex].expressionOfSet[expressionIndex][i]];
                for (int j = 0; j < length; j++) {
                    if (tmpF[j] != -1) {
                        notNULL.push_back(tmpF[j]);
                    }
                }
                connectVector(result, notNULL);
            }

        } else {
            if (!isInVector(result, -1)) result.push_back(-1);
            break;
        }

        //-1�������ʽ��������治�ü�����
        if (!isInVector(grammarLeft->first[grammarLeft->elements[elemIndex].expressionOfSet[expressionIndex][i]], -1))
            break;
    }
}

bool getFirst(int elemIndex, GrammarLL *grammarLeft) {

    //�մ�
    if (elemIndex == -1) {
        return false;
    }

    //������ŵ�first�����Ѿ��������
    if (!grammarLeft->first[elemIndex].empty()) {
        return true;
    }

    //�ж��ǲ����ս�����ǵĻ�˵�����first����ֱ����
    if (grammarLeft->elements[elemIndex].isFinally) {
        //���ս��,���Լ���Ӿ���
        grammarLeft->first[elemIndex].push_back(elemIndex);
    } else {
        //�����ս������Ҫ����������ս����ÿһ�����ʽȻ���������ȥ
        //vector<int> t;
        vector<int> temp;
        grammarLeft->firstTemp.clear();//����׼��
        //�����������ʽ�ı��ʽ
        for (int i = 0; i < grammarLeft->elements[elemIndex].expressionOfSet.size(); i++) {
            //�����i�����ʽ
            calculateExpressionFirst(elemIndex, i, temp, true, grammarLeft);
            connectVector(grammarLeft->first[elemIndex], temp);
        }
        for (int i : grammarLeft->firstTemp) {
            calculateExpressionFirst(elemIndex, i, temp, false, grammarLeft);
            connectVector(grammarLeft->first[elemIndex], temp);
        }
    }
    return true;
}

//����findtable��Elems�������ս���±�
void getTowCharIndex(GrammarLL *grammarLeft) {
    vector<int> t;
    t.clear();

    for (int i = 0; i < grammarLeft->elements.size(); i++) {
        if (grammarLeft->elements[i].isFinally) {
            grammarLeft->finallyCharIndex.push_back(i);
        } else {
            grammarLeft->notFinallyCharIndex.push_back(i);
        }
        grammarLeft->first.push_back(t);
        grammarLeft->follow.push_back(t);
    }
}

//���First����
void createFirstSet(GrammarLL *grammarLeft) {
    for (int i = 0; i < grammarLeft->elements.size(); i++) {
        getFirst(i, grammarLeft);
    }
}

void getFollowByFirst(int elemIndex, Express express, int begin, vector<int> &result, GrammarLL *grammarLeft) {
    result.clear();
    vector<int> tmp;

    //�����һ���մ��Ļ�
    if (express[0] == -1) {
        result.push_back(elemIndex);
        return;
    }

    for (begin = begin; begin < express.expression.size(); begin++) {

        //����ղ����˳�
        if (!isInVector(grammarLeft->first[express[begin]], -1)) {
            connectVector(result, grammarLeft->first[express[begin]]);
            break;
        } else {
            clearNull(grammarLeft->first[express[begin]], tmp);
            connectVector(result, tmp);
        }

    }
    if (begin == express.expression.size()) {
        //result.push_back(-1);
        result.push_back(elemIndex);
    }
}

void adjustFollow(int index, GrammarLL *grammarLeft) {
    //����ʹ��
    vector<int> tempFollow = grammarLeft->follow[grammarLeft->notFinallyCharIndex[index]];
    for (int &i : tempFollow) {
        if (!grammarLeft->elements[i].isFinally) {
            //��ԭ����Follow�н�������ս��ɾ��
            vector<int>::const_iterator deleteIt = find(
                    grammarLeft->follow[grammarLeft->notFinallyCharIndex[index]].begin(),
                    grammarLeft->follow[grammarLeft->notFinallyCharIndex[index]].end(), i);
            grammarLeft->follow[grammarLeft->notFinallyCharIndex[index]].erase(deleteIt);

            //���������һ����
            if (grammarLeft->notFinallyCharIndex[index] != i) {
                adjustFollow(i, grammarLeft);
                connectVector(grammarLeft->follow[grammarLeft->notFinallyCharIndex[index]], grammarLeft->follow[i]);
            }
        }

    }
}

//����Follow����
void createFollowSet(GrammarLL *grammarLeft) {

    vector<int> temp;
    //ÿ�����ս��
    for (int i = 0; i < grammarLeft->notFinallyCharIndex.size(); i++) {
        //����Follw��Ҫ���Ǹ������ʽ��,������i�����ս���ĸ������ʽ
        for (int j = 0; j < grammarLeft->elements[grammarLeft->notFinallyCharIndex[i]].expressionOfSet.size(); j++) {
            //��j�����ʽ
            Express &expreesion = grammarLeft->elements[grammarLeft->notFinallyCharIndex[i]].expressionOfSet[j];
            for (int m = 0; m < expreesion.expression.size(); m++) {
                //����ǿմ��Ļ�
                if (expreesion[m] == -1) {
                    continue;
                }

                //���ս�����ü���
                if (grammarLeft->elements[expreesion[m]].isFinally) continue;

                //���First����
                getFollowByFirst(i, expreesion, m + 1, temp, grammarLeft);
                connectVector(grammarLeft->follow[expreesion[m]], temp);
            }
        }
    }
    //����follow���ϵ����
    for (int i = 0; i < grammarLeft->notFinallyCharIndex.size(); i++) {
        adjustFollow(i, grammarLeft);
    }
}

void clearNull(GrammarLL *grammarLl, const vector<int> &a, vector<int> &result, int value) {
    result.clear();
    for (int i : a) {
        if (i != value) {
            result.push_back(i);
        }
    }
}

//��ȡԤ�������
void getPredictTable(GrammarLL *grammarLeft) {
    //��ʼ��Ԥ�������
    vector<int> tmp;
    for (int i = 0; i < grammarLeft->finallyCharIndex.size(); i++) {
        tmp.push_back(-1);//-1Ϊ����
    }
    for (int i = 0; i < grammarLeft->notFinallyCharIndex.size(); i++) {
        grammarLeft->predictTable.push_back(tmp);
    }

    //��ʼ�����ˣ���ÿһ�����ս��Ϊ�׿�ʼ
    for (int i = 0; i < grammarLeft->notFinallyCharIndex.size(); i++) {

        //����������ս���ı��ʽ
        for (int j = 0; j < grammarLeft->elements[grammarLeft->notFinallyCharIndex[i]].expressionOfSet.size(); j++) {

            getFollowByFirst(grammarLeft->notFinallyCharIndex[i],
                             grammarLeft->elements[grammarLeft->notFinallyCharIndex[i]].expressionOfSet[j], 0, tmp,
                             grammarLeft);
            //���tmp�����з��ս������˵���пգ����û����˵��û�п�
            for (int &m : tmp) {
                //������ս��ֱ�����
                if (grammarLeft->elements[m].isFinally) {
                    vector<int>::const_iterator it = find(grammarLeft->finallyCharIndex.begin(),
                                                          grammarLeft->finallyCharIndex.end(), m);
                    grammarLeft->predictTable[i][it - grammarLeft->finallyCharIndex.begin()] = j;
                } else {
                    //����Ƿ��ս��Ҫ���Follow����
                    for (int k = 0; k < grammarLeft->follow[m].size(); k++) {
                        vector<int>::const_iterator it = find(grammarLeft->finallyCharIndex.begin(),
                                                              grammarLeft->finallyCharIndex.end(),
                                                              grammarLeft->follow[m][k]);
                        grammarLeft->predictTable[i][it - grammarLeft->finallyCharIndex.begin()] = j;
                    }
                }
            }
        }
    }
}


//������
//����Ԥ�������
void creatTable(GrammarLL *grammarLeft) {
    //�������ɣ���5��ʱ��Ͳ���������µķ����ˣ�
    //ͬʱ�������ʼ��һ��First��Follw��
    getTowCharIndex(grammarLeft);

    //��first����
    createFirstSet(grammarLeft);

    //���һ�£�first����
    for (int i = 0; i < grammarLeft->elements.size(); i++) {
        if (grammarLeft->elements[i].isFinally) continue;
        cout << "first[" << grammarLeft->elements[i].name << "]={";

        for (int j = 0; j < grammarLeft->first[i].size(); j++) {
            if (grammarLeft->first[i][j] == -1) cout << "@ ";
            else {
                cout << grammarLeft->elements[grammarLeft->first[i][j]].name << " ";
            }
        }
        cout << "}\n";

    }

    //��follow����
    //��#�͵���ʼ����
    grammarLeft->follow[0].push_back(grammarLeft->findName("#"));
    createFollowSet(grammarLeft);


    for (int i = 0; i < grammarLeft->elements.size(); i++) {

        if (grammarLeft->elements[i].isFinally) continue;

        cout << "follow[" << grammarLeft->elements[i].name << "]={";
        for (int j = 0; j < grammarLeft->follow[i].size(); j++) {
            if (grammarLeft->follow[i][j] == -1) cout << "@ ";
            else {
                cout << grammarLeft->elements[grammarLeft->follow[i][j]].name << " ";
            }
        }
        cout << "}\n";
    }
    //9.����Ԥ�������
    getPredictTable(grammarLeft);
    vector<string> arr;
    for (auto &element : grammarLeft->elements) {
        if (element.isFinally) {
            cout << setw(5) << element.name;
            arr.push_back(element.name);
        }
    }

    cout << endl;
    int a = -1;
    int b = -1;
    for (int i = 0; i < grammarLeft->elements.size(); i++) {
        if (!grammarLeft->elements[i].isFinally) {
            cout << grammarLeft->elements[i].name << setw(5);
            a++;
        }
        if (a == b + 1) {
            for (int l = 0; l < grammarLeft->predictTable[a].size(); l++) {
                if (grammarLeft->predictTable[a][l] >= 0) {
                    if (grammarLeft->elements[grammarLeft->notFinallyCharIndex[a]].expressionOfSet.size() == 1) {
                        string arr = "";
                        for (int o = 0; o <
                                        grammarLeft->elements[grammarLeft->notFinallyCharIndex[a]].expressionOfSet.size(); o++) {
                            for (int j = 0;
                                 j <
                                 grammarLeft->elements[grammarLeft->notFinallyCharIndex[a]].expressionOfSet[o].expression.size(); ++j) {

                                arr += grammarLeft->elements[grammarLeft->elements[grammarLeft->notFinallyCharIndex[a]].expressionOfSet[o].expression[j]].name;
                            }
                        }
                        cout << arr << setw(3);
                    } else {
                        string arr1 = "";
                        for (int o = 0; o <
                                        grammarLeft->elements[grammarLeft->notFinallyCharIndex[a]].expressionOfSet.size(); o++) {
                            arr1 = "";
                            if (grammarLeft->elements[grammarLeft->notFinallyCharIndex[a]].expressionOfSet[o].expression[0] ==
                                grammarLeft->findName(
                                        grammarLeft->elements[grammarLeft->notFinallyCharIndex[l]].name)) {

                                for (int j = 0; j <
                                                grammarLeft->elements[grammarLeft->notFinallyCharIndex[a]].expressionOfSet[o].expression.size(); ++j) {
                                    arr1 += grammarLeft->elements[grammarLeft->elements[grammarLeft->notFinallyCharIndex[a]].expressionOfSet[o].expression[j]].name;
                                }
                            } else {
                                arr1 = "@";
                            }
                        }
                        cout << arr1 << setw(4);
                    }

                } else {
                    cout << -1 << setw(5);
                }
            }
            b++;
            cout << endl;
            cout << "\b\b\b\b\b";
        }
    }
}
