#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#define BUFSIZ  5

int buf[BUFSIZ] = { 0 };
int cnt = 0;
pthread_mutex_t mutex;

// 没能理解为什么要使用信号量，这里拿互斥锁就能解决问题了吧
int insert_item(int item){
    pthread_mutex_lock(&mutex);
    if (cnt == BUFSIZ) {
        pthread_mutex_unlock(&mutex);
        return 1;
    }
    buf[cnt] = item;
    cnt++;
    pthread_mutex_unlock(&mutex);
    return 0;
}

int remove_item(){
    pthread_mutex_lock(&mutex);
    if (cnt == 0) {
        pthread_mutex_unlock(&mutex);
        return 1;
    }
    printf("Get %d from buffer.\n", buf[--cnt]);
    pthread_mutex_unlock(&mutex);
    return 0;
}

_Noreturn void* producer(){
    srand((unsigned)time(NULL));
    while (1){
        sleep(rand() % 3);
        int item = rand() % 32768;
        if (insert_item(item)){
            printf("Buffer is full, unable to add new numbers to buffer.\n");
        }
        else{
            printf("Insert %d into buffer.\n", item);
        }
    }
}

_Noreturn void* consumer(){
    srand((unsigned)time(NULL));
    while (1){
        sleep(rand() % 3);
        if (remove_item()){
            printf("Buffer is empty, unable to read numbers from buffer.\n");
        }
    }
}

int main(){
    pthread_mutex_init(&mutex, NULL);
    pthread_t pid1, pid2;
    pthread_create(&pid1, NULL, consumer, NULL);
    pthread_create(&pid2, NULL, producer, NULL);
    pthread_join(pid1, NULL);
    pthread_join(pid2, NULL);
    return 0;
}
