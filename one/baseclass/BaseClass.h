#ifndef BASECLASS_H
#define BASECLASS_H
#define MAX 100
#define NFA "nfa"

#include <bits/stdc++.h>

namespace base_class {
    using namespace std;

    /**
    * 树节点
    */
    struct Node {
        //节点类型
        char type{};
        //叶子结点代表的字符
        char chars{};
        //左子树
        Node *lchild;
        //右子树
        Node *rchild;
        //起始状态
        int startState{};
        //终止状态
        int endState{};
    public:
        Node();

        //包装叶子节点
        explicit Node(char c);

        //包装根节点
        explicit Node(char type, Node *lchild, Node *rchild);
    };

    /**
     * 状态集
     */
    struct StatesSet {
        int stateSet[MAX]{};
        int length = 0;
    };

    /**
    * 图的矩阵表示
    */
    struct Graph {
    public:
        //状态数
        int stateSize;
        //吸收字符种类数
        int charTypeNum;
        //类型
        string type;
        //吸收字符
        char absorbChar[MAX]{};
        //状态跳转
        int skips[MAX][MAX]{};
        //空状态跳转 每行第一列放该状态的空跳转数
        int emptySkips[MAX][MAX]{};
        //起点
        int startState{};
        //终点(对nfa而言,只有一个终点)
        int endState{};
        //终点集(对于dfa而言,有多个终点)
        StatesSet endSet;
    public:
        Graph(const char *chars, int charSize);

        void visitGraph();
    };
}
#endif