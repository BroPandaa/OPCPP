#ifndef EVALUATE_POSTFIX_H
#define EVALUATE_POSTFIX_H
#include <string>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <stack>
#include <sstream>
#include <cmath>

/*  Klasa za izračunavanje aritmetičkog izraza danog u inverznoj poljskoj notaciji.
 *  Binarne operacije su +,-,*,/ i ^.
 *  Unarne operacije ~ (negacija). Koristimo ~ umjesto - kako bismo
 *  mogli razlikovati unarnu od binarne operacije prema simbolu.
 */
class IPExpression{
public:
    // tip izraza
    using Value = int;

    enum TokenType {operand, unaryOp, binaryOp, endToken};

    // pomoćna klasa koja predstavlja element izraza
    struct Token{
        TokenType type() const;
        char symbol() const;
        Value value() const;
    };

    // Konstrukcija izraza iz stringa.
    // text mora sadržavati aritmetički izraz u poljskoj notaciji
    IPExpression(std::string text);
    // Izračunaj vrijednost izraza. Uništava izraz
    Value evaluate();
private:
    // Daje sljedeći token sadržan u izrazu
    Token getToken();
    std::string mText;
};

// NAPOMENA: Nije nužno koristiti unutarnju klasu  Expression::Token
//           ukoliko imate bolji način, no unutarnje klase su ovdje vrlo
//           prirodne.

#endif // EVALUATE_POSTFIX_H
