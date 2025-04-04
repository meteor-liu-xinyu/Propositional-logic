#include "reasoning.h"

Reasoning::Reasoning()
{
    mode = 0;
    ifend = false;
    openTheTruthTable = true;
    openNF = true;
    openKanuo = true;
    openKanuoSimplify = true;
    return;
}

void Reasoning::Init()
{
    // 重新初始化
    input.clear();
    initialinput.clear();
    ArgName.clear();
    Argnum = 0;
    Value.clear();
    CNFstr.clear();
    DNFstr.clear();
    CNFMstr.clear();
    DNFmstr.clear();
    kanuo.clear();
    PI.clear();
    mode = 0;
    endinter = false;
}

void Reasoning::Input()
{
    Init(); // 初始化

    cout << endl << "请输入：";
    getline(cin, input);
    // input = "F(x,y,z)=∑(0,2,3,4,5,7)"; // !测试用例
    cout << endl;
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == ' ') // 去除空格
        {
            input.erase(i, 1);
            i--;
        }
    }
    if (input.size() == 0) // 判断输入是否为空
    {
        cout << "输入为空" << endl;
        skip = true;
        return;
    }
    if (input == "/end" || input == "/END" || input == "/exit" || input == "/EXIT") // 结束程序
    {
        ifend = true;
        return;
    }
    else if (input == "/clear" || input == "/CLEAR") // 清屏
    {
        system("cls");
        cout << "否定:~、合取:^、析取:v/否定:`或'或‘或’、合取:*、析取:+、条件:>、双条件:<、异或:@、与非:[、或非:]" << endl;
        skip = true;
        return;
    }
    else if (input == "/setting" || input == "/SETTING") // 设置输出模式
    {
        Setting();
        skip = true;
        return;
    }

    // 将中文括号转换为英文括号
    for (int i = 0; i < input.length(); i++)
    {
        if (i <= input.length() - 3 && input[i] == '\357' && input[i + 1] == '\274' && input[i+2] == '\210') // 判断是否为中文左括号
        {
            input.erase(i, 3);
            input.insert(i, "(");
            i--;
        }
        else if (i >= 2 && input[i - 2] == '\357' && input[i - 1] == '\274' && input[i] == '\211') // 判断是否为中文右括号
        {
            input.erase(i - 2, 3);
            input.insert(i - 2, ")");
            i--;
        }
    }

    // 将not运算其他符号转为`
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == '\'')
        {
            input[i] = '`';
        }
        else if (i <= input.length() - 2 && input[i] == '\342' && input[i + 1] == '\200' && input[i + 2] == '\230') // 判断是否为中文左单引号
        {
            input.erase(i, 3);
            input.insert(i, "`");
            i--;
        }
        else if (i >= 1 && input[i - 1] == '\342' && input[i] == '\200' && input[i - 2] == '\231') // 判断是否为中文右单引号
        {
            input.erase(i - 1, 3);
            input.insert(i - 1, "`");
            i--;
        }
        else if (i >= 1 && input[i - 1] == '\357' && input[i] == '\274' && input[i + 1] == '\214') // 判断是否为中文逗号
        {
            input.erase(i - 1, 3);
            input.insert(i - 1, ",");
            i--;
        }
    }

    int count = 0; // 括号匹配
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == '(')
        {
            count++;
        }
        else if (input[i] == ')')
        {
            count--;
            if (count < 0)
            {
                cout << "括号不匹配" << endl;
                skip = true;
                return;
            }
        }
        else if (input[i] == 'V') // 将大写字母V转换为小写字母v
        {
            input[i] = 'v';
        }
        else if (input[i] == 'm'); // 判断mode前暂不将m转换为M
        else if (input[i] >= 'a' && input[i] <= 'z' && input[i] != 'v') // 将小写字母转换为大写字母
        {
            input[i] = input[i] - 'a' + 'A';
        }
    }
    if (count != 0)
    {
        cout << "括号不匹配" << endl;
        skip = true;
        return;
    }

    // 判断是否为连乘或连加式
    for (int i = 0; i < input.length(); i++)
    {
        if (input[i] == '\342' && input[i+1] == '\210' && input[i+2] == '\217') // 连乘
        {
            initialinput = input;
            input.erase(i, 3);
            if (input[i] != 'M')
            {
                input.insert(i, "M");
            }
            mode = 3;
            break;
        }
        else if (input[i] == '\342' && input[i+1] == '\210' && input[i+2] == '\221') // 连加
        {
            initialinput = input;
            input.erase(i, 3);
            if (input[i] != 'm')
            {
                input.insert(i, "m");
            }
            mode = 4;
            break;
        }
    }

    if (mode != 3 && mode != 4)
    {
        for (int i = 0; i < input.length(); i++) // 检查是否有符号混用,同时设置mode
        {
            if (input[i] == '`' || input[i] == '*' || input[i] == '+')
            {
                if (mode == 0 || mode == 2)
                {
                    mode = 2;
                }
                else
                {
                    cout << "符号混用" << endl;
                    skip = true;
                    return;
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
                    skip = true;
                    return;
                }
            }
        }
        if (mode == 0)
        {
            mode = 2; // 默认
        }

        initialinput = input; // 保存初始输入

        for (int i = 0; i < input.length(); i++) // 将符号转换为统一的符号(not运算除外),便于后续计算,同时检查输入是否合法
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
                else if ((i == 0 && input[i] == '`') || (input[i] == '`' && (!(input[i-1] >= 'A' && input[i-1] <= 'Z')) && input[i-1] != ')'))
                {
                    cout << "输入有误" << endl;
                    skip = true;
                    return;
                }
            }
            if (!(input[i] == '~' || input[i] == '^' || input[i] == 'v' || input[i] == '>' || input[i] == '<' || input[i] == '(' || input[i] == ')' || input[i] == '@' || input[i] == '[' || input[i] == ']' || input[i] == '`' || (input[i] >= 'A' && input[i] <= 'Z' && input[i] != 'V')))
            {
                cout << "输入有误" << endl;
                skip = true;
                return;
            }
        }
        FindArg(); // 获取命题变元名
        // 在头尾添加括号
        input.insert(0, "(");
        input.insert(input.length(), ")");
    }
    else // 连乘或连加式
    {
        // 判断输入是否合法(是否有'=')
        bool flag = false;
        for (int i = 0; i < input.length(); i++)
        {
            if (input[i] == '=')
            {
                flag = true;
                break;
            }
        }
        if (!flag)
        {
            cout << "输入有误" << endl;
            skip = true;
            return;
        }
        if (!(input[0] >= 'A' && input[0] <= 'Z' && input[1] == '(')) // 判断输入是否合法
        {
            cout << "输入有误" << endl;
            skip = true;
            return;
        }
        input.erase(0, 1); // 删除函数符号

        FindArg();
    }
}

void Reasoning::FindArg()
{
    if (mode == 1 || mode == 2)
    {
        for (int i = 0; i < input.length(); i++)
        {
            // 跳过符号
            if (input[i] != '~' && input[i] != '^' && input[i] != 'v' && input[i] != '@' && input[i] != '[' && input[i] != ']' && input[i] != '`' &&input[i] != '*' && input[i] != '+' && input[i] != '>' && input[i] != '<' && input[i] != '(' && input[i] != ')')
            {
                // 如果当前字符已经在ArgName中，则跳过
                bool flag = true;
                for (int j = 0; j < ArgName.size(); j++)
                {
                    if (input[i] == ArgName[j])
                    {
                        flag = false;
                        break;
                    }
                }
                if (flag == true)
                {
                    ArgName.push_back(input[i]);
                    Argnum++;
                }
            }
        }
    }
    else // 连乘或连加式
    {
        int i = 1;
        for (; input[i] != ')'; i++)
        {
            if (input[i] == '=')
            {
                cout << "输入有误" << endl;
                skip = true;
                return;
            }
            if (input[i] >= 'A' && input[i] <= 'Z')
            {
                // 如果当前字符已经在ArgName中，则跳过
                bool flag = true;
                for (int j = 0; j < ArgName.size(); j++)
                {
                    if (input[i] == ArgName[j])
                    {
                        flag = false;
                        break;
                    }
                }
                if (flag == true)
                {
                    ArgName.push_back(input[i]);
                    Argnum++;
                }
            }
        }
        if (input[i+1] != '=')
        {
            cout << "输入有误" << endl;
            skip = true;
            return;
        }
        input.erase(0, i+2); // 删除如(a,B,c,d)= 的部分
    }

    powArgnum = pow(2, Argnum); // 计算2^Argnum

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

void Reasoning::BuildHashTable()
{
    int bin[Argnum] = {0};
    for (int i = 0; i < powArgnum; i++)
    {
        DToB(i, bin);
        string binstr;
        for (int j = 0; j < Argnum; j++)
        {
            binstr.push_back(bin[Argnum-j-1] + '0'); // 将二进制数转换为字符串
        }
        ToBin[i] = binstr;
        ToDec[binstr] = i;
    }
}

int Reasoning::FindRight(const string& temp) // 找到0开始第一个右括号的位置
{
    int i = 0;
    while (temp[i] != ')')
    {
        i++;
    }
    return i;
}

int Reasoning::FindLeft(const string& temp) // 找到0开始第一个右括号对应的左括号的位置
{
    int i = FindRight(temp); // 找到0开始第一个右括号的位置
    while (temp[i] != '(')
    {
        i--;
    }
    return i;
}

void Reasoning::DToB(int n, int bin[]) // 将十进制数n转换为二进制数，存入bin数组中
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

char Reasoning::Iff(char A, char B) // 双条件
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

char Reasoning::Xor(char A, char B) // 异或
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

char Reasoning::Nand(char A, char B) // 与非
{
    return Not(And(A, B));
}

char Reasoning::Nor(char A, char B) // 或非
{
    return Not(Or(A, B));
}

int Reasoning::CalculateValue(int n) // 计算真值表n行的真值
{
    // 将十进制数n转换为二进制数，存入bin数组中
    int bin[Argnum] = {0};
    DToB(n, bin);

    // 用T/F替换ArgName对应的值,存入temp中
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
        // 计算最靠左侧的最内侧的一对括号边界
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
            // 补上省略的^
            for (int i = left+1; i < right-1; i++)
            {
                if ((temp[i] >= 'A' && temp[i] <= 'Z') && ((temp[i+1] >= 'A' && temp[i+1] <='Z')))
                {
                    temp.insert(i+2,")");
                    temp.insert(i+1, "^");
                    temp.insert(i, "(");
                    right = FindRight(temp);
                    left = FindLeft(temp);
                    break;
                }
            }
        }

        // 计算其他运算
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

        // 删除剩下的空括号
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

void Reasoning::Cal() // 计算真值表
{
    if (mode == 3 || mode == 4)
    {
        if (input[0] == 'M' && input[1] == '(' && mode == 3)
        {
            for (int i = 0; i < powArgnum; i++)
            {
                Value.push_back(1);
            }
        }
        else if (input[0] == 'm' && input[1] == '(' && mode == 4)
        {
            for (int i = 0; i < powArgnum; i++)
            {
                Value.push_back(0);
            }
        }
        else
        {
            cout << "输入有误" << endl;
            skip = true;
            return;
        }

        // 读取()中的逗号分隔的数字，更改对应Value的值
        string temp;
        int i = 2;
        for (; input[i] != ')'; i++)
        {
            if (!(input[i] >= '0' && input[i] <= '9') && input[i]!= ',')
            {
                cout << "输入有误" << endl;
                skip = true;
                return;
            }
            if (input[i] != ',')
            {
                temp.push_back(input[i]);
            }
            else
            {
                int tempnum = atoi(temp.c_str());
                Value[tempnum] = !Value[tempnum];
                temp.clear();
            }
        }
        int tempnum = atoi(temp.c_str());
        Value[tempnum] = !Value[tempnum];

        mode = 2; // 化归位mode2的情况

        for (int i = 0; i < powArgnum; i++)
        {
            if (Value[i] == 1)
            {
                PI.push_back(i);
            }
        }
    }
    else
    {
        for (int i = 0; i < powArgnum; i++)
        {
            Value.push_back(CalculateValue(i));
            if (Value[i] == 1)
            {
                PI.push_back(i);
            }
        }
    }
}

void Reasoning::MakeTable() // 打印真值表
{
    cout << "-----------------------------------------";
    cout << endl << "真值表:" << endl;
    for (int i = 0; i < Argnum; i++)
    {
        cout << ArgName[i]<< "\t";
    }
    cout << initialinput << endl;

    for (int i = 0; i < powArgnum; i++)
    {
        int bin[Argnum] = {0};
        DToB(i, bin);
        for (int j = Argnum-1; j >= 0; j--)
        {
            cout << bin[j] << "\t";
        }
        if (initialinput.size() >= 15)
        {
            cout << "\t";
        }
        else if (initialinput.size() >= 8)
        {
            cout << "  ";
        }
        cout << "  " << Value[i] << endl;
    }
}

void Reasoning::CNF() // 计算主合取范式
{
    if (Argnum < 2) // 只有一个变量时,不输出
    {
        return;
    }
    bool alltrue = true; // 判断是否为永真式
    for (const auto& i : Value)
    {
        if (i == 0)
        {
            alltrue = false;
            break;
        }
    }
    if (alltrue)
    {
        CNFstr = 'T';
        CNFMstr = 'T';
        return;
    }

    int count = 0;
    if (mode == 1)
    {
        for (int i = 0; i < powArgnum; i++)
        {
            if (Value[i] == 0)
            {
                if (CNFMstr.size() == 0)
                {
                    CNFMstr += " ∏ M(";
                }
                string num = to_string(i);
                CNFMstr.append(num);
                CNFMstr.push_back(',');
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
            if (count % 8 == 0 && CNFstr[CNFstr.length()-1] != '\n' && count != 0)
            {
                CNFstr.push_back('\n');
            }
        }
        CNFstr.pop_back();
    }
    else if (mode == 2)
    {
        for (int i = 0; i < powArgnum; i++)
        {
            if (Value[i] == 0)
            {
                if (CNFMstr.size() == 0)
                {
                    CNFMstr += " ∏ M(";
                }
                string num = to_string(i);
                CNFMstr.append(num);
                CNFMstr.push_back(',');
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
            if (count % 8 == 0 && CNFstr[CNFstr.length()-1] != '\n' && count != 0)
            {
                CNFstr.push_back('\n');
            }
        }
    }
    CNFMstr.pop_back();
    CNFMstr.push_back(')');
}

void Reasoning::DNF() // 计算主析取范式
{
    if (Argnum < 2) // 只有一个变量时,不输出
    {
        return;
    }

    bool allfalse = true; // 判断是否为重言式
    for (const auto& i : Value)
    {
        if (i == 1)
        {
            allfalse = false;
            break;
        }
    }
    if (allfalse)
    {
        DNFstr = 'F';
        DNFmstr = 'F';
        return;
    }

    int count = 0;
    if (mode == 1)
    {
        for (int i = 0; i < powArgnum; i++)
        {
            if (Value[i] == 1)
            {
                if (DNFmstr.size() == 0)
                {
                    DNFmstr += " ∑ m(";
                }
                string num = to_string(i);
                DNFmstr.append(num);
                DNFmstr.push_back(',');
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
            if (count % 8 == 0 && DNFstr[DNFstr.length()-1] != '\n' && count != 0)
            {
                DNFstr.push_back('\n');
            }
        }
    }
    else if (mode == 2)
    {
        for (int i = 0; i < powArgnum; i++)
        {
            if (Value[i] == 1)
            {
                if (DNFmstr.size() == 0)
                {
                    DNFmstr += " ∑ m(";
                }
                string num = to_string(i);
                DNFmstr.append(num);
                DNFmstr.push_back(',');
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
            if (count % 8 == 0 && DNFstr[DNFstr.length()-1] != '\n' && count != 0)
            {
                DNFstr.push_back('\n');
            }
        }
    }
    DNFstr.pop_back();
    DNFmstr.pop_back();
    DNFmstr.push_back(')');
}

void Reasoning::PrintNF() // 打印主合取、析取范式
{
    CNF(); // 生成CNF
    DNF(); // 生成DNF
    if (CNFstr.size() == 0 && DNFstr.size() == 0) // 如果没有内容，则不输出
    {
        return;
    }
    cout << "-----------------------------------------";
    cout << endl << "主合取范式:" << endl;
    cout << CNFstr << endl;
    cout << CNFMstr << endl;
    cout << endl << "主析取范式:" << endl;
    cout << DNFstr << endl;
    cout << DNFmstr << endl;
}

void Reasoning::Setting() // 设置输出选项
{
    cout << "是否输出真值表(1/0):";
    cin >> openTheTruthTable;
    if (openTheTruthTable != 0 && openTheTruthTable != 1)
    {
        cout << "输入错误" << endl;
        while (getchar() != '\n');
        Setting();
    }
    cout << "是否输出主合取+析取范式(1/0):";
    cin >> openNF;
    if (openNF != 0 && openNF != 1)
    {
        cout << "输入错误" << endl;
        while (getchar() != '\n');
        Setting();
    }
    cout << "是否输出卡诺图(1/0):";
    cin >> openKanuo;
    if (openKanuo != 0 && openKanuo != 1)
    {
        cout << "输入错误" << endl;
        while (getchar() != '\n');
        Setting();
    }
    cout << "是否输出卡诺图化简(1/0):";
    cin >> openKanuoSimplify;
    if (openKanuoSimplify != 0 && openKanuoSimplify != 1)
    {
        cout << "输入错误" << endl;
        while (getchar() != '\n');
        Setting();
    }
    while(getchar()!='\n');
    if (openTheTruthTable == false && openNF == false && openKanuo == false && openKanuoSimplify == false)
    {
        cout << "至少选择一个输出选项" << endl;
        Setting();
    }
    cout << "----------------------------------------------------------------------";
}

void Reasoning::Run()
{
    cout << "否定:~、合取:^、析取:v/否定:`(兼容'或‘或’)、合取:*、析取:+、条件:>、双条件:<、异或:@、与非:[、或非:]" << endl;
    cout << "命题变元不区分大小写,只能为A-Z除V外的字母,支持中文括号" << endl;
    cout << "输入/end或/exit结束,/setting设置输出选项,/clear清空页面" << endl;
    cout << "支持离散数学和数字逻辑两种符号体系，请勿混用" << endl;
    cout << "示例输入:~(~A^Bv(A^~BVc)^A>C)>(Cv(A<B))" << endl;
    cout << "示例输入:(A`(B+C`))`(A+B`+C) 或 (A`(B+C`))`*(A+B`+C)" << endl;
    cout << "示例输入:F(x,y,z)=∑ m(2,3,5,7)或F(x,y,z)=∏ M(2,3,5,7)" << endl;

    cout << "-------------------------------------------------------------------------------";
    while (1)
    {
        Input();
        if (skip == true)
        {
            skip = false;
            continue;
        }
        if (ifend)
        {
            break;
        }
        Cal();
        BuildHashTable();
        if (openTheTruthTable)
        {
            MakeTable(); // 生成真值表
        }
        if (openNF)
        {
            PrintNF(); // 打印结果
        }
        if (openKanuo)
        {
            MakeKanuo(); // 生成卡诺图
        }
        if (openKanuoSimplify)
        {
            QM(); // 生成卡诺图化简
        }
        cout << "-------------------------------------------------------------------------------";
    }
}

void Reasoning::MakeKanuo() // 生成卡诺图
{
    cout << "-----------------------------------------" << endl;
    if (Argnum != 3 && Argnum != 4)
    {
        cout << "不支持" << Argnum << "个变元的卡诺图" << endl;
        return;
    }
    cout << "卡诺图:" << endl;
    if (Argnum == 3)
    {
        cout << ArgName[0] << ArgName[1] << "\\" << ArgName[2] << "\t 0 \t 1" << endl;
        vector<int> temp1 = {0,2,6,4};
        vector<int> temp2 = {0,1};
        vector<string> tempbin = {"00","01","11","10"};
        for (int i = 0; i < 4; i++)
        {
            cout << tempbin[i] << "  \t";
            for (int j = 0; j < 2; j++)
            {
                if (Value[temp1[i]+temp2[j]] == 1)
                {
                    cout << " 1 \t";
                }
                else
                {
                    cout << " 0 \t";
                }
            }
            cout << endl;
        }
    }
    else if (Argnum == 4)
    {
        cout << ArgName[0] << ArgName[1] << "\\" << ArgName[2] << ArgName[3] << "\t 00 \t 01 \t 11 \t 10" << endl;
        vector<int> temp1 = {0,4,12,8};
        vector<int> temp2 = {0,1,3,2};
        vector<string> tempbin = {"00","01","11","10"};
        for (int i = 0; i < 4; i++)
        {
            cout << tempbin[i] << "  \t";
            for (int j = 0; j < 4; j++)
            {
                if (Value[temp1[i]+temp2[j]] == 1)
                {
                    cout << " 1 \t";
                }
                else
                {
                    cout << " 0 \t";
                }
            }
            cout << endl;
        }
    }
}

int Reasoning::Countone(string terms) // 计算字符串中1的个数
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

bool Reasoning::IfNear(const string& a, const string& b) // 判断两个字符串是否只有一位不同
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

string Reasoning::Combine(const string& a, const string& b) // 合并两个字符串，将不同的位置替换为'-'
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

int Reasoning::CountDashes(const string& str) // 计算字符串中'-'的个数
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

void Reasoning::QM() // 卡诺图化简
{
    cout << "-----------------------------------------" << endl;
    if (DNFstr == "F")
    {
        cout << "卡诺图化简结果: 该命题为重言式,无需化简" << endl;
        return;
    }
    else if (CNFstr == "T")
    {
        cout << "卡诺图化简结果: 该命题为永真式,无需化简" << endl;
        return;
    }
    if (DNFstr.size() == 0)
    {
        bool alltrue = true;
        bool allfalse = true;
        for (const auto& i : Value)
        {
            if (i == 0)
            {
                alltrue = false;
            }
            if (i == 1)
            {
                allfalse = false;
            }
        }
        if (alltrue)
        {
            cout << "卡诺图化简结果: 该命题为永真式,无需化简" << endl;
            return;
        }
        if (allfalse)
        {
            cout << "卡诺图化简结果: 该命题为重言式,无需化简" << endl;
            return;
        }
    }

    if (Argnum == 1)
    {
        cout << "卡诺图化简结果: 该命题为单变元命题,无需化简" << endl;
        return;
    }

    vector<vector<string>> groups;
    // 初始化groups
    for (int i = 0; i < Argnum+1; i++)
    {
        groups.push_back(vector<string>());
    }

    for (int i = 0; i < PI.size(); i++)
    {
        // 将二进制字符串添加到对应的组中
        string temp = ToBin[PI[i]];
        groups[Countone(temp)].push_back(temp);
    }

    while (!endinter) // 循环合并项
    {
        groups = QMCombine(groups); // 合并项
    }

    // 生成最终PI
    int finalPIsize = 0;
    vector<string> finalPI;
    for (const auto& group : groups)
    {
        for (const auto& term : group)
        {
            finalPI.push_back(term);
            finalPIsize++;
        }
    }

    // 去除不必要的项
    vector<vector<int>> table; // 基本蕴含项表
    vector<int> row(pow(2,Argnum),0);
    for (int i = 0; i < finalPIsize; i++)
    {
        table.push_back(row);
        int dashenum = CountDashes(finalPI[i]);
        int num[2*dashenum] = {0};
        for (int j = 0; j < Argnum; j++)
        {
            if (finalPI[i][j] == '-')
            {
                for (int k = 0; k < dashenum; k++)
                {
                    num[k] += pow(2,Argnum-j-1);
                }
            }
            else if (finalPI[i][j] == '1')
            {
                for (int k = 0; k < dashenum*2; k++)
                {
                    num[k] += pow(2,Argnum-j-1);
                }
            }
        }
        for (int j = 0; j < dashenum*2; j++)
        {
            table[i][num[j]] = 1;
        }
    }

    int tempEPI = 0;
    unordered_map<int, int> hashTable;
    for ( int i = 0; i < finalPIsize; i++)
    {
        hashTable[i] = -i-1;
    }
    for (int i = 0; i < pow(2,Argnum); i++)
    {
        int count = 0;
        for (int j = 0; j < finalPIsize; j++)
        {
            if (table[j][i] == 1)
            {
                tempEPI = j;
                count ++;
            }
        }
        if (count == 1)
        {
            hashTable[tempEPI] = tempEPI;
        }
    }

    bool flag = false;
    for (int i = 0; i < finalPIsize; i++)
    {
        if (hashTable[i] >= 0)
        {
            flag = true;
        }
    }
    if (!flag)
    {
        hashTable[0] = 0;
    }

    for (int i = 0; i < finalPIsize; i++)
    {
        if (hashTable[i] >= 0) // 是基本蕴含项
        {
            for (int j = table[0].size()-1; j >= 0; j--)
            {
                if (table[i][j] == 1)
                {
                    // 删除该列
                    for (int k = finalPIsize-1; k >= 0; k--)
                    {
                        table[k].erase(table[k].begin() + j);
                    }
                }
            }
        }
    }

    // 去除不必要的项
    while (1)
    {
        int temprownum = 0;
        int tempcount = 0;
        for (int i = 0; i < finalPIsize; i++)
        {
            int count = 0;
            if (hashTable[i] >= 0)
            {
                continue; // 是基本蕴含项
            }
            // 不是基本蕴含项
            for (int j = 0; j < table[0].size(); j++)
            {
                if (table[i][j] == 1)
                {
                    count ++;
                }
            }

            if (count > tempcount)
            {
                tempcount = count;
                temprownum = i;
            }
        }
        if (tempcount == 0)
        {
            break;
        }
        hashTable[temprownum] = temprownum; // 更改为基本蕴含项
        for (int j = table[0].size()-1; j >= 0; j--)
        {
            if (table[temprownum][j] == 1)
            {
                // 删除该列
                for (int k = finalPIsize-1; k >= 0; k--)
                {
                    table[k].erase(table[k].begin() + j);
                }
            }
        }
    }

    vector<string> finalEPI; // 存储最终的基本蕴含项
    for (int i = 0; i < finalPIsize; i++)
    {
        if (hashTable[i] >= 0)
        {
            finalEPI.push_back(finalPI[i]);
        }
    }

    // 从finalEPI中计算kanuo化简结果
    if (mode == 1)
    {
        for (const auto& term : finalEPI)
        {
            kanuo.push_back('(');
            for (int i = 0; i < term.size(); i++)
            {
                if (term[i] == '1')
                {
                    kanuo.push_back(ArgName[i]);
                    kanuo.push_back('^');
                }
                else if (term[i] == '0')
                {
                    kanuo.push_back('~');
                    kanuo.push_back(ArgName[i]);
                    kanuo.push_back('^');
                }
            }
            kanuo.pop_back();
            kanuo.push_back(')');
            kanuo.push_back('v');
        }
        kanuo.pop_back();
    }
    else if (mode == 2)
    {
        for (const auto& term : finalEPI)
        {
            for (int i = 0; i < term.size(); i++)
            {
                if (term[i] == '1')
                {
                    kanuo.push_back(ArgName[i]);
                }
                else if (term[i] == '0')
                {
                    kanuo.push_back(ArgName[i]);
                    kanuo.push_back('`');
                }
            }
            kanuo.push_back('+');
        }
        kanuo.pop_back();
    }

    cout << "卡诺图化简结果: " << kanuo << endl;
}

vector<vector<string>> Reasoning::QMCombine(const vector<vector<string>>& groups) // 合并项
{
    endinter = true; // 标记是否已经不能再合并
    vector<vector<string>> nextgroups; // 存储合并后的项
    for (int i = 0; i < Argnum+1; i++)
    {
        nextgroups.push_back(vector<string>());
    }
    vector<vector<bool>> used(groups.size(), vector<bool>(groups.size(), false)); // 标记是否被合并

    for (int i = 0; i < groups.size()-1; i++) // 两两遍历所有组
    {
        for (int j = 0; j < groups[i].size(); j++) // 遍历有i个1的组
        {
            for (int k = 0; k < groups[i+1].size(); k++) // 遍历有i+1个1的组
            {
                if (IfNear(groups[i][j], groups[i+1][k]))
                {
                    int dashesnum = min(CountDashes(groups[i][j]), CountDashes(groups[i+1][k]));
                    string combined = Combine(groups[i][j], groups[i+1][k]);
                    if (CountDashes(combined) == dashesnum+1)
                    {
                        int onenum = Countone(combined);
                        endinter = false; // 如果有新的合并，则继续合并
                        // 检查是否已经存在相同的合并项
                        bool flag = false;
                        for (const auto& nextgroup : nextgroups[onenum])
                        {
                            if (nextgroup == combined)
                            {
                                flag = true;
                                break;
                            }
                        }
                        if (!flag)
                        {
                            nextgroups[onenum].push_back(combined);
                        }
                        // 标记合并的项
                        used[i][j] = true;
                        used[i+1][k] = true;
                    }
                }
            }
        }
    }

    // 将未被合并的项添加到结果中
    for (int i = 0; i < groups.size(); i++)
    {
        for (int j = 0; j < groups[i].size(); j++)
        {
            if (!used[i][j])
            {
                nextgroups[Countone(groups[i][j])].push_back(groups[i][j]);
            }
        }
    }

    return nextgroups;
}

Reasoning::~Reasoning()
{
    return;
}