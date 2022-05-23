#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <utility>
#include <unistd.h>
#include <atomic>
#include <assert.h>

using namespace std;

atomic<bool> x,y;
atomic<int> z;

void write_x()
{
    x.store(true);
}

void write_y()
{
    y.store(true);
}

void read_x_then_y()
{
    while (!x.load());
    if(y.load()){
        z++;
    }
}

void read_y_then_x()
{
    while (!y.load());
    if(x.load()){
        z++;
    }
}

void write_x_then_y_assert()
{
    x.store(true,memory_order_relaxed);
    sleep(1);
    y.store(true,memory_order_relaxed);
    sleep(1);
}

void read_y_then_x_assert()
{
    while(!y.load(memory_order_relaxed));
    if(x.load(memory_order_relaxed))
        ++z;
}

int main()
{
    x = false;
    y=false;
    z=0;

    thread a(write_x);
    thread b(write_y);
    thread c(read_x_then_y);
    thread d(read_y_then_x);

    a.join();
    b.join();
    c.join();
    d.join();
    cout<<"read to assert:"<<z.load()<<endl;
    assert(z.load()!=0);
    cout<<"finish to assert"<<endl;


    x = false;
    y=false;
    z=0;

    thread e(write_x_then_y_assert);
    thread f(read_y_then_x_assert);

    e.join();
    f.join();
    cout<<"read to assert2:"<<z.load()<<endl;
    assert(z.load()!=0);
    cout<<"finish to assert2"<<endl;



    return 0;
}
