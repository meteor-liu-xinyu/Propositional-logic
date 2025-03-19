#include "zhenzhibiao.h"
#include<windows.h>

int main()
{
    SetConsoleOutputCP(65001);

    Reasoning Main;

    Main.MakeTable(); // 生成真值表
    Main.CNF(); // 生成CNF
    Main.DNF(); // 生成DNF
    Main.PrintNF(); // 打印结果

    while (getchar() != '\n'); // 清空缓冲区
	
    getchar();
	return 0;
}