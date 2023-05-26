#ifndef LL_H_
#define LL_H_

#include <bits/stdc++.h>
#include "../../baseclass/Base.h"

using namespace std;

//LL���ķ�������,��Ҫ�Ƿ����Ĳ�ͬ
namespace ll_grammar {
    using namespace base_grammar;

    //LL�ķ��ű�ʾ
    class ElemLeft : public Elem {
    public:
        //��¼�Ƴ����ʽ���ַ����±�
        vector<Express> expressionOfSet;
    public:
        bool addExpression(const Express &expression);

        /**
         * ���캯��
         *
         * @param name
         * @param key
         * @param isFinally
         */
        ElemLeft(string name, int key, bool isFinally);

        /**
         * ��ȡ������
         *
         * @param result
         * @param begin
         * @param count
         * @return true:��������
         */
        bool extractLeftFactor(vector<int> &result, int &begin, int &count);
    };

    class GrammarLL : public Grammar {

    public:
        //��¼�����ַ�
        vector<ElemLeft> elements;
        //LL����Ԥ���
        //��ά����,��һ��Ϊ���ս����
        vector<vector<int>> predictTable;
        //�����¼,��Ҫ���Ǽ���First��Ҫʹ��
        vector<int> firstTemp;

    public:
        GrammarLL();

        /**
         * ��ӷ���,�������ս��Ҳ�����Ƿ��ս��
         *
         * @param name
         * @param isFinally
         * @return
         */
        int addElement(const string &name, bool isFinally);

        void updateElement(int elemIndex, int begin, int end, vector<int> &factory);
    };
}
#endif
