#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <list>

using namespace std;

list<int> some_list;
mutex some_mutex;


void add_to_list(int new_valuw)
{
    lock_guard<mutex> guard(some_mutex);
    some_list.emplace_back(new_valuw);
}

bool list_content(int value_to_find)
{
    lock_guard<mutex> guard(some_mutex);
    if(find(some_list.begin(),some_list.end(),value_to_find)!=some_list.end()){
        cout<<"find number,number size is "<<some_list.size()<<endl;
    }else{
        cout<<"not find"<<endl;
    }
}

void initlist()
{
    for(int i=0;i<100;i++){
        add_to_list(i);
    }
}

class myclass
{
public:
    myclass() = default;
    ~myclass(){}
    
    mutex mu;
};

void swap(myclass& lh,myclass& ls)
{
    if(&lh == &ls){
        return;
    }
    scoped_lock<mutex,mutex> guard(lh.mu,ls.mu);
    //do_something()
    return;
}

int main()
{
    thread t1(initlist);
    thread t2(list_content,90);

    t1.join();
    t2.join();
    return 0;
}
