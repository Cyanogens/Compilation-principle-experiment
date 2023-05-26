/**
 *消除左递归/左因子
 *
 * @author Cyanogen
 * @date 2023-05-05 21:25
 */
#include <bits/stdc++.h>
#include "../baseclass/LL.h"

using namespace std;
using namespace ll_grammar;
using namespace base_grammar;
using namespace public_tool;

/**
 * 消除直接左递归
 * 先检查elems[i]有没有直接左递归,如果就需要将他消除,最后会在elems后面添加一些新的东西
 *
 * @param elemIndex
 * @param grammarLeft
 */
void clearLeft(int elemIndex, GrammarLL *grammarLeft) {
    //有左递归的式子下标
    vector<int> expressionIndex;
    //没有左递归的式子下标
    vector<int> notLeft;
    expressionIndex.clear();
    notLeft.clear();

    //有直接左递归压进去
    for (int i = 0; i < grammarLeft->elements[elemIndex].expressionOfSet.size(); i++) {
        if (grammarLeft->elements[elemIndex].expressionOfSet[i][0] == elemIndex) {
            //判断推出的第一位的字符下标是否是输入的字符下标
            expressionIndex.push_back(i);
        } else {
            notLeft.push_back(i);
        }
    }

    //消除直接直接左递归,size不等于0说明存在直接左递归,进行消除
    if (!expressionIndex.empty()) {
        //先分配一个elems的位置出来
        stringstream ss;
        ss << grammarLeft->elements.size();
        grammarLeft->addElement(grammarLeft->elements[elemIndex].name + ss.str(), false);

        int newIndex = grammarLeft->findName(grammarLeft->elements[elemIndex].name + ss.str());

        vector<Express> temp = grammarLeft->elements[elemIndex].expressionOfSet;

        //更改原来的文法
        grammarLeft->elements[elemIndex].expressionOfSet.clear();

        for (int i : notLeft) {
            //如果是空串,需要将空串除去
            if (temp[i].expression[0] == -1 && temp[i].expression.size() == 1) {
                temp.clear();
            }
            temp[i].expression.push_back(newIndex);
            grammarLeft->elements[elemIndex].addExpression(temp[i].expression);
        }

        //给新的非终结符添加的文法
        //包含空符号
        int t[1];
        t[0] = -1;
        grammarLeft->elements[newIndex].addExpression(Express(t, 1));

        for (int i : expressionIndex) {
            vector<int> a;
            for (int j = 1; j < temp[i].expression.size(); j++) {
                a.push_back(temp[i].expression[j]);
            }
            a.push_back(newIndex);
            grammarLeft->elements[newIndex].addExpression(a);
        }
    }
}

/**
 * 消除间接左递归
 *
 * @param oldElemIndex
 * @param replaceIndex
 * @param grammarLeft
 */
void replaceExpress(int oldElemIndex, int replaceIndex, GrammarLL *grammarLeft) {

    //这一段代码和提取直接左递归类似,可以合并的
    //有replaceIndex的式子下标
    vector<int> expressionIndex;
    //没有replaceIndex的式子下标
    vector<int> notLeft;
    expressionIndex.clear();
    notLeft.clear();

    //有replaceIndex压进去
    for (int i = 0; i < grammarLeft->elements[oldElemIndex].expressionOfSet.size(); i++) {
        if (grammarLeft->elements[oldElemIndex].expressionOfSet[i][0] == replaceIndex) {
            //在推出部分的第一个字符
            expressionIndex.push_back(i);
        } else {
            notLeft.push_back(i);
        }
    }

    //开始替换
    if (!expressionIndex.empty()) {
        //后来需要查的
        vector<Express> temp = grammarLeft->elements[oldElemIndex].expressionOfSet;
        grammarLeft->elements[oldElemIndex].expressionOfSet.clear();
        //先将不用改变的安置好
        for (int i : notLeft) {
            grammarLeft->elements[oldElemIndex].addExpression(temp[i]);
        }
        //需要改变的
        for (int i : expressionIndex) {
            //后缀
            vector<int> diff;
            diff.clear();
            //计算后缀
            for (int j = 1; j < temp[i].expression.size(); j++) {
                diff.push_back(temp[i][j]);
            }
            //替换的表达式遍历,第j+1个表达式
            for (int j = 0; j < grammarLeft->elements[replaceIndex].expressionOfSet.size(); j++) {
                vector<int> exTemp;
                exTemp.clear();
                //这个表达式为空
                if (grammarLeft->elements[replaceIndex].expressionOfSet[j][0] == -1 &&
                    grammarLeft->elements[replaceIndex].expressionOfSet[j].expression.size() == 1) {
                    //直接压入后缀就行
                    for (int &index : diff) {
                        exTemp.push_back(index);
                    }
                    grammarLeft->elements[oldElemIndex].addExpression(exTemp);
                    continue;
                }
                //将表达式压入,上一步已经将空串去除,替换操作
                for (int &index : grammarLeft->elements[replaceIndex].expressionOfSet[j].expression) {
                    exTemp.push_back(index);
                }
                //后缀保留
                for (int &index : diff) {
                    exTemp.push_back(index);
                }
                grammarLeft->elements[oldElemIndex].addExpression(exTemp);
            }
        }
    }
}

/**
 * 消除左递归
 *
 * @param grammarLeft
 */
void clearLeftRecursion(GrammarLL *grammarLeft) {

    //消除直接左递归
    for (int i = 0; i < grammarLeft->elements.size(); i++) {
        //这个元素不是终结符号,需要进行修改
        if (!grammarLeft->elements[i].isFinally) {
            clearLeft(i, grammarLeft);
        }
    }

    //消除间接左递归
    //根据生成规则,第0号下标就是开始符号
    for (int i = 1; i < grammarLeft->elements.size(); i++) {
        //如果elem[i]是终结符不需要
        if (!grammarLeft->elements[i].isFinally) {
            //先需要改写表达式
            for (int j = 0; j < i; j++) {
                //只有非终结符才要改变
                if (!grammarLeft->elements[j].isFinally) {
                    replaceExpress(i, j, grammarLeft);
                }
            }
        }
        //消除直接左递归
        clearLeft(i, grammarLeft);
    }
}

/**
 * 消除左因子
 *
 * @param grammarLeft
 */
void clearLeftFactor(GrammarLL *grammarLeft) {
    vector<int> factory;
    int begin, count;

    for (int i = 0; i < grammarLeft->elements.size(); i++) {
        //只有非终结符要消除左因子
        if (!grammarLeft->elements[i].isFinally) {
            while (grammarLeft->elements[i].extractLeftFactor(factory, begin, count)) {
                //i有左因子
                grammarLeft->updateElement(i, begin, begin + count, factory);
            }
        }
    }
}

GrammarLL *eliminate() {
    auto *grammarLeft = new GrammarLL();
    //消除左递归
    clearLeftRecursion(grammarLeft);
    //消除左因子
    clearLeftFactor(grammarLeft);
    grammarLeft->addElement("#", true);

    cout << "处理后的表达式为:" << endl;
    for (int i = 0; i < grammarLeft->elements.size(); i++) {
        if (!grammarLeft->elements[i].isFinally) {
            cout << grammarLeft->elements[i].name << "->";
            for (int j = 0; j < grammarLeft->elements[i].expressionOfSet.size(); j++) {
                for (int m = 0; m < grammarLeft->elements[i].expressionOfSet[j].expression.size(); m++) {
                    if (grammarLeft->elements[i].expressionOfSet[j].expression[m] != -1) {
                        cout << grammarLeft->elements[grammarLeft->elements[i].expressionOfSet[j].expression[m]].name;
                    } else {
                        cout << "@";
                    }
                }
                if (j < grammarLeft->elements[i].expressionOfSet.size() - 1) {
                    cout << " | ";
                }
            }
            cout << endl;
        }
    }
    cout << endl;
    return grammarLeft;
}


