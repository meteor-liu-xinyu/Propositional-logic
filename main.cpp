#include "zhenzhibiao.h"
#include <windows.h>

int main()
{
    SetConsoleOutputCP(65001);

    Reasoning reasoning;

    reasoning.Run();

    cout << "已退出" << endl;

	return 0;
}