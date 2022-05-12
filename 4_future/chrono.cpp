#include <iostream>
#include <chrono>
#include <thread>
#include <future>
#include <mutex>
#include <utility>
#include <unistd.h>
#include <condition_variable>

using namespace std;


int do_something()
{
    //this_thread::sleep_for(chrono::seconds(4));        //休眠3秒
    cout<<"do something"<<endl;
    return 1;
}

int main()
{
    //this_thread::sleep_for(chrono::seconds(3));        //休眠3秒
    //this_thread::sleep_for(chrono::milliseconds(100)); //休眠100毫秒
    //cout << "Hello" << endl;
    future<int> f = async(do_something);

    if(f.wait_for(chrono::milliseconds(3)) == future_status::ready){
        cout<<"is ready"<<endl;
    }else{
        cout<<"time out"<<endl;
    }

    auto start = chrono::high_resolution_clock::now();
    this_thread::sleep_for(chrono::seconds(1));
    auto stop = chrono::high_resolution_clock::now();
    auto temp = stop-start;
    chrono::seconds s = chrono::duration_cast<chrono::seconds>(temp);
    cout<<"took:"<<s.count()<<endl;

    mutex m;
    condition_variable cv;
    auto const timeout = chrono::high_resolution_clock::now() + chrono::seconds(1);
    unique_lock<mutex> lk(m);
    while(1){
        if(cv.wait_until(lk,timeout) == cv_status::timeout){
            cout<<"time out"<<endl;
            break;
        }else{
            cout<<"time enough"<<endl;
        }
    }
   

    return 0;
}


