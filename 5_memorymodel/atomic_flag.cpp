#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <utility>
#include <unistd.h>

using namespace std;

//在首次使用的时候，要进行初始化
atomic_flag ato = ATOMIC_FLAG_INIT;
atomic<bool> b(true);


//实现自旋锁
class spinlock
{
    private:
        atomic_flag att;
    public:
        spinlock():att(ATOMIC_FLAG_INIT){} //初始化的时候标志是清除的，处于解锁的状态
        void lock(){
            while(att.test_and_set());    //第一次是可以通过的，因为要获取这把锁呀，是合理的
        }
        void unlock(){
            att.clear();
        }
};

int main()
{
    //当对atomic_flag进行初始化之后，只可以做销毁、清除和设置操作
    //test_and_set 先对当前atomic_flag的值进行检测并返回，在将当前的值设置为true
    ato.clear();
    bool x =ato.test_and_set();
    if(x){
        cout<<"1"<<endl;
    }else{
        cout<<"0"<<endl;
    }

    b = false;


    return 0;
}
