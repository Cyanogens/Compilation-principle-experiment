#ifndef BASE_H_
#define BASE_H_

#include <bits/stdc++.h>

//基础类
namespace base_grammar {
    using namespace std;
    //表达式,产生式子
    class Express {
    public:
        //表达式
        vector<int> expression;
    public://方法
        Express(int *expressions, int length);

        Express(const vector<int> &expression);

        //[]运算符重载
        int &operator[](int index);
    };

    //符号,包括终结符和非终结符号,使用模板是因考虑到LL和LR的表达式不一样的而采用的
    class Elem {
    public:
        int key;
        //符号名
        string name;
        //判断是不终结符,是则为true
        bool isFinally;
    public:
        //构造函数
        Elem(string name, int key, bool isFinally);
    };

    class Grammar{
    public:
        //利用map来快速查找
        map<string, int> nameOfKeys;
        //终结符下标集合
        vector<int> finallyCharIndex;
        //非终结符下标集合
        vector<int> notFinallyCharIndex;
        //vector行代表非终结符下标
        //vector<int>里的值对应终结符下标
        vector<vector<int>> first;
        vector<vector<int>> follow;
    public:
        int findName(const string &nameOfElem);
    };
}

//解析字符串,主要是一些函数
namespace public_tool {
    using namespace std;

    //解析所有的产生式
    vector<string> &parseProductions(string ex, vector<string> &productions);/**/

    //单独解析
    vector<string> &parseElem(string production, vector<string> &elems);

    void clearNull(const vector<int> &a, vector<int> &result, int value = -1);

    bool isInVector(const vector<int> &a, int b);

    void connectVector(vector<int> &a, const vector<int> &b);

}

#endif
