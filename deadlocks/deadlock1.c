#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define DEV_PATH "/dev/a6"
#define CDRV_IOC_MAGIC 'Z'
#define E2_IOCMODE1 _IO(CDRV_IOC_MAGIC, 1)
#define E2_IOCMODE2 _IO(CDRV_IOC_MAGIC, 2)

void *open_func(void *arg) {

    printf("Starting open in %d thread\n", *(int *)arg);
    int fd = open(DEV_PATH, O_RDWR);
    printf("Finished open in %d thread\n", *(int *)arg);

    if (fd == -1) {

        printf("Failed to open file\n");
    }

    printf("Doing work\n");
    pause();// thread 1 will block
    printf("Done work\n");

    printf("Closing fd in thread: %d\n", *(int *)arg);
    close(fd);
    printf("Closed fd in thread: %d\n", *(int *)arg);
}



int main(int argc, char *argv[]) {

    pthread_t threads[2];

    int t1 = 1;
    int t2 = 2;
    
    pthread_create(&threads[0], NULL, open_func, &t1);
    pthread_create(&threads[1], NULL, open_func, &t2);

    pthread_join(threads[0], NULL);
    printf("Joined thread 1\n");
    pthread_join(threads[1], NULL);
    printf("Joined thread 2\n");

    printf("Exited program\n");
    return 0;
}
