#ifndef LR_H_
#define LR_H_


#include <bits/stdc++.h>
#include "../../baseclass/Base.h"

using namespace std;

//LR���ķ�������,��Ҫ�Ƿ����Ĳ�ͬ
namespace lr_grammar {
    using namespace base_grammar;

    //LR�ķ��ı��ʽ
    class ExpressLR : public Express {
    public://����
        //���캯��
        ExpressLR(int *expressions, int length);

        ExpressLR(const vector<int> &expression);
    };

    //LR�ķ��ű�ʾ
    class ElemRight : public Elem {
    public:
        vector<ExpressLR> expressionOfSet;
    public:
        //���캯��
        ElemRight(string name, int key, bool isFinally);
        //��ӱ��ʽ
        bool addExpression(const ExpressLR &expression);
    };

    //LR����Ŀ
    class Project {
    public:
        int elemKey;
        int expressIndex;
        int projectPoint;
    public:
        Project(int elemKey, int expressIndex, int point);

        bool operator<(const Project &project) const;

        bool operator==(const Project &project) const;

        bool operator!=(const Project &project) const;
    };

    class DFAState {
    public:
        //��Ŀ��
        vector<Project> projects;

        //״̬ת�Ƶģ�next�ĳ���ΪelemԪ�صĳ���
        int *next;
        int elementCount;
        //״̬���ϵ��±꣬��Ҫ��Ϊ�����Ч��;
        int statesCount;

    public:
        //����Ὣprojects��һ����
        DFAState(const vector<Project> &project, int index, int elemCount);

        //�������캯��
        DFAState(const DFAState &a);

        ~DFAState();
        //�ж�������Ŀ���ǲ���һ��,һ���Ļ�����true��һ���Ļ�����false
        bool operator==(vector<Project> &b);
    };


    //Ԥ������������
    //�ƽ�,��Լ,����
    enum Type {
        SMOVE, REDUCE, ERROR, ACC
    };

    //SMOVEֻ��Ҫһ��״̬��REDUCE��Ҫ���ʽ��λ��,ͬʱ״̬�ͱ��ʽ���߲��Ṳ�棬Ӧ��Ϊ������
    struct ReduceByExpress {
        int elemIndex;
        int expressIndex;

    };

    union Choose {
        int state;
        ReduceByExpress R;
    };

    struct Action {
        Type type;
        Choose choose;
    };

    //�̳�FindTable��Ҫ��Ϊ����д����
    class GrammarLR : public Grammar {

    public:
        //����Ԫ��
        vector<ElemRight> elements;
        //�����¼����Ҫ���Ǽ���First��Ҫʹ��
        vector<int> firstTemp;
        //DFA״̬
        vector<DFAState> states;
        // ������ı�,ָ�������ı�
        vector<vector<Action>> actionTable;

    public:
        //��ӷ���,�������ս��Ҳ�����Ƿ��ս��
        int addElement(const string &name, bool isFinally);

    public://����
        //���캯��
        GrammarLR();
    };
}
#endif