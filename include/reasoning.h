#ifndef REASONING_H
#define REASONING_H

#include <iostream>
#include <vector>
using namespace std;

class Reasoning
{
private:
    string initialinput; // 初始输入
    string input;
    vector<char> ArgName; // 命题变元名
    int Argnum; // 命题变元个数
    vector<int> Value; // 真值
    string CNFstr; // 主合取范式
    string DNFstr; // 主析取范式
    string CNFMstr; // 主合取范式
    string DNFmstr; // 主析取范式
    string kanuo; // 卡诺图化简结果
    int mode; // 1: 离散数学符号体系 2: 数字逻辑符号体系 3: 连乘 4: 连加
    bool ifend; // 是否结束

    bool openTheTruthTable; // 是否输出真值表
    bool openNF; // 是否输出主析取范式和主合取范式
    bool openKanuo; // 是否输出卡诺图结果
    bool openKanuoSimplify; // 是否输出卡诺图化简结果

    vector<int> PI; 
    bool endinter; // 是否结束QM合并循环
    
public:
    Reasoning();
    ~Reasoning();
    void Init();
    void Input();
    void FindArg();
    int FindLeft(const string& temp);
    int FindRight(const string& temp);
    
    int CalculateValue(int n);
    void DToB(int n, int bin[]);
    void Cal();

    char Not(char A);
    char And(char A, char B);
    char Or(char A, char B);
    char If(char A, char B);
    char Iff(char A, char B);
    char Xor(char A, char B);
    char Nand(char A, char B);
    char Nor(char A, char B);
    
    void MakeTable();
    void CNF(); // 主合取范式
    void DNF(); // 主析取范式
    void PrintNF();

    void MakeKanuo();

    int Countone(string terms);
    bool IfNear(const string& a, const string& b);
    string Combine(const string& a, const string& b);
    int CountDashes(const string& str);
    void QM();
    vector<vector<string>> QMCombine(const vector<vector<string>>& groups);

    void Setting();
    void Run();

};



#endif