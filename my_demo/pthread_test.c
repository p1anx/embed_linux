#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_function(void *arg) {
    int thread_id = (int)(long)arg; // Avoid potential cast issues
    printf("线程 %d: 开始执行\n", thread_id);
    // 模拟一些工作
    for (int i = 0; i < 5; i++) {
        printf("线程 %d: 工作中...\n", thread_id);
        sleep(1); // 暂停 1 秒
    }
    printf("线程 %d: 执行完毕\n", thread_id);
    pthread_exit((void *)(long)(thread_id * 10)); // 返回线程ID * 10
    // or, equivalently, return (void*)(long)(thread_id * 10);
}

int main() {
    pthread_t thread1, thread2;
    int thread1_id = 1, thread2_id = 2;
    void *thread1_result, *thread2_result;

    // 创建线程
    if (pthread_create(&thread1, NULL, thread_function, (void *)(long)thread1_id) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&thread2, NULL, thread_function, (void *)(long)thread2_id) != 0) {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }

    // 等待线程结束，并获取返回值
    if (pthread_join(thread1, &thread1_result) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }
    printf("主线程: 线程 1 结束, 返回值: %ld\n", (long)thread1_result);

    if (pthread_join(thread2, &thread2_result) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }
    printf("主线程: 线程 2 结束, 返回值: %ld\n", (long)thread2_result);

    printf("主线程: 程序结束\n");
    return 0;
}