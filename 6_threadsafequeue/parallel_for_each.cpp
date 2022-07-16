#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <utility>
#include <unistd.h>
#include <vector>

using namespace std;

template<typename Iterator,typename Func>
void parallel_for_each(Iterator first,Iterator last ,Func f)
{
    unsigned long const length = distance(first,last);

    if(!length){
        return ;
    }

    unsigned long const min_per_thread = 5;

    if(length<(2*min_per_thread)){
        for_each(first,last,f);
    }else{
        Iterator const mid_point = first+length/2;

        future<void> first_half = async(&parallel_for_each<Iterator,Func>,first,mid_point,f);

        parallel_for_each(mid_point,last,f);
        first_half.get();
    }


}

int main()
{
    vector<int> temp;
    for(int i =0;i<100;i++){
        temp.emplace_back(i);
    }
    mutex mut;
    auto f = [&](int i){
        lock_guard<mutex> lk(mut);
        cout<<i<<endl;
        return;
    };

    parallel_for_each(temp.begin(),temp.end(),f);
    return 0;
}
