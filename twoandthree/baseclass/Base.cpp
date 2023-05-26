#include "Base.h"

using namespace base_grammar;

Elem::Elem(string name, int key, bool isFinally) : name(std::move(name)),key(key) ,isFinally(isFinally) {}

Express::Express(int *expressions, int length) : expression(expressions, expressions + length) {}

Express::Express(const vector<int> &expression) : expression(expression.begin(), expression.end()) {}

int &Express::operator[](int index) {
    return expression[index];
}


//根据字符查找相应下标
//找不到则返回-2
//-1表示为空,空使用@来表示
int Grammar::findName(const string &nameOfElem) {
    int result = -2;
    if (nameOfElem == "@") {
        result = -1;
    } else {
        if (this->nameOfKeys.find(nameOfElem) != nameOfKeys.end()) {
            result = nameOfKeys[nameOfElem];
        }
    }
    return result;
}

void public_tool::clearNull(const vector<int> &a, vector<int> &result, int value) {
    result.clear();
    for (int i : a) {
        if (i != value) {
            result.push_back(i);
        }
    }
}

vector<string> &public_tool::parseProductions(string ex, vector<string> &productions) {
    productions.clear();
    string buffer;
    //下标
    int index = 0;

    //寻找非终结符下标,是以->结束的
    for (; index < ex.length(); index++) {
        if (ex[index] == '-') {
            if (ex[index + 1] == '>') {
                index = index + 2;
                break;
            }
        }
        buffer += ex[index];
    }
    productions.push_back(buffer);
    buffer.clear();

    //解析各个表达式;
    for (int i = index; i < ex.length(); i++) {
        if (ex[i] == '?') {
            buffer += '?' + ex[++i];
        }
        //一个表达式完成
        if (ex[i] == '|') {
            i++;
            productions.push_back(buffer);
            buffer = "";
        }
        buffer += ex[i];

    }
    if (!buffer.empty()) {
        //存在推出
        productions.push_back(buffer);
        buffer.clear();
    }
    return productions;
}

vector<string> &public_tool::parseElem(string production, vector<string> &elems) {
    elems.clear();
    string temp;

    //遍历全字符
    for (int i = 0; i < production.length(); i++) {

        if (production[i] != '?' && production[i] != '$') {
            temp = production[i];
            elems.push_back(temp);
            continue;
        }

        //转义字符修改一下,可能遇见的有?$,??
        if (production[i] == '?') {
            temp = production[++i];
            elems.push_back(temp);
        }

        //到这一步说明前面没有?,后面是一个长的字符串
        if (production[i] == '$') {
            temp = "";
            while (production[++i] != '$') {
                if (production[i] == '?') {
                    temp += production[++i];
                }
                temp += production[i];
            }
            elems.push_back(temp);

        }

    }
    return elems;
}

//合并向量a和向量b,没有重复的结果放在向量a中
void public_tool::connectVector(vector<int> &a, const vector<int> &b) {
    for (int i : b) {
        if (!isInVector(a, i)) {
            a.push_back(i);
        }
    }
}

//判断b是不是在向量a中
//在的话返回true,不在的话返回false
bool public_tool::isInVector(const vector<int> &a, int b) {
    bool result = false;
    for (int i : a) {
        if (i == b) {
            result = true;
            break;
        }
    }
    return result;
}



