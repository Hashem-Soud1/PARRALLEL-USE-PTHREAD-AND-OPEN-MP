#include <stdio.h>
#include <pthread.h>

#define MAX_NUMBER 20
#define THREAD_COUNT 4

int current = 1;
pthread_mutex_t lock;
pthread_cond_t cond;

void* print_numbers(void* arg) {
    int tid = *(int*)arg;

    while (1) {
        pthread_mutex_lock(&lock);

        while (current <= MAX_NUMBER && current % THREAD_COUNT != tid)
            pthread_cond_wait(&cond, &lock);

        if (current > MAX_NUMBER) {
            pthread_mutex_unlock(&lock);
            pthread_cond_broadcast(&cond); 
            break;
        }

        printf("Thread %d printed: %d\n", tid, current);
        current++;

        pthread_cond_broadcast(&cond); 
        pthread_mutex_unlock(&lock);
    }

    return NULL;
}

int main() {
    pthread_t threads[THREAD_COUNT];
    int ids[THREAD_COUNT];

    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&cond, NULL);

    for (int i = 0; i < THREAD_COUNT; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, print_numbers, &ids[i]);
    }

    for (int i = 0; i < THREAD_COUNT; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}

