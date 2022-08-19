#ifndef Action_HPP_INCLUDED
#define Action_HPP_INCLUDED
#include <functional>
#include <list>
#include<iostream>
#include <string>


class action
{
    typedef std::function<void()>Fun;
private:
    std::list<Fun>Funlist;
    /* data */
public:
    action() = default;
    ~action() = default;
    action& operator+=(std::function<void()> func);
    void run();
};

action& action::operator+=(std::function<void()>func)
{
    this->Funlist.push_back(func);
    return *this;
}
void action::run()
{
    for (std::function<void()>& act : Funlist)
    {
        act();
    }
}

class Func
{
public:
    Func(std::string s) :s(s) {};
    Func(std::string s, action& ac);
    void F();
private:
    std::string s;

};
inline Func::Func(std::string s, action& ac)
{
    this->s = s;
    ac += std::bind(&Func::F, this);
}
void Func::F()
{
    std::cout << this->s << std::endl;
}
#endif