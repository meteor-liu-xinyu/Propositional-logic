#ifndef ZHENZHIBIAO_H
#define ZHENZHIBIAO_H

#include <iostream>
#include <vector>
using namespace std;

class Reasoning
{
private:
    string initialinput;
    string input;
    vector<char> ArgName;
    int Argnum;
    vector<int> Value;
    string CNFstr;
    string DNFstr;
    string CNFMstr;
    string DNFmstr;
    string kanuo;
    int mode;
    bool ifend;

    int openzhenzhibiao;
    int openNF;
    int openkanuo;
    int openkanuohuajian;

    vector<int> PI;
    bool endinter;
    
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