#include "composite.h"
#include <cctype>
#include <stdexcept>
#include <sstream>

using namespace std;

void Component::CropText(string& str){
    
    auto i = str.find_first_not_of("= \t\n\v");
    if (i!=string::npos) str.erase(0, i);
}

std::vector<std::string> tokenize(std::string const & str, char delimiter)
{
    std::vector<std::string> rezultat;
    std::stringstream podaci(str);
    std::string rijec, prazan; 

    while(getline(podaci, rijec, delimiter))
    {
        if(rijec==prazan) continue;
        rezultat.push_back(rijec);
        }

    return rezultat;
}

void Component::read(ifstream& in){
    string ttl;
    getline(in, ttl);
    CropText(ttl);
    mChildren.push_back(Ptr(new Title(ttl)));
    auto it = mChildren.begin();
    (*it)->read(in);
}

void Title::read(ifstream& in){
    string row;
    while(isspace(in.peek()))
        in.ignore(1);
    char c = in.peek();
    if(c=='='){
        getline(in, row);
        CropText(row);
        mChildren.push_back(Ptr(new Chapter(row, this)));
        auto it = mChildren.end();
        it--;
        (*it)->read(in);
    }
    else{
        mChildren.push_back(Ptr(new Paragraph(this)));
        auto it = mChildren.end();
        it--;
        (*it)->read(in);
    }
}

void Chapter::read(ifstream& in){
    string row;
    char c=in.peek();
    while(isspace(c)){
        getline(in, row);
        c=in.peek();
    }
    if(c!='='){
        mChildren.push_back(Ptr(new Paragraph(this)));
        auto it=mChildren.begin();
        (*it)->read(in);
    }
}

void Paragraph::read(ifstream& in){
    string row;
    while(true){
        if(in.peek()=='=') break;
        getline(in, row);
        mText.append(row);
    }
}

void HTMLVisitor::visitTitle(Title * pTitle){
    mHTMLText.append("<h1>");
    mHTMLText.append(pTitle->mTitle);
    mHTMLText.append("</h1>");
    mHTMLText.append("\n");
}

void HTMLVisitor::visitChapter(Chapter * pChapter){
    mHTMLText.append("<h2>");
    mHTMLText.append(pChapter->mChapter);
    mHTMLText.append("</h2>");
    mHTMLText.append("\n");
}

void HTMLVisitor::visitParagraph(Paragraph * pParagraph){
    vector<string> paragrph = tokenize(pParagraph->mText, ' ');
    mHTMLText.append("<p>");
    bool bold=false, cursive=false;
    for(size_t i = 0; i<paragrph.size(); i++){
        if(!bold && !cursive){
            if(paragrph[i].front()=='*'){
                bold = true;
                string bld = "<b>";
                paragrph[i].erase(paragrph[i].begin());
                paragrph[i].insert(0, bld);
                if(paragrph[i].back()=='*'){
                     bold = false;
                    paragrph[i].pop_back();
                    paragrph[i].append("</b>");
                }
                mHTMLText.append(paragrph[i]);
                mHTMLText.append(" ");
            }
            else if(paragrph[i].front()=='_'){
                cursive = true;
                string crsv = "<i>";
                paragrph[i].erase(paragrph[i].begin());
                paragrph[i].insert(0, crsv);
                if(paragrph[i].back()=='_'){
                     cursive = false;
                    paragrph[i].pop_back();
                    paragrph[i].append("</i>");
                }
                mHTMLText.append(paragrph[i]);
                mHTMLText.append(" ");
            }
            else{
                mHTMLText.append(paragrph[i]);
                mHTMLText.append(" ");
                }
        }
        else if(bold){
            if(paragrph[i].back()=='*'){
                bold = false;
                paragrph[i].pop_back();
                paragrph[i].append("</b>");
                mHTMLText.append(paragrph[i]);
            }
            else{
                mHTMLText.append(paragrph[i]);
                mHTMLText.append(" ");
            }
        }
        else if(cursive){
            if(paragrph[i].back()=='_'){
                cursive = false;
                paragrph[i].pop_back();
                paragrph[i].append("</i>");
                mHTMLText.append(paragrph[i]);
                mHTMLText.append(" ");
            }
            else{
                mHTMLText.append(paragrph[i]);
                mHTMLText.append(" ");
            }
        }
    }
    mHTMLText.append("</p>");
}

void Component::accept(Visitor *v){
    list<Ptr>::iterator iter, it_t, it_c;
    for(iter = mChildren.begin(); iter != mChildren.end(); iter++){
        (*iter)->accept(v);
        if((*iter)->mChildren.empty()) continue;
        else
        {
            for (it_t = (*iter)->mChildren.begin(); it_t != (*iter)->mChildren.end(); it_t++)
            {
                ((*it_t)->accept(v));
                if((*it_t)->mChildren.empty()) continue;
                else
                {
                    for(it_c = (*it_t)->mChildren.begin(); it_c != (*it_t)->mChildren.end(); it_c++) (*it_c)->accept(v);
                }
            }
        } 
    }
}

void Title::accept(Visitor *v){v->visitTitle(this);}

void Chapter::accept(Visitor *v){v->visitChapter(this);}

void Paragraph::accept(Visitor *v){v->visitParagraph(this);}