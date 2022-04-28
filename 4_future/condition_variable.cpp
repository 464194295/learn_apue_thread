#include <iostream>
#include <thread>
#include <queue>
#include <time.h>
#include <unistd.h>

using namespace std;


struct data_my
{
    int val;
};



mutex mut;
queue<data_my> que;
condition_variable data_cond;

void data_perparation_thread(data_my& num)
{
    cout<<"come data_preparation_come"<<endl;
    lock_guard<mutex> lk(mut);
    que.push(num);
    cout<<"notify one"<<endl;
    data_cond.notify_one();
    cout<<"finsih notify "<<endl;
    
}

void data_processing()
{
    while (1)
    {
        unique_lock<mutex> lk(mut);
        data_cond.wait(lk,[]{
            cout<<!que.empty()<<"come wait"<<endl;
            return !que.empty();
        });
        cout<<"ready to pop"<<endl;
        que.pop();
        break;        
    }
    
}

/*shared_ptr<t> wait_and_pop(){
    shared_ptr<t> res(make_shared<T>(data.front()));
    return res;
}*/

int main()
{
    data_my my1;
    my1.val = 1;
    thread t1(data_perparation_thread,ref(my1));

    

    data_processing();

    
    if(t1.joinable()){
        t1.join();
    }else{
        cout<<"join error"<<endl;
    }

    return 0;
}
