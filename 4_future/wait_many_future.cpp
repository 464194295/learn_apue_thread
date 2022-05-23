

//程序用于等待所有的future都处理结束才进行结果的返回，使用when_all的语法
//由于在mac上对头文件暂时不支持，所以暂时只是看了相关内容，对when_all、锁存器以及Barriers暂时没有编写程序


#include <iostream>
#include <atomic>
#include <vector>
#include <future>
#include <unistd.h>

class MyClass {
 public:
  int func2(int x, int y) {
    return x + y;
  }
  int func1(int x,int y){
      sleep(1);
      return x-y;
  }
};
using namespace std;

void process_data(vector<int> &data)
{
    vector<future<int>> results;

}

int main() {
  //计算（a+b）/(x+y)
  //用三个线程，一个线程计算a+b，另一个线程计算x+y
  int a, b, x, y;
  a = 10, b = 8, x = 2, y = 4;

  MyClass my_class;

  int sum1, sum2;
  //模板传参的时候使用ref，否则传参失败
  vector<future<int>> futrues;

  auto future1 = std::async(std::launch::async, &MyClass::func2, my_class, x, y);
  futrues.emplace_back(std::move(future1));

  auto future2 = std::async(std::launch::async, &MyClass::func1, my_class, a, b);
  futrues.emplace_back(std::move(future2));


  //获取值
  for (int i = 0; i < futrues.size(); i++) {
    std::cout << "sum1=" << futrues[i].get() << std::endl;

  }
  return 0;
}