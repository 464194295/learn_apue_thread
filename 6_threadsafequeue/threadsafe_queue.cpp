#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <utility>
#include <unistd.h>

using namespace std;

//写一个线程安全的队列



template<typename T>
class threadsafe_ququq
{
private:
    struct node
    {
        shared_ptr<T> data;
        unique_ptr<node> next;
    };

    mutex head_mutex;
    unique_lock<node> head;
    mutex tail_mutex;
    node* tail;
    condition_variable data_cond;

    node* get_tail()
    {
        lock_guard<mutex> tail_lock(tail_mutex);
        return tail;
    }
    unique_ptr<node> pop_head()
    {
        unique_ptr<node> old_head = move(head);
        head=move(old_head->next);
        return old_head;
    }

public:
    threadsafe_ququq():head(new node),tail(head.get()){}
    threadsafe_ququq(const threadsafe_ququq& other) = delete;
    threadsafe_ququq& operator=(const threadsafe_ququq& other) = delete;
    
    unique_ptr<T> try_pop_head()
    {
        lock_guard<mutex> head_lock(head_mutex);
        if(head.get() == get_tail()){
            return unique_ptr<node>();
        }
        return pop_head();
    }

    bool try_pop(T& value)
    {
        unique_ptr<node> const old_head = try_pop_head();
        return old_head;
    }

    bool empty()
    {
        lock_guard<mutex> head_lock(head_mutex);
        return (head.get() == get_tail());
    }
};


int main()
{
    return 0;
}
