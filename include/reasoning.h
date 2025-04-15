#ifndef REASONING_H
#define REASONING_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <math.h>
#include <string>

#include "operations.h"

using namespace std;

class Operations; // 前向声明

class Reasoning:Operations
{
private:
    string initialinput; // 初始输入
    string input;
    vector<char> ArgName; // 命题变元名
    int Argnum; // 命题变元个数
    int powArgnum; // 2^Argnum
    vector<int> Value; // 真值
    string CNFstr; // 主合取范式
    string DNFstr; // 主析取范式
    string CNFMstr; // 主合取范式
    string DNFmstr; // 主析取范式
    string kanuo; // 卡诺图化简结果
    vector<int> unrelatedItems; // 无关项
    unordered_map<int, bool> If_unrelated; // 是否无关项
    int mode; // 1: 离散数学符号体系 2: 数字逻辑符号体系 3: 连乘 4: 连加 5: 真值表输入
    bool ifend; // 是否结束
    bool skip; // 是否跳过

    bool openTheTruthTable; // 是否输出真值表
    bool openNF; // 是否输出主析取范式和主合取范式
    bool openKanuo; // 是否输出卡诺图结果
    bool openKanuoSimplify; // 是否输出卡诺图化简结果

    bool testing; // 是否处于测试模式

    vector<int> PI;
    bool endinter; // 是否结束QM合并循环

    unordered_map<string, int> ToDec; // 哈希表
    unordered_map<int, string> ToBin; // 哈希表

public:
    Reasoning();
    ~Reasoning();
    void Init();
    void Input();
    void FindArg();
    void Setting();

    void BuildHashTable(); // 建立哈希表

    int CalculateValue(int n);
    void Cal();

    void MakeTable();

    void NF();
    void PrintNF();

    void MakeKanuo();

    void QM();
    vector<vector<string>> QMCombine(const vector<vector<string>>& groups);

    void Run();
};



#endif