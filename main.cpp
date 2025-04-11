#include "reasoning.h"
#include <windows.h>
#include <conio.h>

int main()
{
    SetConsoleOutputCP(65001); // 设置控制台输出编码为UTF-8

    Reasoning reasoning;

    reasoning.Run();

    cout << "已退出" << endl;
    return 0;
}