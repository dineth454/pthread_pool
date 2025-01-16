#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <pthread.h>

// Maximum threads and tasks (configurable as needed)
#define MAX_THREADS 4
#define MAX_TASKS 10

// Thread pool structure
typedef struct 
{
    pthread_t threads[MAX_THREADS]; // Array of threads

    struct 
    {
        void (*function)(int); // Task function pointer
        int arg;               // Task argument (optional)
    } task_queue[MAX_TASKS];    // Task queue

    pthread_mutex_t lock;       // Mutex for synchronization
    pthread_cond_t notify;      // Condition variable

    int task_count;             // Number of tasks in the queue
    int stop;                   // Stop flag
    // int idle_count;             // TODO: Number of idle threads
} thread_pool_t;

// Functions
void thread_pool_init();
void thread_pool_add_task(void (*function)(int));
void thread_pool_destroy();

#endif // THREAD_POOL_H
