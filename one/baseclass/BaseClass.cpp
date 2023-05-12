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
Graph::Graph(const char *chars, int charSize) {
    this->stateSize = 0;
    this->charTypeNum = charSize;
    for (int i = 0; i < charSize; i++) {
        this->absorbChar[i] = chars[i];
    }
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
    cout << "状态" << setw(6);
    for (int i = 0; i < this->charTypeNum; i++) {
        cout << this->absorbChar[i] << setw(6);
    }
    cout << setw(7) << "ε" << endl;
    for (int i = 0; i < this->stateSize; i++) {
        cout << setw(3) << i;
        for (int j = 0; j < this->charTypeNum; j++) {
            if (this->skips[i][j] == MAX) {
                cout << setw(6) << "-";
            } else {
                cout << setw(6) << this->skips[i][j];
            }
        }
        //输出空跳转
        cout << setw(6) << " ";
        for (int j = 0; j < this->emptySkips[i][0]; j++)
            cout << this->emptySkips[i][j + 1] << " ";
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