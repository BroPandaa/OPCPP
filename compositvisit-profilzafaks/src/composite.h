#ifndef COMPOSITE_H_INCLUDED
#define COMPOSITE_H_INCLUDED

#include <string>
#include <memory>
#include <fstream>
#include <list>
#include <iostream>
#include <vector>

class Visitor;

class Component{
public:
    using Ptr = std::unique_ptr<Component>; // dealokacija je automatska

    // Konstruktor
    Component(Component * parent = nullptr): mParent(parent){}
    // Destruktor.
    virtual ~Component(){}
    // Pročitaj datoteku u asciidoc formatu sa ulaznog toka i kreiraj Composite strukturu dokumenta.
    virtual void read(std::ifstream & in);
    // Metoda za Visitor obrazac. 
    virtual void accept(Visitor * v);
    //daje string za printanje
    std::string printString();
    // ... eventualne dodatne metode
protected:
    // Lista djece. Svaka komponenta je vlasnik svoje djece.
    std::list<Ptr> mChildren;
    // Pokazivač na roditelja, nullptr ako ga nema.
    Component * mParent;
    //uklanja razmake i = s početka redova
    void CropText(std::string& str);
    //tokenizacija stringa
    std::vector<std::string> tokenize(std::string const & str, char delimiter);
    
};

class Title : public Component{
public:
    Title(std::string const & title, Component * parent = nullptr):Component(parent), mTitle(title){}
    // Pročitaj svoj dio datoteke 
    virtual void read(std::ifstream & in);
    // Za vizitacije.
    virtual void accept(Visitor * v);
    // varijabla koja čuva naslov
    std::string mTitle;
};


class Chapter : public Component {
public:
    Chapter(std::string const & sec, Component * parent = nullptr):Component(parent), mChapter(sec){}
     // Pročitaj svoj dio datoteke 
    virtual void read(std::ifstream & in);
    // Za vizitacije.
    virtual void accept(Visitor * v);
    // varijabla koja čuva naslov poglavlja
    std::string mChapter;
};

class Paragraph : public Component {
public:
    Paragraph(Component * parent = nullptr):Component(parent){}
    // Pročitaj paragraf
    virtual void accept(Visitor * v);
    // Za vizitacije.
    virtual void read(std::ifstream & in) override;
    // varijabla koja čuva čitav paragraf  
    std::string mText;
};


// Bazna klasa za sve vizitore
class Visitor{
public:
    virtual void visitComponent(Component *v){ v->accept(this); }
    virtual void visitTitle(Title *) = 0;
    virtual void visitChapter(Chapter *) = 0;
    virtual void visitParagraph(Paragraph *) = 0;
    virtual ~Visitor(){}
};


// Konkretan vizitor. Pretvara Composite objekt u HTML tekst. 
// [Ne treba dodavati <html> i <body> tagove.]
class HTMLVisitor : public Visitor {
public:
    virtual void visitTitle(Title *) override;
    virtual void visitChapter(Chapter *) override;
    virtual void visitParagraph(Paragraph *) override;
    virtual ~HTMLVisitor(){}
    std::string const & getResult() const { return mHTMLText; }
private:    
     std::string mHTMLText;
};


class Iterator{
public:
    Iterator(std::string const & text); 
    void first();
    void next() ;
    std::string current();
    bool isDone() const;
private:
    // Vaše varijable
};

#endif // COMPOSITE_H_INCLUDED
