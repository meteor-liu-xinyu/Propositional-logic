#include "zhenzhibiao.h"
#include <windows.h>

int main()
{
    SetConsoleOutputCP(65001);

    Reasoning reasoning;

    reasoning.Run();

    while (getchar() != '\n');
    getchar();
    
	return 0;
}