#include "zhenzhibiao.h"
#include <math.h>

string lbstr = "（";
string rbstr = "）";
char lb0 = lbstr[0];
char lb1 = lbstr[1];
char lb2 = lbstr[2];
char rb0 = rbstr[0];
char rb1 = rbstr[1];
char rb2 = rbstr[2];
string lcotstr = "‘";
string rcotstr = "’";
char lcot0 = lcotstr[0];
char lcot1 = lcotstr[1];
char rcot0 = rcotstr[0];
char rcot1 = rcotstr[1];

string dotstr = "，";
char dot0 = dotstr[0];
char dot1 = dotstr[1];
string prodstr = "∏";
char prod0 = prodstr[0];
char prod1 = prodstr[1];
string sumstr = "∑";
char sum0 = sumstr[0];
char sum1 = sumstr[1];

Reasoning::Reasoning()
{
    mode = 0;
    ifend = false;
    openzhenzhibiao = 1;
    openNF = 1;
    openkanuo = 1;
    openkanuohuajian = 1;
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
    cout << endl << "请输入：";
    getline(cin, input);
    cout << endl;

    if (input == "/end" || input == "/END" || input == "/exit" || input == "/EXIT")
    {
        ifend = true;
        return;
    }
    else if (input == "/clear" || input == "/CLEAR")
    {
        system("cls");
        cout << "否定:~、合取:^、析取:v/否定:`或'或‘或’、合取:*、析取:+、条件:>、双条件:<、异或:@、与非:[、或非:]" << endl;
        Input();
    }
    else if (input == "/setting" || input == "/SETTING")
    {
        Setting();
        Input();
    }
    for (int i = 0; i < input.length(); i++) // 将中文括号转换为英文括号
    {
        if (i <= input.length() - 3 && input[i] == lb0 && input[i + 1] == lb1 && input[i+2] == lb2)
        {
            input.erase(i, 3);
            input.insert(i, "(");
            i--;
        }
        else if (i >= 2 && input[i - 2] == rb0 && input[i - 1] == rb1 && input[i] == rb2)
        {
            input.erase(i - 2, 3);
            input.insert(i - 2, ")");
            i--;
        }
    }

    for (int i = 0; i < input.length(); i++) // 检查是否有符号混用
    {
        if (input[i] == '\'')
        {
            input[i] = '`';
        }
        else if (i <= input.length() - 2 && input[i] == lcot0 && input[i + 1] == lcot1)
        {
            input.erase(i, 3);
            input.insert(i, "`");
            i--;
        }
        else if (i >= 1 && input[i - 1] == rcot0 && input[i] == rcot1)
        {
            input.erase(i - 1, 3);
            input.insert(i - 1, "`");
            i--;
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
            if (count < 0)
            {
                cout << "括号不匹配" << endl;
                Input();
            }
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
        Input();
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
            else if ((i == 0 && input[i] == '`') || (input[i] == '`' && (!(input[i-1] >= 'A' && input[i-1] <= 'Z')) && input[i-1] != ')'))
            {
                cout << "输入有误" << endl;
                Input();
            }
        }
        if (!(input[i] == '~' || input[i] == '^' || input[i] == 'v' || input[i] == '>' || input[i] == '<' || input[i] == '(' || input[i] == ')' || input[i] == '@' || input[i] == '[' || input[i] == ']' || input[i] == '`' || (input[i] >= 'A' && input[i] <= 'Z' && input[i] != 'V')))
        {
            cout << "输入有误" << endl;
            Input();
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

int Reasoning::FindRight(const string& temp)
{
    int i = 0;
    while (temp[i] != ')')
    {
        i++;
    }
    return i;
}

int Reasoning::FindLeft(const string& temp)
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

void Reasoning::Cal()
{
    for (int i = 0; i < pow(2, Argnum); i++)
    {
        Value.push_back(CalculateValue(i));
        if (Value[i] == 1)
        {
            PI.push_back(i);
        }
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

void Reasoning::CNF()
{
    if (Argnum < 2)
    {
        return;
    }
    bool alltrue = true;
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
        for (int i = 0; i < pow(2, Argnum); i++)
        {
            if (Value[i] == 0)
            {
                if (CNFMstr.size() == 0)
                {
                    CNFMstr += " ^ M(";
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
        CNFMstr.pop_back();
        CNFMstr.push_back(')');
    }
    else if (mode == 2)
    {
        for (int i = 0; i < pow(2, Argnum); i++)
        {
            if (Value[i] == 0)
            {
                if (CNFMstr.size() == 0)
                {
                    CNFMstr += " \\prod M(";
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
        CNFMstr.pop_back();
        CNFMstr.push_back(')');
    }
    if (mode == 1)
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
    bool allfalse = true;
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
        for (int i = 0; i < pow(2, Argnum); i++)
        {
            if (Value[i] == 1)
            {
                if (DNFmstr.size() == 0)
                {
                    DNFmstr += " v m(";
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
        DNFmstr.pop_back();
        DNFmstr.push_back(')');
    }
    else if (mode == 2)
    {
        for (int i = 0; i < pow(2, Argnum); i++)
        {
            if (Value[i] == 1)
            {
                if (DNFmstr.size() == 0)
                {
                    DNFmstr += " \\sum m(";
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
        DNFmstr.pop_back();
        DNFmstr.push_back(')');
    }
    DNFstr.pop_back();
}

void Reasoning::PrintNF()
{
    CNF(); // 生成CNF
    DNF(); // 生成DNF
    if (CNFstr.size() == 0 && DNFstr.size() == 0)
    {
        return;
    }
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
    cin >> openzhenzhibiao;
    if (openzhenzhibiao != 0 && openzhenzhibiao != 1)
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
    cin >> openkanuo;
    if (openkanuo != 0 && openkanuo != 1)
    {
        cout << "输入错误" << endl;
        while (getchar() != '\n');
        Setting();
    }
    cout << "是否输出卡诺图化简(1/0):";
    cin >> openkanuohuajian;
    if (openkanuohuajian != 0 && openkanuohuajian != 1)
    {
        cout << "输入错误" << endl;
        while (getchar() != '\n');
        Setting();
    }
}

void Reasoning::Run()
{
    cout << "否定:~、合取:^、析取:v/否定:`(兼容'或‘或’)、合取:*、析取:+、条件:>、双条件:<、异或:@、与非:[、或非:]" << endl;
    cout << "命题变元不区分大小写,只能为A-Z除V外的字母,支持中文括号" << endl;
    cout << "输入/end或/exit结束,/setting设置输出选项,/clear清空页面" << endl;
    cout << "支持离散数学和数字逻辑两种符号体系，请勿混用" << endl;
    cout << "示例输入:~(~A^Bv(A^~BVc)^A>C)>(Cv(A<B))" << endl;
    cout << "示例输入:(A`(B+C`))`(A+B`+C) 或 (A`(B+C`))`*(A+B`+C)" << endl;

    while (1)
    {
        Init();
        Input();
        if (ifend)
        {
            break;
        }
        Cal();
        if (openzhenzhibiao == 1)
        {
            MakeTable(); // 生成真值表
        }
        if (openNF == 1)
        {
            PrintNF(); // 打印结果
        }
        if (openkanuo == 1)
        {
            MakeKanuo(); // 生成卡诺图
        }
        if (openkanuohuajian == 1)
        {
            QM(); // 生成卡诺图化简
        }
    }
}

void Reasoning::MakeKanuo()
{
    if (Argnum != 3 && Argnum != 4)
    {
        cout << "不支持" << Argnum << "个变元的卡诺图" << endl;
    }
    
    cout << endl << "卡诺图:" << endl;
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

int Reasoning::Countone(string terms)
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

bool Reasoning::IfNear(const string& a, const string& b)
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

string Reasoning::Combine(const string& a, const string& b)
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

int Reasoning::CountDashes(const string& str)
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

void Reasoning::QM()
{
    if (DNFstr == "F")
    {
        cout << endl << "卡诺图化简结果: 该命题为重言式,无需化简" << endl;
        return;
    }
    else if (CNFstr == "T")
    {
        cout << endl << "卡诺图化简结果: 该命题为永真式,无需化简" << endl;
        return;
    }
    else if (DNFstr.size() == 0)
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
            cout << endl << "卡诺图化简结果: 该命题为永真式,无需化简" << endl;
            return;
        }
        if (allfalse)
        {
            cout << endl << "卡诺图化简结果: 该命题为永假式,无需化简" << endl;
            return;
        }
    }
    
    // 将PI转为二进制字符串
    vector<vector<string>> groups;
    for (int i = 0; i < Argnum+1; i++)
    {
        groups.push_back(vector<string>());
    }
    for (int i = 0; i < PI.size(); i++)
    {
        string binstr;
        int bin[Argnum] = {0};
        DToB(PI[i], bin);
        for (int j = 0; j < Argnum; ++j)
        {
            binstr += to_string(bin[Argnum-j-1]);
        }
        groups[Countone(binstr)].push_back(binstr);
    }

    while (!endinter)
    {
        groups = QMCombine(groups); // 合并项
    }

    vector<string> finalPI;
    for (const auto& group : groups)
    {
        for (const auto& term : group)
        {
            finalPI.push_back(term);
        }
    }

    if (mode == 1)
    {
        for (const auto& term : finalPI)
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
        for (const auto& term : finalPI)
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

    cout << endl << "卡诺图化简结果: " << kanuo << endl;
}

vector<vector<string>> Reasoning::QMCombine(const vector<vector<string>>& groups)
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
            if (used[i][j])
            {
                continue; // 跳过已经被合并的项
            }
            for (int k = 0; k < groups[i+1].size(); k++) // 遍历有i+1个1的组
            {
                if (used[i+1][k])
                {
                    continue; // 跳过已经被合并的项
                }
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
                        used[i][j] = true;
                        used[i+1][k] = true;
                        break;
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