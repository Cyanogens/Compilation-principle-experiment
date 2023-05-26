#ifndef LR_H_
#define LR_H_


#include <bits/stdc++.h>
#include "../../baseclass/Base.h"

using namespace std;

//LR型文法的特例,主要是方法的不同
namespace lr_grammar {
    using namespace base_grammar;

    //LR文法的表达式
    class ExpressLR : public Express {
    public://方法
        //构造函数
        ExpressLR(int *expressions, int length);

        ExpressLR(const vector<int> &expression);
    };

    //LR的符号表示
    class ElemRight : public Elem {
    public:
        vector<ExpressLR> expressionOfSet;
    public:
        //构造函数
        ElemRight(string name, int key, bool isFinally);
        //添加表达式
        bool addExpression(const ExpressLR &expression);
    };

    //LR的项目
    class Project {
    public:
        int elemKey;
        int expressIndex;
        int projectPoint;
    public:
        Project(int elemKey, int expressIndex, int point);

        bool operator<(const Project &project) const;

        bool operator==(const Project &project) const;

        bool operator!=(const Project &project) const;
    };

    class DFAState {
    public:
        //项目集
        vector<Project> projects;

        //状态转移的，next的长度为elem元素的长度
        int *next;
        int elementCount;
        //状态集合的下标，主要是为了提高效率;
        int statesCount;

    public:
        //后面会将projects排一次序
        DFAState(const vector<Project> &project, int index, int elemCount);

        //拷贝构造函数
        DFAState(const DFAState &a);

        ~DFAState();
        //判断两个项目集是不是一样,一样的话返回true不一样的话返回false
        bool operator==(vector<Project> &b);
    };


    //预测分析表的内容
    //移进,归约,错误
    enum Type {
        SMOVE, REDUCE, ERROR, ACC
    };

    //SMOVE只需要一个状态，REDUCE需要表达式的位置,同时状态和表达式两者不会共存，应该为联合体
    struct ReduceByExpress {
        int elemIndex;
        int expressIndex;

    };

    union Choose {
        int state;
        ReduceByExpress R;
    };

    struct Action {
        Type type;
        Choose choose;
    };

    //继承FindTable主要是为了重写方法
    class GrammarLR : public Grammar {

    public:
        //各个元素
        vector<ElemRight> elements;
        //保存记录，主要的是计算First需要使用
        vector<int> firstTemp;
        //DFA状态
        vector<DFAState> states;
        // 减句柄的表,指导动作的表
        vector<vector<Action>> actionTable;

    public:
        //添加符号,可以是终结符也可以是非终结符
        int addElement(const string &name, bool isFinally);

    public://方法
        //构造函数
        GrammarLR();
    };
}
#endif