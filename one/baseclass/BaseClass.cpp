#include "BaseClass.h"

using namespace base_class;

Node::Node() : lchild(nullptr), rchild(nullptr) {}

//包装叶子节点
Node::Node(char c) : lchild(nullptr), rchild(nullptr), chars(c), type('L') {}

//包装根节点
Node::Node(char type, Node *lchild, Node *rchild) : lchild(lchild), rchild(rchild), type(type) {}

/**
 * 初始化状态转移矩阵
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
            //表示无跳转
            j = MAX;
        }
    }
    for (auto &emptySkip : this->emptySkips) {
        //空跳转个数为0
        emptySkip[0] = 0;
    }
}

/**
* 图的遍历
*
* @param graph
* @param type
*/
void Graph::visitGraph() {

    cout << "状态跳转矩阵:" << endl;
    cout << "状态" << "\t\t";
    for (char i : this->absorbChar) {
        cout << i;
        cout<<"\t\t";
    }
    cout << "ε" << endl;
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


        //输出空跳转
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
    cout << "其中,起点:" << this->startState << " ";
    if (this->type == NFA) {
        cout << "终点:" << this->endState << " ";
    } else {
        cout << "终点:";
        for (int i = 0; i < this->endSet.length; i++) {
            cout << this->endSet.stateSet[i] << " ";
        }
    }
    cout << endl;
}