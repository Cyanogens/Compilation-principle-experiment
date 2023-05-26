#ifndef BASE_H_
#define BASE_H_

#include <bits/stdc++.h>

//������
namespace base_grammar {
    using namespace std;
    //���ʽ,����ʽ��
    class Express {
    public:
        //���ʽ
        vector<int> expression;
    public://����
        Express(int *expressions, int length);

        Express(const vector<int> &expression);

        //[]���������
        int &operator[](int index);
    };

    //����,�����ս���ͷ��ս����,ʹ��ģ�������ǵ�LL��LR�ı��ʽ��һ���Ķ����õ�
    class Elem {
    public:
        int key;
        //������
        string name;
        //�ж��ǲ��ս��,����Ϊtrue
        bool isFinally;
    public:
        //���캯��
        Elem(string name, int key, bool isFinally);
    };

    class Grammar{
    public:
        //����map�����ٲ���
        map<string, int> nameOfKeys;
        //�ս���±꼯��
        vector<int> finallyCharIndex;
        //���ս���±꼯��
        vector<int> notFinallyCharIndex;
        //vector�д�����ս���±�
        //vector<int>���ֵ��Ӧ�ս���±�
        vector<vector<int>> first;
        vector<vector<int>> follow;
    public:
        int findName(const string &nameOfElem);
    };
}

//�����ַ���,��Ҫ��һЩ����
namespace public_tool {
    using namespace std;

    //�������еĲ���ʽ
    vector<string> &parseProductions(string ex, vector<string> &productions);/**/

    //��������
    vector<string> &parseElem(string production, vector<string> &elems);

    void clearNull(const vector<int> &a, vector<int> &result, int value = -1);

    bool isInVector(const vector<int> &a, int b);

    void connectVector(vector<int> &a, const vector<int> &b);

}

#endif
