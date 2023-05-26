#include "Base.h"

using namespace base_grammar;

Elem::Elem(string name, int key, bool isFinally) : name(std::move(name)),key(key) ,isFinally(isFinally) {}

Express::Express(int *expressions, int length) : expression(expressions, expressions + length) {}

Express::Express(const vector<int> &expression) : expression(expression.begin(), expression.end()) {}

int &Express::operator[](int index) {
    return expression[index];
}


//�����ַ�������Ӧ�±�
//�Ҳ����򷵻�-2
//-1��ʾΪ��,��ʹ��@����ʾ
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
    //�±�
    int index = 0;

    //Ѱ�ҷ��ս���±�,����->������
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

    //�����������ʽ;
    for (int i = index; i < ex.length(); i++) {
        if (ex[i] == '?') {
            buffer += '?' + ex[++i];
        }
        //һ�����ʽ���
        if (ex[i] == '|') {
            i++;
            productions.push_back(buffer);
            buffer = "";
        }
        buffer += ex[i];

    }
    if (!buffer.empty()) {
        //�����Ƴ�
        productions.push_back(buffer);
        buffer.clear();
    }
    return productions;
}

vector<string> &public_tool::parseElem(string production, vector<string> &elems) {
    elems.clear();
    string temp;

    //����ȫ�ַ�
    for (int i = 0; i < production.length(); i++) {

        if (production[i] != '?' && production[i] != '$') {
            temp = production[i];
            elems.push_back(temp);
            continue;
        }

        //ת���ַ��޸�һ��,������������?$,??
        if (production[i] == '?') {
            temp = production[++i];
            elems.push_back(temp);
        }

        //����һ��˵��ǰ��û��?,������һ�������ַ���
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

//�ϲ�����a������b,û���ظ��Ľ����������a��
void public_tool::connectVector(vector<int> &a, const vector<int> &b) {
    for (int i : b) {
        if (!isInVector(a, i)) {
            a.push_back(i);
        }
    }
}

//�ж�b�ǲ���������a��
//�ڵĻ�����true,���ڵĻ�����false
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



