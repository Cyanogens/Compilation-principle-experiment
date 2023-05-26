#ifndef LL_H_
#define LL_H_

#include <bits/stdc++.h>
#include "../../baseclass/Base.h"

using namespace std;

//LL型文法的特例,主要是方法的不同
namespace ll_grammar {
    using namespace base_grammar;

    //LL的符号表示
    class ElemLeft : public Elem {
    public:
        //记录推出表达式的字符的下标
        vector<Express> expressionOfSet;
    public:
        bool addExpression(const Express &expression);

        /**
         * 构造函数
         *
         * @param name
         * @param key
         * @param isFinally
         */
        ElemLeft(string name, int key, bool isFinally);

        /**
         * 提取左因子
         *
         * @param result
         * @param begin
         * @param count
         * @return true:有左因子
         */
        bool extractLeftFactor(vector<int> &result, int &begin, int &count);
    };

    class GrammarLL : public Grammar {

    public:
        //记录所有字符
        vector<ElemLeft> elements;
        //LL分析预测表
        //二维数组,第一个为非终结符号
        vector<vector<int>> predictTable;
        //保存记录,主要的是计算First需要使用
        vector<int> firstTemp;

    public:
        GrammarLL();

        /**
         * 添加符号,可以是终结符也可以是非终结符
         *
         * @param name
         * @param isFinally
         * @return
         */
        int addElement(const string &name, bool isFinally);

        void updateElement(int elemIndex, int begin, int end, vector<int> &factory);
    };
}
#endif
