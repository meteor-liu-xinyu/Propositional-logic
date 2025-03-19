#ifndef ZHENZHIBIAO_H
#define ZHENZHIBIAO_H

#include <iostream>
#include <vector>
using namespace std;

class Reasoning
{
private:
    string input;
    vector<char> ArgName;
    int Argnum;
    vector<int> Value;
    string CNFstr;
    string DNFstr;
    
public:
    Reasoning();
    ~Reasoning();
    void FindArg();
    int FindLeft(string temp);
    int FindRight(string temp);
    int CalculateValue(int n);
    void DToB(int n, int bin[]);
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

};



#endif