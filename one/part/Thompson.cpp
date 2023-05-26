/**
 * �ʷ�����
 *
 * @author Cyanogen
 * @date 2023-03-20 23:25
 */
#include <bits/stdc++.h>
#include "../baseclass/BaseClass.h"

using namespace std;
using namespace base_class;

/**
 * ��������﷨��
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
 * �����ת����Ӧ���ȼ�
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
            //���ǲ�����
            return 0;
    }
}

/**
 * �ж��Ƿ�������������
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
 * ����������ʽ
 *
 * @return
 */
string inputRegex() {
    cout << "������������ʽ��";
    string regex = "e*|(c|d*)";
    cin >> regex;
    regex.insert(0, "#");
    //���������
    regex.append("#");
    //��������ʽ�������ӷ�
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
 * �Ƚ���������ȼ�
 *
 * @param oddOp
 * @param newOp
 * @return
 */
bool isHigh(char oddOp, char newOp) {
    return processingOperPriority(oddOp) < processingOperPriority(newOp);
}

/**
 * ������������ӽڵ�
 *
 * @param op
 * @param charStack
 * @return
 */
Node *connectNodes(char op, deque<Node *> &charStack) {
    Node *chars1, *chars2;
    if (op == '|' || op == '&') {
        //�����Բ�����Ϊ���Ľ��
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
 * �����﷨��
 *
 * @param str
 * @return
 */
Node *syntaxTree(string str) {
    //����ջ���﷨ջ
    stack<char> operStack;
    deque<Node *> charStack;
    //��#ѹ�����ջ
    operStack.push(str[0]);
    //ɨ��(���˿�ͷ�ͽ�β��#)
    for (int i = 1; i < str.size() - 1; i++) {
        char c = str[i];
        //�﷨����
        if (!processingOperPriority(c)) {
            //��װҶ�ӽڵ�,ѹ���﷨ջ
            charStack.push_back(new Node(c));
        } else {
            //������
            if (c == '(') {
                //ֱ����ջ
                operStack.push(c);
            } else if (c == ')') {
                //��������ջ����'('��Ĳ�����������ؽڵ��������
                while (operStack.top() != '(') {
                    char op = operStack.top();
                    operStack.pop();
                    Node *node = connectNodes(op, charStack);
                    charStack.push_back(node);
                }
                operStack.pop();
            } else if (c == '*' || c == '+') {
                //������������,ֱ�Ӳ���
                Node *node = connectNodes(c, charStack);
                charStack.push_back(node);
            } else {
                while (!isHigh(operStack.top(), c)) {
                    //��ǰ���������ȼ�����ջ�����������ȼ�
                    //�������ȼ��Ĳ�������صĽ������Ӵ���
                    char op = operStack.top();
                    operStack.pop();
                    //�õ��µĸ��ڵ�
                    Node *node = connectNodes(op, charStack);
                    charStack.push_back(node);
                }
                //ѹ�����ջ
                operStack.push(c);
            }
        }
    }
    //ɨ��ȫ������ ��ջ��ʣ�ಿ������
    while (operStack.top() != '#') {
        char op = operStack.top();
        operStack.pop();
        Node *node = connectNodes(op, charStack);
        charStack.push_back(node);
    }
    return charStack.front();
}

/**
 * ���ַ������м�����Ԫ��
 * ���Ѿ�����,�򷵻�������
 *
 * @param chars
 * @param a
 * @param size
 * @return
 */
int addFindChar(vector<char> &chars, char a) {
    for (int i = 0; i < chars.size(); i++) {
        if (chars[i] == a) {
            //���ض�Ӧ�±�
            return i;
        }
    }
    //û�ҵ�,���
    chars.push_back(a);
    return MAX;
}

/**
 * ��ȡ����ʽ���������
 *
 * @param str
 * @param chars
 * @return
 */
int getAbsorbChar(const string &str, vector<char> &chars) {
    for (char i : str) {
        //���ǲ�����
        if (!processingOperPriority(i)) {
            addFindChar(chars, i);
        }
    }
    return chars.size();
}

/**
 * ����µ�״̬��ת
 *
 * @param graph ͼ
 * @param begin ���
 * @param end �յ�
 * @param c �����ַ�
 */
void addSkips(Graph &graph, int begin, int end, char c) {
    graph.skips[begin][addFindChar(graph.absorbChar, c)] = end;
}

/**
 * ����µĿ���ת
 *
 * @param graph ͼ
 * @param begin ���
 * @param end �յ�
 */
void addEmptySkips(Graph &graph, int begin, int end) {
    //����ת����һ
    graph.emptySkips[begin][++graph.emptySkips[begin][0]] = end;
}

/**
 * ����Ҷ�ӽڵ�״̬ת��
 *
 * @param graph
 * @param node
 */
void leafSkip(Graph &graph, Node &node) {
    node.startState = graph.stateSize++;
    node.endState = graph.stateSize++;
    //���ʼĩ״̬ת��
    addSkips(graph, node.startState, node.endState, node.chars);
}

/**
 * �����״̬ת��
 *
 * @param graph
 * @param node
 */
void rootSkip(Graph &graph, Node &node) {
    if (node.type == '|') {
        //�������
        node.startState = graph.stateSize++;
        node.endState = graph.stateSize++;
        addEmptySkips(graph, node.startState, node.lchild->startState);
        addEmptySkips(graph, node.startState, node.rchild->startState);
        addEmptySkips(graph, node.lchild->endState, node.endState);
        addEmptySkips(graph, node.rchild->endState, node.endState);
    } else if (node.type == '*') {
        //�հ�����
        node.startState = graph.stateSize++;
        node.endState = graph.stateSize++;
        addEmptySkips(graph, node.startState, node.lchild->startState);
        addEmptySkips(graph, node.startState, node.endState);
        addEmptySkips(graph, node.lchild->endState, node.lchild->startState);
        addEmptySkips(graph, node.lchild->endState, node.endState);
    } else if (node.type == '+') {
        //���հ�����
        node.startState = graph.stateSize++;
        node.endState = graph.stateSize++;
        addEmptySkips(graph, node.startState, node.lchild->startState);
        addEmptySkips(graph, node.lchild->endState, node.lchild->startState);
        addEmptySkips(graph, node.lchild->endState, node.endState);
    } else if (node.type == '&') {
        //���ӹ���
        node.startState = node.lchild->startState;
        node.endState = node.rchild->endState;
        for (int i = 0; i < graph.absorbChar.size(); i++) {
            graph.skips[node.lchild->endState][i] = graph.skips[node.rchild->startState][i];
            graph.skips[node.rchild->startState][i] = -1;
            graph.emptySkips[node.lchild->endState][i] = graph.emptySkips[node.rchild->startState][i];
            graph.emptySkips[node.rchild->startState][i] = -1;
        }
        node.rchild->startState = node.lchild->startState;
        node.lchild->endState = node.rchild->endState;
    } else {
        cout << "fault!";
    }
}

/**
 * �ݹ鹹��NFA
 *
 * @param tree
 * @param graph
 * @param str
 */
void makeNFA(Node *tree, Graph &graph, const string &str) {
    //����״̬ת�ƾ���
    if (tree->type == 'L') {
        //Ҷ��״̬ת��
        leafSkip(graph, *tree);
    } else {
        //����������״̬ת��
        makeNFA(tree->lchild, graph, str);
        //����������״̬ת��
        if (tree->rchild != nullptr) {
            makeNFA(tree->rchild, graph, str);
        }
        //�����״̬ת��
        rootSkip(graph, *tree);
    }
}

/**
 * ����NFA
 *
 * @param tree
 * @param graph
 * @param str
 */
Graph constructNFA(Node *tree, const string &str) {
    //�õ����շ���
    vector<char> chars;
    //������ʽ�е��ַ�
    getAbsorbChar(str, chars);
    //��ʼ��NFA
    auto *graph = new Graph(chars);
    graph->type = NFA;
    //����
    makeNFA(tree, *graph, str);
    graph->startState = tree->startState;
    graph->endState = tree->endState;
    return *graph;
}

/**
 * ����NFA
 */
Graph toMakeNFA() {
    string regex = inputRegex();
    cout << "����ʽΪ:" << regex << endl;
    //�����﷨��
    Node *tree = syntaxTree(regex);
    cout << endl;
    //����NFA
    cout << endl << "Thompson�㷨����NFA" << endl;
    Graph nfa = constructNFA(tree, regex);
    nfa.visitGraph();
    return nfa;
}