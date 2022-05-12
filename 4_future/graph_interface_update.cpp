#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <utility>
#include <deque>
#include <unistd.h>

using namespace std;

//一、被包装的任务(stored task)，任务(task)是一个可调用的对象，如函数指针、成员函数指针或者函数对象。
//二、共享状态(shared state)，用于保存任务的返回值，可以通过 std::future 对象来达到异步访问共享状态的效果。


std::mutex m;
std::deque<std::packaged_task<int()> > tasks;
bool gui_shutdown_message_received() {/*……*/return false; };
void get_and_process_gui_message() {/*……*/};

void gui_thread() // 1
{
 cout << "gui thread id = " << std::this_thread::get_id() << endl;
 while (!gui_shutdown_message_received()) // 2
 {
  get_and_process_gui_message(); // 3
  std::packaged_task<int()> task;
  {
   std::lock_guard<std::mutex> lk(m);
   if (tasks.empty()) // 4
    continue;
   task = std::move(tasks.front()); // 5
   tasks.pop_front();  
  }
  task();
   // 6
 }
}
template<typename Func>
std::future<int> post_task_for_gui_thread(Func f)
{
 std::packaged_task<int()> task(f); // 7
 std::future<int> res = task.get_future(); // 8
 std::lock_guard<std::mutex> lk(m); // 9
 tasks.push_back(std::move(task)); // 10
 return res;
}

int Fun1() { cout << "Fun1执行在线程=" << this_thread::get_id() << endl; return 1; }
int Fun2() { cout << "Fun2执行在线程=" << this_thread::get_id() << endl; return 2; }

int main()
{
 cout << "主线程id= " << this_thread::get_id() << endl;
 std::thread gui_bg_thread(gui_thread);
 gui_bg_thread.detach();
 
 auto result1 = post_task_for_gui_thread<int()>(Fun1);
 auto result2 = post_task_for_gui_thread<int()>(Fun2);
 sleep(1);
 cout << "result1的结果: " << result1.get() << endl;
 cout << "result2的结果: " << result2.get() << endl;
 system("pause");
 return 0;
}