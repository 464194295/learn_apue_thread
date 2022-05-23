#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <utility>
#include <unistd.h>
#include <list>

using namespace std;


template<typename T>
list<T> parrllel_quick_sort(list<T> input)
{
    if(input.empty()){
        return input;
    }

    list<T> result;
    result.splice(result.begin(),input,input.begin()); //将input转移到result中

    T const& pivot = *result.begin();

    //返回的是一个迭代器
    auto divide_point = partition(input.begin(),input.end(),[&](T const& t){return t<pivot;}); //根据lambda中定义的规则进行排序，
    


    list<T> lower_part;
    lower_part.splice(lower_part.begin(),input,input.begin(),divide_point);

    future<list<T>> new_lower(async(&parrllel_quick_sort<T>, move(lower_part)));

    auto new_heigher(parrllel_quick_sort(move(input)));


    result.splice(result.end(),new_heigher);
    result.splice(result.begin(),new_lower.get());

    return result;
}

int main()
{
    list<int> ls{2,3,4,6,7,8,1,2,3};
    list<int> temp = parrllel_quick_sort(ls);
    for(auto i : temp){
        cout<<" "<<i<<" ";
    }
    cout<<endl;
    return 0;
}
