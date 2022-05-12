#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <utility>
#include <unistd.h>

using namespace std;

void mythread1(std::promise<int> &tmpp, int calc)
{
 int result = calc * 2;
 tmpp.set_value(result);
 cout << " 线程 "<<this_thread::get_id()<<" 设置值: " << result << endl;
 return;
}
void mythread2(std::shared_future<int>& fu)
{
 cout << "线程"<<this_thread::get_id()<<" 读取值： " << fu.get() << endl;
}

int main()
{
 std::promise<int> myprom;
 std::shared_future<int> result(myprom.get_future().share());
 std::thread t1(mythread1, std::ref(myprom), 10);
 std::thread t2(mythread2, std::ref(result));
 std::thread t3(mythread2, std::ref(result));
 t1.join();
 t2.join(); 
 t3.join();


 return 0;
}
