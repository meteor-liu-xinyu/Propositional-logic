#include "reasoning.h"

Reasoning::Reasoning()
{
    mode = 0;
    ifend = false;
    openTheTruthTable = true;
    openNF = true;
    openKanuo = true;
    openKanuoSimplify = true;
    testing = false;
    return;
}

Reasoning::~Reasoning()
{
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
    unrelatedItems.clear();
    mode = 0;
    endinter = false;
}

void Reasoning::Input()
{
    Init(); // 初始化

    cout << endl << "请输入：";
    getline(cin, input);
    // input = "F(A,B,C,D)=∑ m(0,2,3,5,6,7,8,9)+ ∑ d(10,11,12,13,14,15)"; // !测试用例
    // input = "(A+B`)(A+C)"; // !测试用例
    // input = "F(a,b,c,d)=∑(1,3,4,5,7,8,9,11,15)"; // !测试用例
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
    else if (input == "/test" || input == "/TEST") // 测试模式，输出运算过程
    {
        testing = true;
        cout << "测试模式开启" << endl;
        skip = true;
        return;
    }
    else if (input == "/help" || input == "/HELP" || input == "/?" || input == "/h" || input == "/H") // 输出帮助信息
    {
        cout << "否定:~、合取:^、析取:v/否定:`(兼容'或‘或’)、合取:*、析取:+、条件:>、双条件:<、异或:@、与非:[、或非:]" << endl;
        cout << "命题变元不区分大小写,只能为A-Z除V外的字母,支持中文括号" << endl;
        cout << "输入/end或/exit结束,/setting设置输出选项,/truthtable用真值表形式输入,/clear清空页面,/help帮助" << endl;
        cout << "支持离散数学和数字逻辑两种符号体系，请勿混用" << endl;
        cout << "示例输入:~(~A^Bv(A^~BVc)^A>C)>(Cv(A<B))" << endl;
        cout << "示例输入:(A`(B+C`))`(A+B`+C) 或 (A`(B+C`))`*(A+B`+C)" << endl;
        cout << "示例输入:F(x,y,z)=∑ m(2,3,5,7)或F(x,y,z)=∏ M(2,3,5,7)" << endl;
        cout << "示例输入:F(A,B,C,D)=∑ m(0,2,3,5,6,7,8,9)+ ∑ d(10,11,12,13,14,15)" << endl;
        cout << "-------------------------------------------------------------------------------";
        skip = true;
        return;
    }
    else if (input == "/truthtable" || input == "/TRUTHTABLE") // 输入真值表
    {
        cout << "请输入命题变元,以逗号分隔" << endl;
        getline(cin, input);
        mode = 5; // 真值表模式
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
        else if (input[i] == '\'') // 将英文单引号转换为`
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

    if (mode == 5)
    {
        FindArg();
        cout << endl << "真值表输入:" << endl;
        for (int i = 0; i < Argnum; i++)
        {
            cout << ArgName[i]<< "\t";
        }
        cout << " 预期输出的真值 " << endl;
        string trans_input = "F(";
        for (int i = 0; i < Argnum; i++)
        {
            trans_input += ArgName[i];
            trans_input += ",";
        }
        trans_input.erase(trans_input.length() - 1, 1);
        trans_input += ")=∑m(";
        for (int i = 0; i < powArgnum; i++)
        {
            string binstr = ToBin[i]; // 获取二进制数
            for (int j = 0; j < Argnum; j++)
            {
                cout << binstr[j] << "\t";
            }
            cout << "\t";
            string temp;
            getline(cin, temp);
            if (temp == "1")
            {
                trans_input += to_string(i);
                trans_input += ",";
            }
            else if (temp == "0");
            else if (temp == "-")
            {
                unrelatedItems.push_back(i);
            }
            else
            {
                cout << endl << "输入错误" << endl;
                i--;
                continue;
            }
            cout << endl;
        }
        trans_input.erase(trans_input.length() - 1, 1);
        trans_input += ")";
        if (unrelatedItems.size() != 0)
        {
            trans_input += "+∑D(";
            for (int i = 0; i < unrelatedItems.size(); i++)
            {
                trans_input += to_string(unrelatedItems[i]);
                trans_input += ",";
            }
            trans_input.erase(trans_input.length() - 1, 1);
            trans_input += ")";
        }
        input = trans_input;
        unrelatedItems.clear();
        ArgName.clear();
        Argnum = 0;
        powArgnum = 0;
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

    int temp = 0;
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
            else if (input[i] == '>' || input[i] == '<' || input[i] == '@' || input[i] == '[' || input[i] == ']')
            {
                temp = 1;
            }

        }
        if (mode == 0 && temp == 1)
        {
            mode = 1; // 默认
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
            if (input[i] == 'm')
            {
                input[i] = 'M';
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
    unordered_map<char, bool> If_in_Arg; // 记录命题变元是否在ArgName中

    if (mode == 1 || mode == 2 || mode == 5)
    {
        for (int i = 0; i < input.length(); i++)
        {
            // 跳过符号
            if (input[i] != '~' && input[i] != '^' && input[i] != 'v' && input[i] != '@' && input[i] != '[' && input[i] != ']' && input[i] != '`' &&input[i] != '*' && input[i] != '+' && input[i] != '>' && input[i] != '<' && input[i] != '(' && input[i] != ')' && input[i] != ',')
            {
                if (If_in_Arg[input[i]] == false)
                {
                    ArgName.push_back(input[i]);
                    Argnum++;
                    If_in_Arg[input[i]] = true;
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
            if (input[i] == 'v')
            {
                input[i] = 'V';
            }
            if (input[i] >= 'A' && input[i] <= 'Z')
            {
                if (If_in_Arg[input[i]] == false)
                {
                    ArgName.push_back(input[i]);
                    Argnum++;
                    If_in_Arg[input[i]] = true;
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
    if (mode == 5)
    {
        BuildHashTable();
    }
}

void Reasoning::BuildHashTable()
{
    ToBin.clear();
    ToDec.clear();
    for (int i = 0; i < powArgnum; i++)
    {
        string binstr;
        for (int j = 0; j < Argnum; j++)
        {
            binstr += (i >> j) & 1 ? '1' : '0'; // 将十进制数转换为二进制数
        }
        // 反转字符串
        for (int j = 0; j < binstr.length() / 2; j++)
        {
            char temp = binstr[j];
            binstr[j] = binstr[binstr.length() - j - 1];
            binstr[binstr.length() - j - 1] = temp;
        }
        ToBin[i] = binstr;
        ToDec[binstr] = i;
        If_unrelated[i] = false; // 初始化无关项
    }
}

int Reasoning::CalculateValue(int n) // 计算真值表n行的真值
{
    string binstr = ToBin[n]; // 获取二进制数

    // 用T/F替换ArgName对应的值,存入temp中
    string temp = input;
    for (int i = 0; i < input.length(); i++)
    {
        for (int j = 0; j < Argnum; j++)
        {
            if (temp[i] == ArgName[j])
            {
                if (binstr[j] == '1')
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
        temp.clear();

        input.erase(0, i+1);
        i = 0;
        if (input[i] == '+' && input[i+1] == '\342' && input[i+2] == '\210' && input[i+3] == '\221' && input[i+4] == 'D')
        {
            input.erase(i, 5);
            i++;
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
                    unrelatedItems.push_back(tempnum);
                    If_unrelated[tempnum] = true;
                    temp.clear();
                }
            }
            int tempnum = atoi(temp.c_str());
            unrelatedItems.push_back(tempnum);
            If_unrelated[tempnum] = true;

            int unrelatedItemsSize = unrelatedItems.size();

            unordered_map<int,string> tempToBin; // 临时哈希表
            for (int j = 0; j < pow(2,unrelatedItemsSize); j++)
            {
                string binstr;
                for (int k = 0; k < unrelatedItemsSize; k++)
                {
                    binstr += (j >> k) & 1 ? '1' : '0'; // 将十进制数转换为二进制数
                }
                // 反转字符串
                for (int k = 0; k < binstr.length() / 2; k++)
                {
                    char temp = binstr[k];
                    binstr[k] = binstr[binstr.length() - k - 1];
                    binstr[binstr.length() - k - 1] = temp;
                }
                tempToBin[j] = binstr;
            }

            vector<string> kanuo_all;
            for (int j = 0; j < pow(2,unrelatedItemsSize); j++)
            {
                for (int k = 0; k < unrelatedItemsSize; k++)
                {
                    Value[unrelatedItems[k]] = tempToBin[j][k] - '0';
                }
                for (int k = 0; k < powArgnum; k++)
                {
                    if (Value[k] == 1)
                    {
                        PI.push_back(k);
                    }
                }
                QM();
                kanuo_all.push_back(kanuo);
                kanuo.clear();
                PI.clear();
            }

            int j = 0;
            for (int k = 0; k < pow(2,unrelatedItemsSize); k++)
            {
                if (kanuo_all[j].size()>kanuo_all[k].size())
                {
                    j = k;
                }
            }

            for (int k = 0; k < unrelatedItemsSize; k++)
            {
                Value[unrelatedItems[k]] = tempToBin[j][k] - '0';
            }
        }

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
    cout << endl << "真值表:" << endl;
    for (int i = 0; i < Argnum; i++)
    {
        cout << ArgName[i]<< "\t";
    }
    cout << initialinput << endl;

    for (int i = 0; i < powArgnum; i++)
    {
        string binstr = ToBin[i]; // 获取二进制数
        for (int j = 0; j < Argnum; j++)
        {
            cout << binstr[j] << "\t";
        }
        if (initialinput.size() >= 15)
        {
            cout << "\t";
        }
        else if (initialinput.size() >= 8)
        {
            cout << "  ";
        }
        if (If_unrelated[i] == true)
        {
            cout << "  ?=" << Value[i] << endl;
        }
        else
        {
            cout << "  " << Value[i] << endl;
        }
    }
}

void Reasoning::NF()
{
    if (Argnum == 1) // 只有一个变量时,不输出
    {
        cout << "只有一个变元,不计算范式" << endl;
        return;
    }

    bool alltrue = true; // 判断是否为永真式
    bool allfalse = true; // 判断是否为重言式
    for (const auto& i : Value)
    {
        if (i == 0)
        {
            alltrue = false;
        }
        else
        {
            allfalse = false;
        }
        if (!alltrue && !allfalse) // 既不是永真式也不是重言式
        {
            break;
        }
    }
    if (alltrue)
    {
        CNFstr = 'T';
        CNFMstr = 'T';
        return;
    }
    if (allfalse)
    {
        DNFstr = 'F';
        DNFmstr = 'F';
        return;
    }

    string NFstr[2] = {};
    string NFMstr[2] = {"∏ M(","∑ m("};

    int count[2] = {0, 0};
    for (int i = 0; i < powArgnum; i++)
    {
        int CorD = Value[i]; // 0--CNF, 1--DNF
        NFMstr[CorD].append(to_string(i));
        NFMstr[CorD].push_back(',');
        string binstr = ToBin[i]; // 获取二进制数
        if (!(CorD == 1 && mode == 2))
        {
            NFstr[Value[i]].push_back('(');
        }
        for (int j = 0; j < Argnum; j++)
        {
            if (binstr[j] == '1' - CorD)
            {
                if (mode == 1)
                {
                    NFstr[CorD].push_back('~');
                    NFstr[CorD].push_back(ArgName[j]);
                }
                else if (mode == 2)
                {
                    NFstr[CorD].push_back(ArgName[j]);
                    NFstr[CorD].push_back('`');
                }
            }
            else
            {
                NFstr[CorD].push_back(ArgName[j]);
            }
            if (j != Argnum-1)
            {
                if (CorD == 0)
                {
                    if (mode == 1)
                    {
                        NFstr[0].push_back('v');
                    }
                    else if (mode == 2)
                    {
                        NFstr[0].push_back('+');
                    }
                }
                else // CorD == 1
                {
                    if (mode == 1)
                    {
                        NFstr[1].push_back('^');
                    }
                }
            }
        }
        if (CorD == 0)
        {
            NFstr[0].push_back(')');
            if (mode == 1)
            {
                NFstr[0].push_back('^');
            }
        }
        else // CorD == 1
        {
            if (mode == 1)
            {
                NFstr[1].push_back(')');
                NFstr[1].push_back('v');
            }
            else if (mode == 2)
            {
                NFstr[1].push_back('+');
            }
        }
        count[CorD]++;
        if (count[CorD] % 8 == 0 && NFstr[CorD][NFstr[CorD].length()-1] != '\n' && count[CorD] != 0)
        {
            NFstr[CorD].push_back('\n');
        }
    }
    if (NFstr[0][NFstr[0].length()-1] == '\n')
    {
        NFstr[0].pop_back();
    }
    if (NFstr[1][NFstr[1].length()-1] == '\n')
    {
        NFstr[1].pop_back();
    }
    if (mode == 1)
    {
        NFstr[0].pop_back();
    }
    NFMstr[0].pop_back();
    NFMstr[0].push_back(')');
    NFstr[1].pop_back();
    NFMstr[1].pop_back();
    NFMstr[1].push_back(')');

    CNFstr = NFstr[0];
    CNFMstr = NFMstr[0];
    DNFstr = NFstr[1];
    DNFmstr = NFMstr[1];
}

void Reasoning::PrintNF() // 打印主合取、析取范式
{
    NF();
    cout << endl << "主合取范式:" << endl;
    cout << CNFstr << endl;
    cout << CNFMstr << endl;
    cout << endl << "主析取范式:" << endl;
    cout << DNFstr << endl;
    cout << DNFmstr << endl;
}

void Reasoning::Setting() // 设置输出选项
{
    string temp;
    openTheTruthTable = false;
    openNF = false;
    openKanuo = false;
    openKanuoSimplify = false;
    cout << "是否输出真值表(1/0):";
    getline(cin, temp);
    if (temp[0] != '0')
    {
        openTheTruthTable = true;
    }
    cout << "是否输出主合取+析取范式(1/0):";
    getline(cin, temp);
    if (temp[0] != '0')
    {
        openNF = true;
    }
    cout << "是否输出卡诺图(1/0):";
    getline(cin, temp);
    if (temp[0] != '0')
    {
        openKanuo = true;
    }
    cout << "是否输出卡诺图化简(1/0):";
    getline(cin, temp);
    if (temp[0] != '0')
    {
        openKanuoSimplify = true;
    }
    if (openTheTruthTable == false && openNF == false && openKanuo == false && openKanuoSimplify == false)
    {
        cout << "至少选择一个输出选项" << endl;
        Setting();
    }
}

void Reasoning::Run()
{
    cout << "否定:~、合取:^、析取:v/否定:`(兼容'或‘或’)、合取:*、析取:+、条件:>、双条件:<、异或:@、与非:[、或非:]" << endl;
    cout << "命题变元不区分大小写,只能为A-Z除V外的字母,支持中文括号" << endl;
    cout << "输入/end或/exit结束,/setting设置输出选项,/truthtable用真值表形式输入,/clear清空页面,/help帮助" << endl;
    cout << "支持离散数学和数字逻辑两种符号体系，请勿混用" << endl;
    cout << "示例输入:~(~A^Bv(A^~BVc)^A>C)>(Cv(A<B))" << endl;
    cout << "示例输入:(A`(B+C`))`(A+B`+C) 或 (A`(B+C`))`*(A+B`+C)" << endl;
    cout << "示例输入:F(x,y,z)=∑ m(2,3,5,7)或F(x,y,z)=∏ M(2,3,5,7)" << endl;
    cout << "示例输入:F(A,B,C,D)=∑ m(0,2,3,5,6,7,8,9)+ ∑ d(10,11,12,13,14,15)" << endl;
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
        BuildHashTable();
        Cal();
        if (openTheTruthTable)
        {
            cout << "-----------------------------------------";
            MakeTable(); // 生成真值表
        }
        if (openNF)
        {
            cout << "-----------------------------------------";
            PrintNF(); // 打印结果
        }
        if (openKanuo)
        {
            cout << "-----------------------------------------" << endl;
            MakeKanuo(); // 生成卡诺图
        }
        if (openKanuoSimplify)
        {
            cout << "-----------------------------------------" << endl;
            QM(); // 生成卡诺图化简
            cout << "卡诺图化简结果: " << kanuo << endl;
        }
        cout << "-------------------------------------------------------------------------------";
        // break; // !test
    }
}

void Reasoning::MakeKanuo() // 生成卡诺图
{
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
                if (If_unrelated[temp1[i]+temp2[j]] == true)
                {
                    if (Value[temp1[i]+temp2[j]] == 1)
                    {
                        cout << "?=1 \t";
                    }
                    else
                    {
                        cout << " ?=0 \t";
                    }
                }
                else
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
                if (If_unrelated[temp1[i]+temp2[j]] == true)
                {
                    if (Value[temp1[i]+temp2[j]] == 1)
                    {
                        cout << "?=1 \t";
                    }
                    else
                    {
                        cout << " ?=0 \t";
                    }
                }
                else
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
            }
            cout << endl;
        }
    }
}

void Reasoning::QM() // 卡诺图化简
{
    if (Argnum == 1)
    {
        cout << "卡诺图化简结果: 只有一个变元,无需化简" << endl;
        return;
    }
    if (openNF == false) // 未调用NF()函数，无DNFstr和CNFstr
    {
        bool alltrue = true;
        bool allfalse = true;
        for (const auto& i : Value)
        {
            if (i == 0)
            {
                alltrue = false;
            }
            else // i == 1
            {
                allfalse = false;
            }
            if (allfalse == false && alltrue == false)
            {
                break;
            }
        }
        if (alltrue)
        {
            cout << "卡诺图化简结果: T" << endl;
            return;
        }
        if (allfalse)
        {
            cout << "卡诺图化简结果: F" << endl;
            return;
        }
    }
    else
    {
        if (DNFstr == "F")
        {
            cout << "卡诺图化简结果: F" << endl;
            return;
        }
        else if (CNFstr == "T")
        {
            cout << "卡诺图化简结果: T" << endl;
            return;
        }
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

    if (testing) // !test
    {
        cout << endl;
        for (int i = 0; i < groups.size(); i++)
        {
            cout << i << "个1组: ";
            for (const auto& term : groups[i])
            {
                cout << term << " ";
            }
            cout << endl;
        }
        cout << endl;
    }

    endinter = false;
    while (!endinter) // 循环合并项
    {
        groups = QMCombine(groups); // 合并项
        if (testing) // !test
        {
            cout << "合并后: " << endl;
            for (int i = 0; i < groups.size(); i++)
            {
                cout << i << "个1组: ";
                for (const auto& term : groups[i])
                {
                    cout << term << " ";
                }
                cout << endl;
            }
            cout << endl;
        }
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
    vector<string> bin;
    for (int i = 0; i < pow(2,Argnum); i++)
    {
        bin.push_back(ToBin[i]);
    }
    for (int i = 0; i < finalPIsize; i++)
    {
        table.push_back(row);
        for (int j = 0; j < pow(2,Argnum); j++)
        {
            bool flag = true;
            for (int k = 0; k < Argnum; k++)
            {
                if (finalPI[i][k] == '1' && bin[j][k] == '0')
                {
                    flag = false;
                    break; // 不满足
                }
                else if (finalPI[i][k] == '0' && bin[j][k] == '1')
                {
                    flag = false;
                    break; // 不满足
                }
            }
            if (flag)
            {
                table[i][j] = 1;
            }
            else
            {
                table[i][j] = 0;
            }
        }
    }

    if (testing) // !test
    {
        cout << "基本蕴含项表: " << endl;
        for (int i = 0; i < finalPIsize; i++)
        {
            cout << finalPI[i] << "  ";
            for (int j = 0; j < pow(2,Argnum); j++)
            {
                cout << table[i][j] << "  ";
            }
            cout << endl;
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

    if (testing) // !test
    {
        cout << "最终的基本蕴含项: " << endl;
        for (const auto& term : finalEPI)
        {
            cout << term << "   ";
        }
        cout << endl;
    }

    // 从finalEPI中计算kanuo化简结果
    for (const auto& term : finalEPI)
    {
        if (mode == 1 && CountDashes(term) < Argnum-1)
        {
            kanuo.push_back('(');
        }
        for (int i = 0; i < term.size(); i++)
        {
            if (term[i] == '1')
            {
                kanuo.push_back(ArgName[i]);
            }
            else if (term[i] == '0')
            {
                if (mode == 1)
                {
                    kanuo.push_back('~');
                    kanuo.push_back(ArgName[i]);
                }
                else // mode == 2
                {
                    kanuo.push_back(ArgName[i]);
                    kanuo.push_back('`');
                }
            }
            if (term[i] != '-' && mode == 1)
            {
                kanuo.push_back('^');
            }
        }
        if (mode == 1)
        {
            kanuo.pop_back();
        }
        if (mode == 1)
        {
            if (CountDashes(term) < Argnum-1)
            {
                kanuo.push_back(')');
            }
            kanuo.push_back('v');
        }
        else // mode == 2
        {
            kanuo.push_back('+');
        }
    }
    kanuo.pop_back();
}

vector<vector<string>> Reasoning::QMCombine(const vector<vector<string>>& groups) // 合并项
{
    endinter = true; // 标记是否已经不能再合并
    vector<vector<string>> nextgroups; // 存储合并后的项
    for (int i = 0; i < Argnum+1; i++)
    {
        nextgroups.push_back(vector<string>());
    }

    unordered_map<string, bool> If_in_next; // 标记nextgroups中是否已有
    // 初始化
    for (const auto& group : groups)
    {
        for (const auto& term : group)
        {
            If_in_next[term] = false;
        }
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
                        endinter = false; // 如果有新的合并，则继续合并
                        if (If_in_next[combined] == false) // 如果nextgroups中没有该项
                        {
                            nextgroups[Countone(combined)].push_back(combined);
                            If_in_next[combined] = true;
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