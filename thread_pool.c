#include "thread_pool.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Thread pool global variable
static thread_pool_t pool;

// Worker thread function
void *thread_worker(void *arg) 
{
    int thread_id = *(int *)arg; // Thread ID
    free(arg); // Avoid memory leak

    while (1) 
    {
        pthread_mutex_lock(&pool.lock);

        // Wait for tasks or stop signal
        while (pool.task_count == 0 && !pool.stop) 
        {
            pthread_cond_wait(&pool.notify, &pool.lock);
        }

        if (pool.stop) 
        {
            pthread_mutex_unlock(&pool.lock);
            break;
        }

        // Get the next task
        void (*task_function)(int) = pool.task_queue[--pool.task_count].function;
        pthread_mutex_unlock(&pool.lock);

        // Execute the task
        task_function(thread_id);
    }

    return NULL;
}

// Initialize the thread pool
void thread_pool_init() 
{
    pthread_mutex_init(&pool.lock, NULL);
    pthread_cond_init(&pool.notify, NULL);
    pool.task_count = 0;
    pool.stop = 0;

    for (int i = 0; i < MAX_THREADS; i++) 
    {
        int *id = malloc(sizeof(int)); // Allocate memory for thread ID
        *id = i + 1;                  // Thread IDs start at 1
        pthread_create(&pool.threads[i], NULL, thread_worker, id);
    }
}

// Add a task to the pool
void thread_pool_add_task(void (*function)(int)) 
{
    pthread_mutex_lock(&pool.lock);

    if (pool.task_count < MAX_TASKS) 
    {
        pool.task_queue[pool.task_count].function = function;
        pool.task_count++;
        pthread_cond_signal(&pool.notify);
    } 
    else 
    {
        printf("Task queue is full. Task not added.\n");
    }

    pthread_mutex_unlock(&pool.lock);
}

// Destroy the thread pool
void thread_pool_destroy() 
{
    pthread_mutex_lock(&pool.lock);
    pool.stop = 1;
    pthread_cond_broadcast(&pool.notify);
    pthread_mutex_unlock(&pool.lock);

    for (int i = 0; i < MAX_THREADS; i++) 
    {
        pthread_join(pool.threads[i], NULL);
    }

    pthread_mutex_destroy(&pool.lock);
    pthread_cond_destroy(&pool.notify);
}
