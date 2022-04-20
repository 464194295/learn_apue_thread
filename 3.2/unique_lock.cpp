#include <iostream>
#include <thread>

using namespace std;

class X
{
private:
    mutex mMut;
public:
    X(int a_):a(a_){}
    ~X(){}
    void swap(X& lhs,X& lhy){
        unique_lock<mutex> lock_a(lhs.mMut,defer_lock);
        unique_lock<mutex> lock_b(lhy.mMut,defer_lock);
        lock(lock_a,lock_b);
        lhs.a = lhs.a^lhy.a;
        lhy.a = lhs.a^lhy.a;
        lhs.a = lhs.a^lhy.a;
    }

    int a;
};


shared_ptr<X> ptr;
once_flag resource_flag;
void init_X()
{
    ptr.reset(new X(2));
}

void foo()
{
    call_once(resource_flag,init_X);
    cout<<ptr->a<<endl;
}

int main()
{

    foo();
    return 0;
}
