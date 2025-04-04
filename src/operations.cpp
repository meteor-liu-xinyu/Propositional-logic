#include "operations.h"

Operations::Operations()
{
}

Operations::~Operations()
{
}

int Operations::FindRight(const string& temp) // 找到0开始第一个右括号的位置
{
    int i = 0;
    while (temp[i] != ')')
    {
        i++;
    }
    return i;
}

int Operations::FindLeft(const string& temp) // 找到0开始第一个右括号对应的左括号的位置
{
    int i = FindRight(temp); // 找到0开始第一个右括号的位置
    while (temp[i] != '(')
    {
        i--;
    }
    return i;
}

int Operations::Countone(string terms) // 计算字符串中1的个数
{
    int count = 0;
    for (const auto& term : terms)
    {
        if (term == '1')
        {
            count++;
        }
    }
    return count;
}

bool Operations::IfNear(const string& a, const string& b) // 判断两个字符串是否只有一位不同
{
    int diffCount = 0;
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i])
        {
            diffCount++;
        }
        if (diffCount > 1)
        {
            return false;
        }
    }
    return true;
}

string Operations::Combine(const string& a, const string& b) // 合并两个字符串，将不同的位置替换为'-'
{
    string combined = a;
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i])
        {
            combined[i] = '-';
        }
    }
    return combined;
}

int Operations::CountDashes(const string& str) // 计算字符串中'-'的个数
{
    int count = 0;
    for (int i = 0; i < str.size(); i++)
    {
        if (str[i] == '-')
        {
            count++;
        }
    }
    return count;
}

char Operations::Not(char A)
{
    if (A == 'T')
    {
        return 'F';
    }
    else
    {
        return 'T';
    }
}

char Operations::And(char A, char B)
{
    if (A == 'T' && B == 'T')
    {
        return 'T';
    }
    else
    {
        return 'F';
    }
}

char Operations::Or(char A, char B)
{
    if (A == 'T' || B == 'T')
    {
        return 'T';
    }
    else
    {
        return 'F';
    }
}

char Operations::If(char A, char B)
{
    if (A == 'T' && B == 'F')
    {
        return 'F';
    }
    else
    {
        return 'T';
    }

}

char Operations::Iff(char A, char B) // 双条件
{
    if (A == B)
    {
        return 'T';
    }
    else
    {
        return 'F';
    }
}

char Operations::Xor(char A, char B) // 异或
{
    if (A != B)
    {
        return 'T';
    }
    else
    {
        return 'F';
    }
}

char Operations::Nand(char A, char B) // 与非
{
    return Not(And(A, B));
}

char Operations::Nor(char A, char B) // 或非
{
    return Not(Or(A, B));
}