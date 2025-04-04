#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <string>

using namespace std;

class Operations
{
public:
    Operations();
    ~Operations();
    
    int FindLeft(const string& temp);
    int FindRight(const string& temp);

    int Countone(string terms);
    bool IfNear(const string& a, const string& b);
    string Combine(const string& a, const string& b);
    int CountDashes(const string& str);

    char Not(char A);
    char And(char A, char B);
    char Or(char A, char B);
    char If(char A, char B);
    char Iff(char A, char B);
    char Xor(char A, char B);
    char Nand(char A, char B);
    char Nor(char A, char B);
};


#endif
