#include "zhenzhibiao.h"
#include <windows.h>
#include <conio.h>

int main()
{
    SetConsoleOutputCP(65001);

    Reasoning reasoning;

    reasoning.Run();

    cout << "已退出,按任意键关闭程序" << endl;
    _getch();
    return 0;
}