// Ovdje dolazi vaš kod za IPExpression klasu dolazi ovdje.
#include "evaluate_postfix.h"


IPExpression::IPExpression(std::string text)
{
    mText=text;
    std::string chars = "\t\n\v\f\r ";
    mText.erase(0, mText.find_first_not_of(chars));
}


IPExpression::Value IPExpression::evaluate()
{
    std::stack<int> rez;
    std::string token;
    std::runtime_error error("Pogresan unos izraza");
    if(mText[0]=='+' || mText[0]=='-' || mText[0]=='*' || mText[0]=='/' || mText[0]=='~' || mText[0]=='^'){
        throw error;
    }
    for(int i=0; i<mText.size(); i++){
        token=mText[i];
        if(token==" ") continue;
        if(!isdigit(mText[i]) && !(mText[i]=='+' || mText[i]=='^' || mText[i]=='-' || mText[i]=='*' || mText[i]=='/' || mText[i]=='~')) throw error; 
        if(token=="~"){
            int k=(-1)*rez.top();
            rez.pop();
            rez.push(k);
        }
        else if(token=="+" || token=="-" || token=="*" || token=="/" || token=="^"){
            int a=rez.top();
            rez.pop();
            int b=rez.top();
            rez.pop();

            switch(token[0]){
                case '+': rez.push(b+a); break;
                case '-': rez.push(b-a); break;
                case '*': rez.push(b*a); break;
                case '/': rez.push(b/a); break;
                case '^': rez.push(pow(b,a)); break;
            }
        }
        else{
            int c=std::stoi(token);
            rez.push(c);
        }
    }
    if (rez.size()) {
            int n = rez.top();
            while (rez.size()) {
                rez.pop();
            }
        return n;
    }
    throw error;
}