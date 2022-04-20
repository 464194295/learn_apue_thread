#include <iostream>
#include <thread>
#include <vector>

using namespace std;


class joining_thread
{
private:
    thread t;
public:
    joining_thread() noexcept=default;
    //template<typename Callable, typename ... Args>
    //explicit joining_thread(Callable&& func, Args&& ... args):t(forward<Callable>(func),forward<Args>(args)...){}

    explicit joining_thread(thread t_) noexcept:t(move(t_)){}

    joining_thread(joining_thread&& other) noexcept:t(move(other.t)){}

    joining_thread& operator=(joining_thread&& other) noexcept{
        if(joinable()){
            join();
        }
        t = move(other.t);
        return *this;
    }

    ~joining_thread() noexcept{
        if(joinable()){
            join();
        }
    }
    void swap(joining_thread& other)noexcept{
        t.swap(other.t);
    }

    thread::id get_id() const noexcept{
        return t.get_id();
    }

    bool joinable(){
        return t.joinable();
    }

    void join(){
        t.join();
    }

    void detach(){
        t.detach();
    }

    thread& as_thread() noexcept
    {
        return t;
    }

    const thread& as_thread() const noexcept
    {
        return t;
    }
};

void do_something(int i)
{
    std::cout<<"i is "<<i<<std::endl;
}


int main()
{
    std::vector<std::thread> th;
    for(int i = 0;i<20;i++){
        th.emplace_back(do_something,i);
    }

    for(auto& entry : th){
        entry.join();
    }
    return 0;
}


