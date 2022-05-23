#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <utility>
#include <unistd.h>
#include <atomic>
#include <vector>

using namespace std;

vector<int> data_store;
atomic<int> mycount;

void queue_data()
{
    const int number_of_item = 20;
    data_store.clear();
    for(int i=0;i<number_of_item;i++){
        data_store.push_back(i);
    }
    mycount.store(number_of_item,memory_order_relaxed);
}

void enqueue_data()
{
    data_store.emplace_back(1);
}

void consume_data()
{
    while(1){
        int temp = 0;
        if((temp = mycount.fetch_sub(1,memory_order_acquire))<=0){
            sleep(1);
            cout<<"wait more data"<<endl;
            continue;
        }
        cout<<"process data is:"<<data_store[temp-1]<<endl;
        data_store.pop_back();
    }
}



int main()
{
    thread a(queue_data);
    thread b(consume_data);
    thread c(consume_data);

    a.join();
    b.join();
    c.join();
    return 0;
}
