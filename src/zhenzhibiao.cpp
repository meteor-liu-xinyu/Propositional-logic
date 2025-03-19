#include "zhenzhibiao.h"
#include <math.h>
#include <string.h>


Reasoning::Reasoning()
{
    cout << "否定:~、合取:^、析取:v、条件:>、双条件:<" << endl;
    cout << "(命题变元不区分大小写,只能为A-Z除V外的字母)" << endl;
    cout << "请输入命题公式：";
    cin >> input;
    cout << endl;
    // input = "~(~A^Bv(A^~BVc)^A>C)>(CvA^B)";

    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == ' ') // 去除所有空格
        {
            input.erase(i, 1);
            i--;
        }
        else if (input[i] == 'V') // 将大写字母V转换为小写字母v
        {
            input[i] = 'v';
        }
        else if (input[i] >= 'a' && input[i] <= 'z' && input[i] != 'v') // 将小写字母转换为大写字母
        {
            input[i] = input[i] - 'a' + 'A';
        }
        if (!(input[i] == '~' || input[i] == '^' || input[i] == 'v' || input[i] == '>' || input[i] == '<' || input[i] == '(' || input[i] == ')' || (input[i] >= 'A' && input[i] <= 'Z')))
        {
            cout << "输入有误" << endl;
            exit(0);
        }
        
    }

    FindArg(); // 获取命题变元名

    // 在头尾添加括号
    input.insert(0, "(");
    input.insert(input.length(), ")");

}


void Reasoning::FindArg()
{
    for (int i = 0; i < input.length(); i++)
    {
        bool flag = true;
        // 跳过符号
        if (input[i] != '~' && input[i] != '^' && input[i] != 'v' && input[i] != 'V' && input[i] != '>' && input[i] != '<' && input[i] != '(' && input[i] != ')')
        {
            for (int j = 0; j < ArgName.size(); j++)
            {
                // 如果当前字符已经在ArgName中，则跳过
                if (input[i] == ArgName[j])
                {
                    flag = false;
                    break;
                }
            }
            if (flag == true)
            {
                ArgName.push_back(input[i]);
            }
        }
    }
    Argnum = ArgName.size();

    // 按字母顺序重排
    for (int i = 0; i < Argnum; i++)
    {
        for (int j = i + 1; j < Argnum; j++)
        {
            if (ArgName[i] > ArgName[j])
            {
                char temp = ArgName[i];
                ArgName[i] = ArgName[j];
                ArgName[j] = temp;
            }
        }
    }
}

int Reasoning::FindRight(string temp)
{
    int i = 0;
    while (temp[i] != ')')
    {
        i++;
    }
    return i;
}

int Reasoning::FindLeft(string temp)
{
    int i = FindRight(temp);
    while (temp[i] != '(')
    {
        i--;
    }
    return i;
}

void Reasoning::DToB(int n, int bin[])
{
    for (int i = 0; i < Argnum; i++)
    {
        bin[i] = 0;
    }
    int count = 0;
    while (n > 0)
    {
        bin[count++] = n % 2;
        n = n / 2;
    }
}

char Reasoning::Not(char A)
{
    if (A == 'T')
    {
        return 'F';
    }
    else if (A == 'F')
    {
        return 'T';
    }
}

char Reasoning::And(char A, char B)
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

char Reasoning::Or(char A, char B)
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

char Reasoning::If(char A, char B)
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

char Reasoning::Iff(char A, char B)
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

int Reasoning::CalculateValue(int n)
{
    int bin[Argnum];
    DToB(n, bin);

    string temp = input;
    for (int i = 0; i < input.length(); i++)
    {
        for (int j = 0; j < Argnum; j++)
        {
            if (temp[i] == ArgName[j])
            {
                if (bin[Argnum-j-1] == 1)
                {
                    temp[i] = 'T';
                }
                else
                {
                    temp[i] = 'F';
                }
            }
        }
    }
    
    while (temp[0] == '(')
    {
        int right = FindRight(temp);
        int left = FindLeft(temp);
        for (int i = left+1; i < right; i++) // 计算not运算
        {
            if (temp[i] == '~')
            {
                temp[i+1] = Not(temp[i+1]);
                temp.erase(i, 1);
                right--;
            }
        }
        for (int i = left+2; i < right-1; i++)
        {
            if (temp[i] == '^')
            {
                temp[i+1] = And(temp[i-1], temp[i+1]);
                temp.erase(i-1, 2);
                right -= 2;
                i--;
            }
            else if (temp[i] == 'v')
            {
                temp[i+1] = Or(temp[i-1], temp[i+1]);
                temp.erase(i-1, 2);
                right -= 2;
                i--;
            }
            else if (temp[i] == '>')
            {
                temp[i+1] = If(temp[i-1], temp[i+1]);
                temp.erase(i-1, 2);
                right -= 2;
                i--;
            }
            else if (temp[i] == '<')
            {
                temp[i+1] = Iff(temp[i-1], temp[i+1]);
                temp.erase(i-1, 2);
                right -= 2;
                i--;
            }
        }
        
        temp.erase(left+2, 1);
        temp.erase(left, 1);
    }

    if (temp[0] == 'T')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void Reasoning::MakeTable()
{
    cout << endl << "真值表:" << endl;
    for (int i = 0; i < Argnum; i++)
    {
        cout << ArgName[i]<< "\t";
    }
    cout << input << endl;
    
    for (int i = 0; i < pow(2, Argnum); i++)
    {
        int bin[Argnum] = {0};
        DToB(i, bin);
        for (int j = Argnum-1; j >= 0; j--)
        {
            cout << bin[j] << "\t";
        }
        int value = CalculateValue(i);
        Value.push_back(value);
        cout << "\t" << value << endl;
    }

}

void Reasoning::CNF()
{
    if (Argnum < 2)
    {
        return;
    }
    for (int i = 0; i < pow(2, Argnum); i++)
    {
        if (Value[i] == 0)
        {
            CNFstr.push_back('(');
            int bin[Argnum] = {0};
            DToB(i, bin);
            for (int j = 0; j < Argnum; j++)
            {
                if (bin[Argnum-j-1] == 1)
                {
                    CNFstr.push_back('~');
                }
                CNFstr.push_back(ArgName[j]);
                if (j != Argnum-1)
                {
                    CNFstr.push_back('v');
                }
            }
            CNFstr.push_back(')');
            CNFstr.push_back('^');
        }
    }
    if (CNFstr.size() == 0) // 为永真式
    {
        CNFstr.push_back('T');
    }
    else
    {
        CNFstr.pop_back();
    }
}

void Reasoning::DNF()
{
    if (Argnum < 2)
    {
        return;
    }
    for (int i = 0; i < pow(2, Argnum); i++)
    {
        if (Value[i] == 1)
        {
            DNFstr.push_back('(');
            int bin[Argnum] = {0};
            DToB(i, bin);
            for (int j = 0; j < Argnum; j++)
            {
                if (bin[Argnum-j-1] == 0)
                {
                    DNFstr.push_back('~');
                }
                DNFstr.push_back(ArgName[j]);
                if (j != Argnum-1)
                {
                    DNFstr.push_back('^');
                }
            }
            DNFstr.push_back(')');
            DNFstr.push_back('v');
        }
    }
    if (DNFstr.size() == 0) // 为永假式
    {
        DNFstr.push_back('F');
    }
    else
    {
        DNFstr.pop_back();
    }
}

void Reasoning::PrintNF()
{
    cout << endl << "主合取范式:" << endl;
    cout << CNFstr << endl;
    cout << endl << "主析取范式:" << endl;
    cout << DNFstr << endl;

}


Reasoning::~Reasoning()
{
    return;
}