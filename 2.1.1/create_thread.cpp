#include <iostream>
#include <thread>

using namespace std;

void do_some(){
    cout<<"this is function do_some"<<endl;
}
//thread my_thread(do_some);

class bakcground_task
{
public:
    void operator()()const{
        do_some();
    }
};


int main()
{
    bakcground_task task;
    //thread my_thread(task);

    thread my_thread([]{
        do_some();
    });
    my_thread.join();
    return 0;
}




