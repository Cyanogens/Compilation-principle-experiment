#include "BaseClass.h"

using namespace base_class;

Node::Node() : lchild(nullptr), rchild(nullptr) {}

//��װҶ�ӽڵ�
Node::Node(char c) : lchild(nullptr), rchild(nullptr), chars(c), type('L') {}

//��װ���ڵ�
Node::Node(char type, Node *lchild, Node *rchild) : lchild(lchild), rchild(rchild), type(type) {}

/**
 * ��ʼ��״̬ת�ƾ���
 *
 * @param graph
 * @param chars
 * @param charSize
 */
Graph::Graph(vector<char> chars) {
    this->stateSize = 0;
    this->absorbChar.insert(this->absorbChar.end(), chars.begin(), chars.end());
    for (auto &skip : this->skips) {
        for (int &j : skip) {
            //��ʾ����ת
            j = MAX;
        }
    }
    for (auto &emptySkip : this->emptySkips) {
        //����ת����Ϊ0
        emptySkip[0] = 0;
    }
}

/**
* ͼ�ı���
*
* @param graph
* @param type
*/
void Graph::visitGraph() {

    cout << "״̬��ת����:" << endl;
    cout << "״̬" << "\t\t";
    for (char i : this->absorbChar) {
        cout << i;
        cout<<"\t\t";
    }
    cout << "��" << endl;
    for (int i = 0; i < this->stateSize; i++) {
        if (this->skips[i][0] == -1) {
            continue;
        }
        cout << i;

        for (int j = 0; j < this->absorbChar.size(); j++) {
            if (this->skips[i][j] == MAX) {
                cout << "\t\t" << " ";
            } else {
                cout << "\t\t"<<"{" << this->skips[i][j]<<"}";
            }
        }


        //�������ת
        cout << "\t\t" << " ";
        if (this->emptySkips[i][0] > 0) {
            cout << "{";
            for (int j = 0; j < this->emptySkips[i][0]; j++) {
                cout << this->emptySkips[i][j + 1];
                if (j != this->emptySkips[i][0] - 1) {
                    cout << ", ";
                }
            }
            cout << "}";
        }
        cout << endl;
    }
    cout << "����,���:" << this->startState << " ";
    if (this->type == NFA) {
        cout << "�յ�:" << this->endState << " ";
    } else {
        cout << "�յ�:";
        for (int i = 0; i < this->endSet.length; i++) {
            cout << this->endSet.stateSet[i] << " ";
        }
    }
    cout << endl;
}