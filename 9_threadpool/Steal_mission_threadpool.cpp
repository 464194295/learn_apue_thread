#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <utility>
#include <unistd.h>
#include <deque>

using namespace std;

//这是一个可以窃取任务的队列，当其他线程中没有任务的时候，可以从其他的任务队列中获取到任务，就需要本地的任务列表可以被其他线程访问。
//需要队列中的任务被同步和保护

class function_wrapper
{
    //...
};

class work_stealing_queue
{
    private:
        typedef function_wrapper data_type;
        deque<data_type> the_queue;
        mutable mutex the_mutex;   //表示锁是一个变量，如果一个函数用const进行修饰，而使用lock_ground会改变锁，所以需要使用mutable进行修饰

    public:
        work_stealing_queue() = default;
        work_stealing_queue(const work_stealing_queue& other) = delete;

        void push(data_type){
            lock_guard<mutex> lock(the_mutex);
            the_queue.push_front(move(data));
        }

        bool empty() const
        {
            lock_guard<mutex> lock(the_mutex);
            return the_queue.empty();
        }

        bool try_pop(data_type& res)
        {
            lock_guard<mutex> lock(the_mutex);
            if(the_queue.empty()){
                return false;
            }

            res = move(the_queue.front());
            the_queue.pop_front();
            return true;
        }

        //这个函数用于从别的线程获取任务
        bool try_steal(data_type& res)
        {
            lock_guard<mutex> lock(the_mutex);
            if(the_queue.empty()){
                return false;
            }
            
            res = move(the_queue.front());//对队列后端进行操作
            the_queue.pop_front();
            return true;

        }

};



//使用这个偷窃任务队列，个人理解相当于每个线程都有一个可以进行偷窃的任务队列
class thread_pool
{
    typedef function_wrapper task_type;
    atomic_bool done;
    thread_safe_queue<task_type> pool_work_queue;
    vector<unique_ptr<work_stealing_queue>> queues;
    vector<thread> threads;
    join_thread joiner;

    static thread_local work_stealing_queue* local_work_queue;  //具有线程生命周期
    static thread_local unsigned my_index;

    void worker_thread(unsigned my_index_){
        my_index = my_index_;
        local_work_queue = queues[my_index].get();
        while(!done){
            run_pending_task();
        }
    }

    bool pop_task_from_local_queue(task_type& task){
        return local_work_queue && local_work_queue->try_pop(task);
    }

    bool pop_task_from_pool_queue(task_type& task){
        return pool_work_queue.try_pop(task);
    }

    bool pop_task_from_other_thread_queue(task_type& task){
        for(unsigned i=0;i<queues.size();i++){
            unsigned const index = (my_index+i+1) % queues.size();
            if(queues[index]->try_steal(task)){
                return true;
            }
        }
        return false;
    }

    public:
        thread_pool():done(false),joiner(threads){
            unsigned const thread_count = thread::hardware_concurrency();

            try
            {
                for(unsigned i = 0;i<thread_count;i++){
                    queues.push_back(unique_ptr<work_stealing_queue>(new work_stealing_queue));
                }

                threads.push_back(thread(&thread_pool::worker_thread,this,i));
            }
            catch(...){
                done = true;
                throw;
            }
        }

        ~thread_pool(){
            done = true;
        }

        template<typename FunctionType>
        future<typename result_of<FunctionType()>::type>> submit(FunctionType f){
            typedef typename result_of<FunctionType()>::type result_type;
            future<result_type> res(task.get_fuure());
            if(local_work_queue){
                local_work_queue->push(move(task));
            }else{
                pool_work_queue.push(move(task));
            }
            return res;
        }

        void run_pending_task(){
            task_type task;
            if(pop_task_from_local_queue(task) || pop_task_from_pool_queue(task) || pop_task_from_other_thread_queue(task)){
                task();
            }else{
                this_thread::yield();
            }
        }

}


int main()
{
    
    return 0;
}
