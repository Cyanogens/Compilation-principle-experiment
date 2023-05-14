/**
 * 词法分析
 *
 * @author Cyanogen
 * @date 2023-03-20 23:25
 */
#include <bits/stdc++.h>
#include "../baseclass/BaseClass.h"

using namespace std;
using namespace base_class;


/**
 * 后序遍历语法树
 *
 * @param Tree
 */
void visitTree(Node *Tree) {
    if (Tree->type == 'L') {
        cout << Tree->chars << " ";
        return;
    } else {
        visitTree(Tree->lchild);
        if (Tree->rchild != nullptr) {
            visitTree(Tree->rchild);
        }
        cout << Tree->type << " ";
        return;
    }
}

/**
 * 运算符转换对应优先级
 *
 * @param o
 * @return
 */
int processingOperPriority(char o) {
    switch (o) {
        case '#':
            return 1;
        case '(':
            return 2;
        case '|':
            return 3;
        case '&':
            return 4;
        case ')':
            return 5;
        case '+':
        case '*':
            return 6;
        default :
            //不是操作符
            return 0;
    }
}

/**
 * 判断是否加入连接运算符
 *
 * @param l
 * @param r
 * @return
 */
bool isInsert(char l, char r) {
    return
        //char+(
            (!processingOperPriority(l) && r == '(') ||
            //char+char
            (!processingOperPriority(l) && !processingOperPriority(r)) ||
            //)+char
            (l == ')' && !processingOperPriority(r)) ||
            //*+char
            (l == '*' && !processingOperPriority(r)) ||
            //++char
            (l == '+' && !processingOperPriority(r)) ||
            //)+(
            (l == ')' && r == '(') ||
            //*+(
            (l == '*' && r == '(') ||
            //++(
            (l == '+' && r == '(');
}

/**
 * 输入正则表达式
 *
 * @return
 */
string inputRegex() {
    cout << "请输入正则表达式：";
    string regex = "(a|b)+(a|b)*";
//    string regex = "(ab|c)*";
    //cin >> regex;
    regex.insert(0, "#");
    //加入结束符
    regex.append("#");
    //将正则表达式加入连接符
    int l = 0, r = 1;
    while (regex[r] != '#') {
        if (isInsert(regex[l], regex[r])) {
            regex.insert(l + 1, "&");
        }
        l++;
        r++;
    }
    return regex;
}

/**
 * 比较运算符优先级
 *
 * @param oddOp
 * @param newOp
 * @return
 */
bool isHigh(char oddOp, char newOp) {
    return processingOperPriority(oddOp) < processingOperPriority(newOp);
}

/**
 * 根据运算符连接节点
 *
 * @param op
 * @param charStack
 * @return
 */
Node *connectNodes(char op, deque<Node *> &charStack) {
    Node *chars1, *chars2;
    if (op == '|' || op == '&') {//创建以操作符为根的结点
        chars1 = charStack.back();
        charStack.pop_back();
        chars2 = charStack.back();
        charStack.pop_back();
        return new Node(op, chars2, chars1);
    } else {
        chars1 = charStack.back();
        charStack.pop_back();
        return new Node(op, chars1, nullptr);
    }
}

/**
 * 构造语法树
 *
 * @param str
 * @return
 */
Node *syntaxTree(string str) {
    //符号栈和语法栈
    stack<char> operStack;
    deque<Node *> charStack;
    //将#压入符号栈
    operStack.push(str[0]);
    //扫描(除了开头和结尾的#)
    for (int i = 1; i < str.size() - 1; i++) {
        char c = str[i];
        //语法内容
        if (!processingOperPriority(c)) {
            //包装叶子节点,压入语法栈
            charStack.push_back(new Node(c));
        } else {
            //操作符
            if (c == '(') {
                //直接入栈
                operStack.push(c);
            } else if (c == ')') {
                //将操作符栈中在'('后的操作符及其相关节点进行链接
                while (operStack.top() != '(') {
                    char op = operStack.top();
                    operStack.pop();
                    Node *node = connectNodes(op, charStack);
                    charStack.push_back(node);
                }
                operStack.pop();
            } else if (c == '*' || c == '+') {
                //遇到最高运算符,直接操作
                Node *node = connectNodes(c, charStack);
                charStack.push_back(node);
            } else {
                while (!isHigh(operStack.top(), c)) {
                    //当前操作符优先级低于栈顶操作符优先级
                    //将高优先级的操作符相关的进行链接处理
                    char op = operStack.top();
                    operStack.pop();
                    //得到新的根节点
                    Node *node = connectNodes(op, charStack);
                    charStack.push_back(node);
                }
                //压入符号栈
                operStack.push(c);
            }
        }
    }
    //扫描全部结束 将栈内剩余部分连接
    while (operStack.top() != '#') {
        char op = operStack.top();
        operStack.pop();
        Node *node = connectNodes(op, charStack);
        charStack.push_back(node);
    }
    return charStack.front();
}

/**
 * 在字符数组中加入新元素
 * 若已经存在,则返回数组编号
 *
 * @param chars
 * @param a
 * @param size
 * @return
 */
int addFindChar(vector<char> &chars, char a) {
    for (int i = 0; i < chars.size(); i++) {
        if (chars[i] == a) {
            //返回对应下标
            return i;
        }
    }
    //没找到,添加
    chars.push_back(a);
    return MAX;
}

/**
 * 获取正规式的输入符号
 *
 * @param str
 * @param chars
 * @return
 */
int getAbsorbChar(const string &str, vector<char> &chars) {
    for (char i : str) {
        //不是操作符
        if (!processingOperPriority(i)) {
            addFindChar(chars, i);
        }
    }
    return chars.size();
}

/**
 * 添加新的状态跳转
 *
 * @param graph 图
 * @param begin 起点
 * @param end 终点
 * @param c 输入字符
 */
void addSkips(Graph &graph, int begin, int end, char c) {
    graph.skips[begin][addFindChar(graph.absorbChar, c)] = end;
}

/**
 * 添加新的空跳转
 *
 * @param graph 图
 * @param begin 起点
 * @param end 终点
 */
void addEmptySkips(Graph &graph, int begin, int end) {
    //空跳转数加一
    graph.emptySkips[begin][0]++;
    graph.emptySkips[begin][graph.emptySkips[begin][0]] = end;
}

/**
 * 构造叶子节点状态转移
 *
 * @param graph
 * @param node
 */
void leafSkip(Graph &graph, Node &node) {
    node.startState = graph.stateSize++;
    node.endState = graph.stateSize++;
    addSkips(graph, node.startState, node.endState, node.chars); //添加始末状态转移
}

/**
 * 构造根状态转移
 *
 * @param graph
 * @param node
 */
void rootSkip(Graph &graph, Node &node) {
    if (node.type == '|') {
        //或根构造
        node.startState = graph.stateSize;
        graph.stateSize++;
        node.endState = graph.stateSize;
        graph.stateSize++;
        addEmptySkips(graph, node.startState, node.lchild->startState);
        addEmptySkips(graph, node.startState, node.rchild->startState);
        addEmptySkips(graph, node.lchild->endState, node.endState);
        addEmptySkips(graph, node.rchild->endState, node.endState);
    } else if (node.type == '*') {
        //闭包构造
        node.startState = graph.stateSize;
        graph.stateSize++;
        node.endState = graph.stateSize;
        graph.stateSize++;
        addEmptySkips(graph, node.startState, node.lchild->startState);
        addEmptySkips(graph, node.startState, node.endState);
        addEmptySkips(graph, node.lchild->endState, node.lchild->startState);
        addEmptySkips(graph, node.lchild->endState, node.endState);
    } else if (node.type == '+') {
        //正闭包构造
        node.startState = graph.stateSize;
        graph.stateSize++;
        node.endState = graph.stateSize;
        graph.stateSize++;
        addEmptySkips(graph, node.startState, node.lchild->startState);
        addEmptySkips(graph, node.lchild->endState, node.lchild->startState);
        addEmptySkips(graph, node.lchild->endState, node.endState);
    } else if (node.type == '&') {
        //连接构造
        node.startState = node.lchild->startState;
        node.endState = node.rchild->endState;
        addEmptySkips(graph, node.lchild->endState, node.rchild->startState);
    } else {
        cout << "fault!";
    }
}

/**
 * 递归构造NFA
 *
 * @param tree
 * @param graph
 * @param str
 */
void makeNFA(Node *tree, Graph &graph, const string &str) {
    //构造状态转移矩阵
    if (tree->type == 'L') {
        //叶子状态转移
        leafSkip(graph, *tree);
    } else {
        //构造左子树状态转移
        makeNFA(tree->lchild, graph, str);
        //构造右子树状态转移
        if (tree->rchild != nullptr) {
            makeNFA(tree->rchild, graph, str);
        }
        //构造根状态转移
        rootSkip(graph, *tree);
    }
}

/**
 * 构造NFA
 *
 * @param tree
 * @param graph
 * @param str
 */
Graph constructNFA(Node *tree, const string &str) {
    //得到吸收符号
    vector<char> chars;
    //正则表达式中的字符
    getAbsorbChar(str, chars);
    //初始化NFA
    auto *graph = new Graph(chars);
    graph->type = NFA;
    //构造
    makeNFA(tree, *graph, str);
    graph->startState = tree->startState;
    graph->endState = tree->endState;
    return *graph;
}

/**
 * 生成NFA
 */
void toMakeNFA() {
    string regex = inputRegex();
    cout << "正规式为:" << regex << endl;
    //构建语法树
    Node *tree = syntaxTree(regex);
    //遍历语法树
    cout << "后序遍历语法树: ";
    visitTree(tree);
    cout << endl;
    //构造NFA
    cout << endl << "-----------Thompson算法构造NFA-----------" << endl;
    Graph nfa = constructNFA(tree, regex);
    nfa.visitGraph();
}