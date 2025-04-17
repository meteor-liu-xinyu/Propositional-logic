#include "reasoning.h"
#include <windows.h>
#include <conio.h> // 用于 _getch() 函数

int main()
{
    // 设置控制台输入和输出编码为 UTF-8
    SetConsoleCP(65001); // 设置控制台输入代码页为 UTF-8
    SetConsoleOutputCP(65001); // 设置控制台输出代码页为 UTF-8

    Reasoning reasoning;

    reasoning.Run();

    cout << "已退出,按任意键关闭" << endl;
    _getch();
    return 0;
}