#include <iostream>
#include <thread>
#include <future>
#include <mutex>
#include <utility>
#include <unistd.h>

using namespace std;


thread task_thread;
atomic<bool> task_cancelled(false);

void gui_thread()
{
    while(1){
        event_data event = get_event();
        if(event == quit){
            break;
        }
        process(event);
    }
}

void task()
{
    while(!task_complete() && !task_cancelled){
        do_next_operation();
    }
    if(task_cancelled){
        perform_cleanup();
    }else{
        post_gui_enent(task_complete);
    }
    
}

void process(event_data const &event)
{
    switch(event.type)
    {
        case start_case:
            task_cancelled = false;
            task_thread = thread(task);
        case stop_case:
            task_cancelled = true;
            task_thread.join();
        case task_complete:
            task_thread.join();
            display_result();
            break;
        default:
            //

            
    }
}




int main()
{
    
    return 0;
}
