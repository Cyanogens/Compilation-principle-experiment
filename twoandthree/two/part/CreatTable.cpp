#include <bits/stdc++.h>
#include "../baseclass/LL.h"

using namespace std;
using namespace ll_grammar;
using namespace base_grammar;
using namespace public_tool;

bool getFirst(int elemIndex, GrammarLL *grammarLeft);

//计算elems[elemIndex]的expressIndex下标的表达式的first集合，结果放在result里面
void
calculateExpressionFirst(int elemIndex, int expressionIndex, vector<int> &result, bool choose, GrammarLL *grammarLeft) {

    result.clear();

    for (int i = 0; i < grammarLeft->elements[elemIndex].expressionOfSet[expressionIndex].expression.size(); i++) {

        //对于这种情况有没有可能出现：
        //A->TAb|c|T
        //T->@|t
        //进行递归了
        if (grammarLeft->elements[elemIndex].expressionOfSet[expressionIndex][i] == elemIndex) {

            //后续处理
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
                vector<int> notNULL;//将空串除去
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

        //-1不在这个式子里面后面不用继续了
        if (!isInVector(grammarLeft->first[grammarLeft->elements[elemIndex].expressionOfSet[expressionIndex][i]], -1))
            break;
    }
}

bool getFirst(int elemIndex, GrammarLL *grammarLeft) {

    //空串
    if (elemIndex == -1) {
        return false;
    }

    //这个符号的first集合已经求出来了
    if (!grammarLeft->first[elemIndex].empty()) {
        return true;
    }

    //判断是不是终结符，是的话说明这个first可以直接求
    if (grammarLeft->elements[elemIndex].isFinally) {
        //是终结符,将自己添加就行
        grammarLeft->first[elemIndex].push_back(elemIndex);
    } else {
        //不是终结符，需要计算这个非终结符的每一个表达式然后在添加上去
        //vector<int> t;
        vector<int> temp;
        grammarLeft->firstTemp.clear();//后续准备
        //计算这个产生式的表达式
        for (int i = 0; i < grammarLeft->elements[elemIndex].expressionOfSet.size(); i++) {
            //计算第i个表达式
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

//根据findtable和Elems来产生终结符下标
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

//获得First集合
void createFirstSet(GrammarLL *grammarLeft) {
    for (int i = 0; i < grammarLeft->elements.size(); i++) {
        getFirst(i, grammarLeft);
    }
}

void getFollowByFirst(int elemIndex, Express express, int begin, vector<int> &result, GrammarLL *grammarLeft) {
    result.clear();
    vector<int> tmp;

    //如果是一个空串的话
    if (express[0] == -1) {
        result.push_back(elemIndex);
        return;
    }

    for (begin = begin; begin < express.expression.size(); begin++) {

        //如果空不在退出
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
    //保存使用
    vector<int> tempFollow = grammarLeft->follow[grammarLeft->notFinallyCharIndex[index]];
    for (int &i : tempFollow) {
        if (!grammarLeft->elements[i].isFinally) {
            //在原来的Follow中将这个非终结符删除
            vector<int>::const_iterator deleteIt = find(
                    grammarLeft->follow[grammarLeft->notFinallyCharIndex[index]].begin(),
                    grammarLeft->follow[grammarLeft->notFinallyCharIndex[index]].end(), i);
            grammarLeft->follow[grammarLeft->notFinallyCharIndex[index]].erase(deleteIt);

            //如果两个是一样的
            if (grammarLeft->notFinallyCharIndex[index] != i) {
                adjustFollow(i, grammarLeft);
                connectVector(grammarLeft->follow[grammarLeft->notFinallyCharIndex[index]], grammarLeft->follow[i]);
            }
        }

    }
}

//生成Follow集合
void createFollowSet(GrammarLL *grammarLeft) {

    vector<int> temp;
    //每个非终结符
    for (int i = 0; i < grammarLeft->notFinallyCharIndex.size(); i++) {
        //计算Follw需要的是各个表达式子,遍历第i个非终结符的各个表达式
        for (int j = 0; j < grammarLeft->elements[grammarLeft->notFinallyCharIndex[i]].expressionOfSet.size(); j++) {
            //第j个表达式
            Express &expreesion = grammarLeft->elements[grammarLeft->notFinallyCharIndex[i]].expressionOfSet[j];
            for (int m = 0; m < expreesion.expression.size(); m++) {
                //如果是空串的话
                if (expreesion[m] == -1) {
                    continue;
                }

                //是终结符不用计算
                if (grammarLeft->elements[expreesion[m]].isFinally) continue;

                //获得First集合
                getFollowByFirst(i, expreesion, m + 1, temp, grammarLeft);
                connectVector(grammarLeft->follow[expreesion[m]], temp);
            }
        }
    }
    //考虑follow集合的添加
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

//获取预测分析表
void getPredictTable(GrammarLL *grammarLeft) {
    //初始化预测分析表
    vector<int> tmp;
    for (int i = 0; i < grammarLeft->finallyCharIndex.size(); i++) {
        tmp.push_back(-1);//-1为错误
    }
    for (int i = 0; i < grammarLeft->notFinallyCharIndex.size(); i++) {
        grammarLeft->predictTable.push_back(tmp);
    }

    //开始计算了，以每一个非终结符为首开始
    for (int i = 0; i < grammarLeft->notFinallyCharIndex.size(); i++) {

        //遍历这个非终结符的表达式
        for (int j = 0; j < grammarLeft->elements[grammarLeft->notFinallyCharIndex[i]].expressionOfSet.size(); j++) {

            getFollowByFirst(grammarLeft->notFinallyCharIndex[i],
                             grammarLeft->elements[grammarLeft->notFinallyCharIndex[i]].expressionOfSet[j], 0, tmp,
                             grammarLeft);
            //如果tmp里面有非终结符，则说明有空，如果没有则说明没有空
            for (int &m : tmp) {
                //如果是终结符直接添加
                if (grammarLeft->elements[m].isFinally) {
                    vector<int>::const_iterator it = find(grammarLeft->finallyCharIndex.begin(),
                                                          grammarLeft->finallyCharIndex.end(), m);
                    grammarLeft->predictTable[i][it - grammarLeft->finallyCharIndex.begin()] = j;
                } else {
                    //如果是非终结符要添加Follow集合
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


//主函数
//生成预测分析表
void creatTable(GrammarLL *grammarLeft) {
    //两表生成（在5的时候就不会再添加新的符号了）
    //同时在这里初始化一下First和Follw表
    getTowCharIndex(grammarLeft);

    //求first集合
    createFirstSet(grammarLeft);

    //检查一下，first集合
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

    //求follow集合
    //将#送到开始符号
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
    //9.构造预测分析表
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
