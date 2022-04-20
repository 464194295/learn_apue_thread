#include <iostream>
#include <thread>
#include <unistd.h>


using namespace std;

class scoped_thread
{
private:
    thread t;
public:
    explicit scoped_thread(thread t_):t(move(t_)){
        cout<<"scoped thread is construct"<<endl;
        if(!t.joinable()){
            throw logic_error("No thread");
        }
    }
    ~scoped_thread(){
        cout<<"scoped thread is destruct"<<endl;
        t.join();
    }
    scoped_thread(scoped_thread const&)=delete;
    scoped_thread& operator=(scoped_thread const&)=delete;
};




void some_other_function()
{
    cout<<"this is some other function"<<endl;
    return;   
}

void some_function()
{
    cout<<"this is some function"<<endl;
    return;
}

int main()
{
    some_other_function();
    //effictive 6 使用{}进行初始化
    scoped_thread t(thread{some_function});
    return 0;
}




