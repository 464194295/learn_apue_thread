#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <utility>
#include <unistd.h>

using namespace std;



class Fpp
{
public:
    Fpp() = default;
    ~Fpp() = default;
};

int main()
{
    //原子指针类型
    Fpp array[5];
    atomic<Fpp*> p(array);

    Fpp* x=p.fetch_add(2);
    assert(x == array);   //assert在传入为false的时候才会触发
    cout<<"ready to assert"<<endl;
    assert(p.load() == &array[2]);
    cout<<"not assert"<<endl;
    return 0;
}
