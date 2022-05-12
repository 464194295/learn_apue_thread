#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <utility>
#include <unistd.h>

using namespace std;


//std::promise是一个类模板，可以在某一个线程中给他赋值，然后从另一个线程中将其取出来使用。这样可以实现多线程之间的通信。
//与packaged_task一样，promise不支持拷贝构造，但支持移动构造。成员函数get_future()返回其绑定的future对象。
//成员函数set_value()对其中包含的对象进行赋值。


void mythread1(std::promise<int> &tmpp, int calc)
{
    int result = calc * 2;
    tmpp.set_value(result);
    cout << " 线程 mythread1 设置值: " << result << endl;
    return;
}

void mythread2(std::future<int>& fu)
{
 cout << "线程 mythread2 读取值： " << fu.get() << endl;
}

int main()
{
 std::promise<int> myprom;
 std::future<int> result = myprom.get_future();
 std::thread t1(mythread1, std::ref(myprom), 10);
 std::thread t2(mythread2, std::ref(result));


 t1.join();
 t2.join(); 
 system("pause");
}
