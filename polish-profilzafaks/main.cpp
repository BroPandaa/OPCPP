#include <iostream>
#include "evaluate_postfix.h"
#include "evaluate_prefix.h"


int main(){

// VAŠI testovi dolaze ovdje. Barem 3 test za prefix i postfix formu.

    std::string expr1 = " 2 3 ^ 8 3 * +";
    IPExpression ipexp1(expr1);
    std::cout  << expr1  << " = "  << ipexp1.evaluate()  << std::endl;

    std::string expr2 = " 5 7 + 8 * 6 /";
    IPExpression ipexp2(expr2);
    std::cout  << expr2  << " = "  << ipexp2.evaluate()  << std::endl;

    std::string expr3 = " 3 4 ^ 9 / ~ 8 +";
    IPExpression ipexp3(expr3);
    std::cout  << expr3  << " = "  << ipexp3.evaluate()  << std::endl;

    std::string pexpr1 = "~ / ^ 8 5 4 ";
    Expression expr4(pexpr1);
    std::cout << pexpr1  << " = "  << expr4.evaluate()  << std::endl;

    std::string pexpr2 = "- + 5 3 2 * 8 ";
    Expression expr5(pexpr2);
    std::cout << pexpr2  << " = "  << expr5.evaluate()  << std::endl;

    std::string pexpr3 = "* / + 9 7 8 - 4 5";
    Expression expr6(pexpr3);
    std::cout << pexpr3  << " = "  << expr6.evaluate()  << std::endl;

    /*std::string greska1 = " 5 7 8 a 1 - +";
    IPExpression greska(greska1);
    std::cout  << greska1  << " = "  << greska.evaluate()  << std::endl;*/

    return 0;
}