#include "zhenzhibiao.h"
#include <math.h>


Reasoning::Reasoning()
{
    mode = 0;
    ifend = 0;
    return;
}


void Reasoning::Input()
{
    // 重新初始化
    input = "";
    initialinput = "";
    while (ArgName.size() != 0)
    {
        ArgName.pop_back();
    }
    Argnum = 0;
    while (Value.size() != 0)
    {
        Value.pop_back();
    }
    CNFstr = "";
    DNFstr = "";
    mode = 0;

    cout << endl << "请输入命题公式：";
    cin >> input;
    cout << endl;

    if (input == "/end")
    {
        while (getchar() != '\n');
        ifend = 1;
        return;
    }

    for (int i = 0; i < input.length(); i++) // 检查是否有符号混用
    {
        if (input[i] == '\'')
        {
            input[i] = '`';
        }
        
        if (input[i] == '`' || input[i] == '*' || input[i] == '+')
        {
            if (mode == 0 || mode == 2)
            {
                mode = 2;
            }
            else
            {
                cout << "符号混用" << endl;
                while (getchar() != '\n');
                Input();
            }
        }
        else if (input[i] == '~' || input[i] == '^' || input[i] == 'v' || input[i] == 'V')
        {
            if (mode == 0 || mode == 1)
            {
                mode = 1;
            }
            else
            {
                cout << "符号混用" << endl;
                while (getchar() != '\n');
                Input();
            }
        }
    }
    if (mode == 0)
    {
        mode = 1; // 默认
    }
    

    int count = 0;

    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == ' ') // 去除所有空格
        {
            input.erase(i, 1);
            i--;
        }
        else if (input[i] == '(')
        {
            count++;
        }
        else if (input[i] == ')')
        {
            count--;
        }
        else if (input[i] == 'V') // 将大写字母V转换为小写字母v
        {
            input[i] = 'v';
        }
        else if (input[i] >= 'a' && input[i] <= 'z' && input[i] != 'v') // 将小写字母转换为大写字母
        {
            input[i] = input[i] - 'a' + 'A';
        }
    }
    initialinput = input;
    if (count != 0)
    {
        cout << "括号不匹配" << endl;
        while (getchar() != '\n');
        Input();
    }
    for (int i = 0; i < input.length(); i++) // 如果有省略*的情况就补上
    {
        if ((input[i] == '`' || input[i] == ')' || (input[i] >= 'A' && input[i] <= 'Z')) && (input[i + 1] == '(' || (input[i+1] >= 'A' && input[i+1] <='Z')))
        {
            input.insert(i+1, "*");
        }
    }
    for (int i = 0; i < input.length(); i++) // 将符号转换为统一的符号
    {
        if (mode == 2)
        {
            if (input[i] == '*')
            {
                input[i] = '^';
            }
            else if (input[i] == '+')
            {
                input[i] = 'v';
            }
        }
        if (!(input[i] == '~' || input[i] == '^' || input[i] == 'v' || input[i] == '>' || input[i] == '<' || input[i] == '(' || input[i] == ')' || input[i] == '@' || input[i] == '[' || input[i] == ']' || input[i] == '`' || (input[i] >= 'A' && input[i] <= 'Z' && input[i] != 'V')))
        {
            cout << "输入有误" << endl;
            while (getchar() != '\n');
            Input();
        }
    }

    FindArg(); // 获取命题变元名

    // 在头尾添加括号
    input.insert(0, "(");
    input.insert(input.length(), ")");

    while (getchar() != '\n'); // 清空缓冲区

}

void Reasoning::FindArg()
{
    for (int i = 0; i < input.length(); i++)
    {
        bool flag = true;
        // 跳过符号
        if (input[i] != '~' && input[i] != '^' && input[i] != 'v' && input[i] != '@' && input[i] != '[' && input[i] != ']' && input[i] != '`' &&input[i] != '*' && input[i] != '+' && input[i] != '>' && input[i] != '<' && input[i] != '(' && input[i] != ')')
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
    else
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

char Reasoning::Xor(char A, char B)
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

char Reasoning::Nand(char A, char B)
{
    return Not(And(A, B));
}

char Reasoning::Nor(char A, char B)
{
    return Not(Or(A, B));
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
        // 计算not运算
        if (mode == 1)
        {
            for (int i = left+1; i < right; i++)
            {
                if (temp[i] == '~')
                {
                    temp[i+1] = Not(temp[i+1]);
                    temp.erase(i, 1);
                    right--;
                }
            }
        }
        else if (mode == 2)
        {
            for (int i = left+1; i < right; i++)
            {
                if (temp[i] == '`')
                {
                    temp[i-1] = Not(temp[i-1]);
                    temp.erase(i, 1);
                    right--;
                }
            }
            for (int i = left+1; i < right; i++)
            {
                if ((input[i] >= 'A' && input[i] <= 'Z') && ((input[i+1] >= 'A' && input[i+1] <='Z')))
                {
                    input.insert(i+2,")");
                    input.insert(i+1, "*");
                    input.insert(i, "(");
                }
            }
        }

        for (int i = left+2; i < right-1; i++)
        {
            if (temp[i] =='^' || temp[i] == 'v' || temp[i] == '>' || temp[i] == '<' || temp[i] == '@' || temp[i] == '[' || temp[i] == ']')
            {
                if (temp[i] == '^')
                {
                    temp[i+1] = And(temp[i-1], temp[i+1]);
                }
                else if (temp[i] == 'v')
                {
                    temp[i+1] = Or(temp[i-1], temp[i+1]);
                }
                else if (temp[i] == '>')
                {
                    temp[i+1] = If(temp[i-1], temp[i+1]);
                }
                else if (temp[i] == '<')
                {
                    temp[i+1] = Iff(temp[i-1], temp[i+1]);
                }
                else if (temp[i] == '@')
                {
                    temp[i+1] = Xor(temp[i-1], temp[i+1]);
                }
                else if (temp[i] == '[')
                {
                    temp[i+1] = Nand(temp[i-1], temp[i+1]);
                }
                else if (temp[i] == ']')
                {
                    temp[i+1] = Nor(temp[i-1], temp[i+1]);
                }
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
    cout << initialinput << endl;
    
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
    int count = 0;
    if (mode == 1)
    {
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
                count++;
            }
        }
    }
    else if (mode == 2)
    {
        for (int i = 0; i < pow(2, Argnum); i++)
        {
            if (Value[i] == 0)
            {
                CNFstr.push_back('(');
                int bin[Argnum] = {0};
                DToB(i, bin);
                for (int j = 0; j < Argnum; j++)
                {
                    CNFstr.push_back(ArgName[j]);
                    if (bin[Argnum-j-1] == 1)
                    {
                        CNFstr.push_back('`');
                    }
                    if (j != Argnum-1)
                    {
                        CNFstr.push_back('+');
                    }
                }
                CNFstr.push_back(')');
                count++;
            }
        }
    }
    if (count == 0) // 为永真式
    {
        CNFstr.push_back('T');
    }
    else if (mode == 1)
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
    int count = 0;
    if (mode == 1)
    {
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
                count++;
            }
        }
    }
    else if (mode == 2)
    {
        for (int i = 0; i < pow(2, Argnum); i++)
        {
            if (Value[i] == 1)
            {
                int bin[Argnum] = {0};
                DToB(i, bin);
                for (int j = 0; j < Argnum; j++)
                {
                    DNFstr.push_back(ArgName[j]);
                    if (bin[Argnum-j-1] == 0)
                    {
                        DNFstr.push_back('`');
                    }
                }
                DNFstr.push_back('+');
                count++;
            }
        }
    }
    if (count == 0) // 为永假式
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
    if (CNFstr.size() == 0 && DNFstr.size() == 0)
    {
        return;
    }
    cout << endl << "主合取范式:" << endl;
    cout << CNFstr << endl;
    cout << endl << "主析取范式:" << endl;
    cout << DNFstr << endl;

}

int Reasoning::CheckEnd()
{
    if (ifend == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

Reasoning::~Reasoning()
{
    return;
}