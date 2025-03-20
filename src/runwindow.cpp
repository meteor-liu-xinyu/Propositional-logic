#include "runwindow.h"

RunWindow::RunWindow()
{
    cout << "否定:~或`、合取:^或*、析取:v或+、条件:>、双条件:<、异或:@、与非:[、或非:]" << endl;
    cout << "命题变元不区分大小写,只能为A-Z除V外的字母,支持中文括号,输入/end结束" << endl;
    cout << "支持离散数学和数字逻辑两种符号体系，请勿混用" << endl;
    cout << "示例:" << endl;
    cout << "输入:~(~A^Bv(A^~BVc)^A>C)>(Cv(A<B))" << endl;
    cout << "输入:(A`(B+C`))`(A+B`+C)(A`B`C`)`" << endl;
    cout << "输入:A`B+B(C+D)或A`*B+B*(C+D)" << endl;

    while (1)
    {
        reasoning.Input();
        if (reasoning.CheckEnd() == 1)
        {
            break;
        }
        reasoning.MakeTable(); // 生成真值表
        reasoning.CNF(); // 生成CNF
        reasoning.DNF(); // 生成DNF
        reasoning.PrintNF(); // 打印结果
    }
    
}


RunWindow::~RunWindow()
{
}