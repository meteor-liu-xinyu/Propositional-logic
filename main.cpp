#include "runwindow.h"
#include <windows.h>

int main()
{
    SetConsoleOutputCP(65001);

    RunWindow Main;

    while (getchar() != '\n');
    getchar();
    
	return 0;
}