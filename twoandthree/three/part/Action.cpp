#include <bits/stdc++.h>
#include "../baseclass/LR.h"

using namespace std;
using namespace lr_grammar;
using namespace base_grammar;
using namespace public_tool;

//产生终结符下标
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

    //如果是一个空串的话
    if (express[0] == -1) {
        result.push_back(elemIndex);
        return;
    }

    for (begin = begin; begin < express.expression.size(); begin++) {
        //如果空不在退出
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
    //保存使用
    vector<int> tempFollow = grammarLr->follow[grammarLr->notFinallyCharIndex[index]];
    for (int &i : tempFollow) {
        if (!grammarLr->elements[i].isFinally) {
            //在原来的Follow中将这个非终结符删除
            vector<int>::const_iterator deleteIt = find(
                    grammarLr->follow[grammarLr->notFinallyCharIndex[index]].begin(),
                    grammarLr->follow[grammarLr->notFinallyCharIndex[index]].end(), i);
            grammarLr->follow[grammarLr->notFinallyCharIndex[index]].erase(deleteIt);

            //如果两个是一样的
            if (grammarLr->notFinallyCharIndex[index] != i) {
                adjustFollow(grammarLr, i);

                public_tool::connectVector(grammarLr->follow[grammarLr->notFinallyCharIndex[index]],
                                           grammarLr->follow[i]);
            }
        }
    }
}

//生成Follow集合
void createFollowSet(GrammarLR *grammarLr) {

    vector<int> temp;
    //每个非终结符
    for (int i = 0; i < grammarLr->notFinallyCharIndex.size(); i++) {
        //计算Follw需要的是各个表达式子,遍历第i个非终结符的各个表达式
        for (int j = 0; j < grammarLr->elements[grammarLr->notFinallyCharIndex[i]].expressionOfSet.size(); j++) {
            //第j个表达式
            ExpressLR &expreesion = grammarLr->elements[grammarLr->notFinallyCharIndex[i]].expressionOfSet[j];
            for (int m = 0; m < expreesion.expression.size(); m++) {
                //如果是空串的话
                if (expreesion[m] == -1) {
                    continue;
                }
                //是终结符不用计算
                if (grammarLr->elements[expreesion[m]].isFinally) continue;

                //获得First集合
                getFollowByFirst(grammarLr, i, expreesion, m + 1, temp);
                public_tool::connectVector(grammarLr->follow[expreesion[m]], temp);
            }
        }
    }
    //考虑follow集合的添加
    for (int i = 0; i < grammarLr->notFinallyCharIndex.size(); i++) {
        adjustFollow(grammarLr, i);
    }
}

void calculateExpressionFirst(GrammarLR *grammarLr, int elemIndex, int expressionIndex, vector<int> &result,
                              bool choose = true);
//计算elem[elemIndex]的first集合,并且将结果放在First[elemIndex中]
//返回值，如果是true，说明这个不是空串（elemIndex！=-1）;
//		  如果是false，说明这个是空串，elemIndex=-1,级这个First集合为（-1）空
bool getFirst(GrammarLR *grammarLr, int elemIndex) {

    //空串
    if (elemIndex == -1) return false;

    //这个符号的first集合已经求出来了
    if (grammarLr->first[elemIndex].size() != 0) return true;

    //判断是不是终结符，是的话说明这个first可以直接求
    if (grammarLr->elements[elemIndex].isFinally) {
        //是终结符,将自己添加就行
        grammarLr->first[elemIndex].push_back(elemIndex);
    } else {
        //不是终结符，需要计算这个非终结符的每一个表达式然后在添加上去
        //vector<int> t;
        vector<int> temp;
        grammarLr->firstTemp.clear();//后续准备
        //计算这个产生式的表达式
        for (int i = 0; i < grammarLr->elements[elemIndex].expressionOfSet.size(); i++) {
            //计算第i个表达式
            calculateExpressionFirst(grammarLr, elemIndex, i, temp);
            public_tool::connectVector(grammarLr->first[elemIndex], temp);
        }
        //对于这种情况有没有可能出现：
        //A->TAb|c|T
        //T->@|t
        //进行递归了
        for (int i = 0; i < grammarLr->firstTemp.size(); i++) {
            calculateExpressionFirst(grammarLr, elemIndex, grammarLr->firstTemp[i], temp, false);
            public_tool::connectVector(grammarLr->first[elemIndex], temp);
        }

    }

    return true;
}

//计算elems[elemIndex]的expressIndex下标的表达式的first集合，结果放在result里面
void
calculateExpressionFirst(GrammarLR *grammarLr, int elemIndex, int expressionIndex, vector<int> &result, bool choose) {
    result.clear();
    for (int i = 0; i < grammarLr->elements[elemIndex].expressionOfSet[expressionIndex].expression.size(); i++) {
        if (grammarLr->elements[elemIndex].expressionOfSet[expressionIndex][i] == elemIndex) {

            //后续处理
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
                vector<int> notNULL;//将空串除去
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

        //-1不在这个式子里面后面不用继续了
        if (!public_tool::isInVector(
                grammarLr->first[grammarLr->elements[elemIndex].expressionOfSet[expressionIndex][i]], -1))
            break;

    }
}

//获得First集合
void createFirstSet(GrammarLR *grammarLr) {
    for (int i = 0; i < grammarLr->elements.size(); i++) {
        getFirst(grammarLr, i);
    }
}

void createActionTable(GrammarLR *grammarLr) {

    //初始化一下表,有状态数那么多个行，有元素个数那么多个的列，初始化的所有符列都是ERROR
    vector<Action> t;
    Action action{};
    action.type = ERROR;
    for (int i = 0; i < grammarLr->elements.size(); i++) {
        t.push_back(action);
    }

    for (int i = 0; i < grammarLr->states.size(); i++) {
        grammarLr->actionTable.push_back(t);
    }

    //以状态为基础遍历填表
    for (int i = 0; i < grammarLr->states.size(); i++) {

        //转移的直接提取就行,遍历next函数
        for (int j = 0; j < grammarLr->states[i].elementCount; j++) {

            //不等于-1说明可以接受这个转移
            if (grammarLr->states[i].next[j] != -1) {

                if (grammarLr->actionTable[i][j].type != ERROR) {
                    cout << "冲突";
                    cout << grammarLr->actionTable[i][grammarLr->states[i].next[j]].type << "to" << " SMOVE";
                }
                grammarLr->actionTable[i][j].type = SMOVE;
                grammarLr->actionTable[i][j].choose.state = grammarLr->states[i].next[j];
            }
        }

        //归约开始,需要检查是每一个项目，判断点的位置是不是在最后
        for (int j = 0; j < grammarLr->states[i].projects.size(); j++) {
            Project tmpP = grammarLr->states[i].projects[j];
            if (grammarLr->elements[tmpP.elemKey].expressionOfSet[tmpP.expressIndex].expression.size() ==
                tmpP.projectPoint) {

                //表示点在表达式最后，进行归约
                //先判断是不是终结符号,是就是acc而不是REDUCE
                if (tmpP.elemKey == grammarLr->findName(grammarLr->elements[0].name + "'")) {
                    grammarLr->actionTable[i][grammarLr->findName("#")].type = ACC;
                    continue;

                }
                for (int fi = 0; fi < grammarLr->follow[tmpP.elemKey].size(); fi++) {

                    if (grammarLr->actionTable[i][grammarLr->follow[tmpP.elemKey][fi]].type != ERROR) {
                        cout << "冲突";
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

    //两表生成（在5的时候就不会再添加新的符号了）
    //同时在这里初始化一下First和Follw表
    getTowCharIndex(grammarLr);

    //求first集合
    createFirstSet(grammarLr);

    //检查一下，first集合
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

    //求follow集合
    //将#送到开始符号
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




    //创建指导动作的表
    createActionTable(grammarLr);

    //将表打印出来
    char formatS[] = "%+7s";//栈的格式
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


