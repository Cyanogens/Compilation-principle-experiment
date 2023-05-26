/**
 *������ݹ�/������
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
 * ����ֱ����ݹ�
 * �ȼ��elems[i]��û��ֱ����ݹ�,�������Ҫ��������,������elems�������һЩ�µĶ���
 *
 * @param elemIndex
 * @param grammarLeft
 */
void clearLeft(int elemIndex, GrammarLL *grammarLeft) {
    //����ݹ��ʽ���±�
    vector<int> expressionIndex;
    //û����ݹ��ʽ���±�
    vector<int> notLeft;
    expressionIndex.clear();
    notLeft.clear();

    //��ֱ����ݹ�ѹ��ȥ
    for (int i = 0; i < grammarLeft->elements[elemIndex].expressionOfSet.size(); i++) {
        if (grammarLeft->elements[elemIndex].expressionOfSet[i][0] == elemIndex) {
            //�ж��Ƴ��ĵ�һλ���ַ��±��Ƿ���������ַ��±�
            expressionIndex.push_back(i);
        } else {
            notLeft.push_back(i);
        }
    }

    //����ֱ��ֱ����ݹ�,size������0˵������ֱ����ݹ�,��������
    if (!expressionIndex.empty()) {
        //�ȷ���һ��elems��λ�ó���
        stringstream ss;
        ss << grammarLeft->elements.size();
        grammarLeft->addElement(grammarLeft->elements[elemIndex].name + ss.str(), false);

        int newIndex = grammarLeft->findName(grammarLeft->elements[elemIndex].name + ss.str());

        vector<Express> temp = grammarLeft->elements[elemIndex].expressionOfSet;

        //����ԭ�����ķ�
        grammarLeft->elements[elemIndex].expressionOfSet.clear();

        for (int i : notLeft) {
            //����ǿմ�,��Ҫ���մ���ȥ
            if (temp[i].expression[0] == -1 && temp[i].expression.size() == 1) {
                temp.clear();
            }
            temp[i].expression.push_back(newIndex);
            grammarLeft->elements[elemIndex].addExpression(temp[i].expression);
        }

        //���µķ��ս����ӵ��ķ�
        //�����շ���
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
 * ���������ݹ�
 *
 * @param oldElemIndex
 * @param replaceIndex
 * @param grammarLeft
 */
void replaceExpress(int oldElemIndex, int replaceIndex, GrammarLL *grammarLeft) {

    //��һ�δ������ȡֱ����ݹ�����,���Ժϲ���
    //��replaceIndex��ʽ���±�
    vector<int> expressionIndex;
    //û��replaceIndex��ʽ���±�
    vector<int> notLeft;
    expressionIndex.clear();
    notLeft.clear();

    //��replaceIndexѹ��ȥ
    for (int i = 0; i < grammarLeft->elements[oldElemIndex].expressionOfSet.size(); i++) {
        if (grammarLeft->elements[oldElemIndex].expressionOfSet[i][0] == replaceIndex) {
            //���Ƴ����ֵĵ�һ���ַ�
            expressionIndex.push_back(i);
        } else {
            notLeft.push_back(i);
        }
    }

    //��ʼ�滻
    if (!expressionIndex.empty()) {
        //������Ҫ���
        vector<Express> temp = grammarLeft->elements[oldElemIndex].expressionOfSet;
        grammarLeft->elements[oldElemIndex].expressionOfSet.clear();
        //�Ƚ����øı�İ��ú�
        for (int i : notLeft) {
            grammarLeft->elements[oldElemIndex].addExpression(temp[i]);
        }
        //��Ҫ�ı��
        for (int i : expressionIndex) {
            //��׺
            vector<int> diff;
            diff.clear();
            //�����׺
            for (int j = 1; j < temp[i].expression.size(); j++) {
                diff.push_back(temp[i][j]);
            }
            //�滻�ı��ʽ����,��j+1�����ʽ
            for (int j = 0; j < grammarLeft->elements[replaceIndex].expressionOfSet.size(); j++) {
                vector<int> exTemp;
                exTemp.clear();
                //������ʽΪ��
                if (grammarLeft->elements[replaceIndex].expressionOfSet[j][0] == -1 &&
                    grammarLeft->elements[replaceIndex].expressionOfSet[j].expression.size() == 1) {
                    //ֱ��ѹ���׺����
                    for (int &index : diff) {
                        exTemp.push_back(index);
                    }
                    grammarLeft->elements[oldElemIndex].addExpression(exTemp);
                    continue;
                }
                //�����ʽѹ��,��һ���Ѿ����մ�ȥ��,�滻����
                for (int &index : grammarLeft->elements[replaceIndex].expressionOfSet[j].expression) {
                    exTemp.push_back(index);
                }
                //��׺����
                for (int &index : diff) {
                    exTemp.push_back(index);
                }
                grammarLeft->elements[oldElemIndex].addExpression(exTemp);
            }
        }
    }
}

/**
 * ������ݹ�
 *
 * @param grammarLeft
 */
void clearLeftRecursion(GrammarLL *grammarLeft) {

    //����ֱ����ݹ�
    for (int i = 0; i < grammarLeft->elements.size(); i++) {
        //���Ԫ�ز����ս����,��Ҫ�����޸�
        if (!grammarLeft->elements[i].isFinally) {
            clearLeft(i, grammarLeft);
        }
    }

    //���������ݹ�
    //�������ɹ���,��0���±���ǿ�ʼ����
    for (int i = 1; i < grammarLeft->elements.size(); i++) {
        //���elem[i]���ս������Ҫ
        if (!grammarLeft->elements[i].isFinally) {
            //����Ҫ��д���ʽ
            for (int j = 0; j < i; j++) {
                //ֻ�з��ս����Ҫ�ı�
                if (!grammarLeft->elements[j].isFinally) {
                    replaceExpress(i, j, grammarLeft);
                }
            }
        }
        //����ֱ����ݹ�
        clearLeft(i, grammarLeft);
    }
}

/**
 * ����������
 *
 * @param grammarLeft
 */
void clearLeftFactor(GrammarLL *grammarLeft) {
    vector<int> factory;
    int begin, count;

    for (int i = 0; i < grammarLeft->elements.size(); i++) {
        //ֻ�з��ս��Ҫ����������
        if (!grammarLeft->elements[i].isFinally) {
            while (grammarLeft->elements[i].extractLeftFactor(factory, begin, count)) {
                //i��������
                grammarLeft->updateElement(i, begin, begin + count, factory);
            }
        }
    }
}

GrammarLL *eliminate() {
    auto *grammarLeft = new GrammarLL();
    //������ݹ�
    clearLeftRecursion(grammarLeft);
    //����������
    clearLeftFactor(grammarLeft);
    grammarLeft->addElement("#", true);

    cout << "�����ı��ʽΪ:" << endl;
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


