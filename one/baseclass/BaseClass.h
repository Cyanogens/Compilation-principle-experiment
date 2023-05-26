#ifndef BASECLASS_H
#define BASECLASS_H
#define MAX 100
#define NFA "nfa"

#include <bits/stdc++.h>

namespace base_class {
    using namespace std;

    /**
    * ���ڵ�
    */
    struct Node {
        //�ڵ�����
        char type{};
        //Ҷ�ӽ�������ַ�
        char chars{};
        //������
        Node *lchild;
        //������
        Node *rchild;
        //��ʼ״̬
        int startState{};
        //��ֹ״̬
        int endState{};
    public:
        Node();

        //��װҶ�ӽڵ�
        explicit Node(char c);

        //��װ���ڵ�
        explicit Node(char type, Node *lchild, Node *rchild);
    };

    /**
     * ״̬��
     */
    struct StatesSet {
        int stateSet[MAX]{};
        int length = 0;
    };

    /**
    * ͼ�ľ����ʾ
    */
    struct Graph {
    public:
        //״̬��
        int stateSize;
        //����
        string type;
        //�����ַ�
        vector<char> absorbChar;
        //״̬��ת
        int skips[MAX][MAX]{};
        //��״̬��ת ÿ�е�һ�зŸ�״̬�Ŀ���ת��
        int emptySkips[MAX][MAX]{};
        //���
        int startState{};
        //�յ�(��nfa����,ֻ��һ���յ�)
        int endState{};
        //�յ㼯(����dfa����,�ж���յ�)
        StatesSet endSet;
    public:
        explicit Graph(vector<char> chars);

        void visitGraph();
    };
}
#endif