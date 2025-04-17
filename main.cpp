#include "reasoning.h"
#include <windows.h>

int main()
{
    // 设置控制台输入和输出编码为 UTF-8
    SetConsoleCP(65001); // 设置控制台输入代码页为 UTF-8
    SetConsoleOutputCP(65001); // 设置控制台输出代码页为 UTF-8

    Reasoning reasoning;

    reasoning.Run();

    cout << "已退出" << endl;
    system("pause");
    return 0;
}