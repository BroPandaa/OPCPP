// Ovdje dolazi vaš kod za Expression klasu dolazi ovdje.

#include "evaluate_prefix.h"
#include <iostream>
Expression::Expression(std::string text)
{
    std::runtime_error error("Pogresan unos izraza");
    mText=text;
    int i;
    mText.erase(remove_if(mText.begin(), mText.end(), isspace), mText.end());

    for(i=0; i<mText.length(); i++){
        if(!isdigit(mText[i]) && !(mText[i]=='+' || mText[i]=='^' || mText[i]=='-' || mText[i]=='*' || mText[i]=='/' || mText[i]=='~')) throw error;
    }
    if(mText[i]=='+' || mText[i]=='-' || mText[i]=='*' || mText[i]=='/' || mText[i]=='~' || mText[i]=='^'){
        throw error;
    }
    mNextToken=0;
}

Expression::Token Expression::getToken()
{
    Token novi;
    if(mText[mNextToken]=='+' || mText[mNextToken]=='-' || mText[mNextToken]=='*' || mText[mNextToken]=='/' || mText[mNextToken]=='^'){
            novi.type=binaryOp;
            novi.symbol=mText[mNextToken];  
            }
    else if(mText[mNextToken]=='~'){
            novi.type=unaryOp;
            novi.symbol='~';
    }
    else{
        novi.type=operand;
        char a=mText[mNextToken];
        int inta=a -'0'; 
        novi.value=inta;
    }
    ++mNextToken;
    return novi;
}

Expression::Value Expression::evaluate()
{
    Token t=getToken();

    if(mNextToken<mText.length()){
        if(t.type==operand){
            return t.value;
        }
        if(t.type==unaryOp){
            Value a=evaluate();
            return (-1)*a;
        }
        else if(t.type==binaryOp){
            Value b=evaluate();
            Value c=evaluate();
            switch(t.symbol){
                case '+': return b+c; break;
                case '-': return b-c; break;
                case '*': return b*c; break;
                case '/': return b/c; break;
                case '^': return pow(b,c); break;
            }
        }
    }
    else return t.value;
}