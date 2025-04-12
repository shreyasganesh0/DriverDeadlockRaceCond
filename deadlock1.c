#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <pthread.h>

#include DEV_PATH "/dev/a6"
void open_func(void *arg) {

    printf("Starting open in %d thread\n", *(int *)arg);
    int fd = open(DEV_PATH, O_RDWR);
    printf("Finished open in %d thread\n", *(int *)arg);

    if (fd == -1) {

        printf("Failed to open file\n");
    }

    printf("Closing fd in thread: %d\n", *(int *)arg);
    close(fd);
    printf("Closed fd in thread: %d\n", *(int *)arg);
}

void open_func2(void *arg) {

    printf("Starting open in %d thread\n", *(int *)arg);
    int fd = open(DEV_PATH, O_RDWR);
    printf("Finished open in %d thread\n", *(int *)arg);

    if (fd == -1) {

        printf("Failed to open file\n");
    }

    int ioctl_err = ioctl(fd, E2_IOCMODE2);

    if (ioctl_err == -1) {

        printf("Failed to switch mode\n");
    }

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
