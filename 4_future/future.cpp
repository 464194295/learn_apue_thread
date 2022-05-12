#include <iostream>
#include <thread>
#include <future>
#include <unistd.h>
#include <string>

using namespace std;


int return_result_number()
{
    return 5;
}

void do_something()
{
    cout<<"this is a test"<<endl;
}

struct X
{
    void foo(int x,string const& y);
    string bar(string const& y);
};

void X::foo(int x,string const& y){
    cout<<x<<endl;
    cout<<y<<endl;
}

string X::bar(string const& y){
    cout<<"bar:"<<y<<endl;
    string temp = y;
    return temp;
}



int main()
{
    /*
    future<int> the_answer = async(return_result_number);
    cout<<the_answer.get()<<endl;
    do_something();*/
    X x;
    auto f1 = async(&X::foo,&x,42,"hello");//使用的是指针
    auto f2 = async(&X::bar,x,"haha");
    cout<<"f2:"<<f2.get()<<endl;
    return 0;
}
