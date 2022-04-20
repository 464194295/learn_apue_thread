#include <iostream>
#include <thread>
#include <unistd.h>

using namespace std;

void some_function()
{
    cout<<"this is some function"<<endl;
    return;
}

void some_other_function()
{
    cout<<"this is some other function"<<endl;
    return;   
}

//交换所有权是要在线程执行完成后才可以进行的

int main()
{
    thread t1(some_function); 
    t1.join(); 
    thread t2 = move(t1);
    
    sleep(1);
    t1 = thread(some_other_function);
    t1.detach();
    thread t3;
    t3 = move(t2);
    
    return 0;
}
