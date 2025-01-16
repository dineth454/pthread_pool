#include "thread_pool.h"
#include <stdio.h>
#include <unistd.h>

// Task function
void print_task(int thread_id) 
{
    printf("Thread %d: Executing task\n", thread_id);
    sleep(1); // Simulate work
    printf("Thread %d: Task complete\n", thread_id);
}

int main() 
{
    // Initialize the thread pool
    thread_pool_init();

    // Add tasks to the pool
    for (int i = 0; i < MAX_TASKS; i++) 
    {
        thread_pool_add_task(print_task);
    }

    // Wait for tasks to complete
    sleep(5);

    // Destroy the thread pool
    thread_pool_destroy();

    return 0;
}
