#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/ioctl.h>

#define DEV_PATH "/dev/a6"
#define CDRV_IOC_MAGIC 'Z'
#define E2_IOCMODE1 _IO(CDRV_IOC_MAGIC, 1)
#define E2_IOCMODE2 _IO(CDRV_IOC_MAGIC, 2)

void *ioctl_func(void *arg) {

    printf("Starting open in %d thread\n", *(int *)arg);
    int fd = open(DEV_PATH, O_RDWR);
    printf("Finished open in %d thread\n", *(int *)arg);

    if (fd == -1) {

        printf("Failed to open file\n");
    }

    printf("IOCTL switching started\n");
    int ioctl_err = ioctl(fd, E2_IOCMODE2);

    if (ioctl_err == -1) {

        printf("Failed to switch mode\n");
    }
    printf("IOCTL mode 2 switching ended\n");

    pause();// simulate work
    
    printf("Closing fd in thread: %d\n", *(int *)arg);
    close(fd);
    printf("Closed fd in thread: %d\n", *(int *)arg);
    return NULL;
}

void *ioctl_switch_func(void *arg) {

    printf("Starting open in %d thread\n", *(int *)arg);
    int fd = open(DEV_PATH, O_RDWR);
    printf("Finished open in %d thread\n", *(int *)arg);

    if (fd == -1) {

        printf("Failed to open file\n");
    }

    printf("IOCTL switching started\n");
    int ioctl_err = ioctl(fd, E2_IOCMODE2);

    if (ioctl_err == -1) {

        printf("Failed to switch mode\n");
    }
    printf("IOCTL mode 2 switching ended\n");

    sleep(2);

    printf("IOCTL back to 1 switching started\n");
    ioctl_err = ioctl(fd, E2_IOCMODE1);

    if (ioctl_err == -1) {

        printf("Failed to switch mode\n");
    }
    printf("IOCTL mode 1 switching ended\n");

    printf("Closing fd in thread: %d\n", *(int *)arg);
    close(fd);
    printf("Closed fd in thread: %d\n", *(int *)arg);

    return NULL;
}

int main(int argc, char *argv[]) {

    pthread_t threads[3];

    int t1 = 1;
    int t2 = 2;

    pthread_create(&threads[1], NULL, ioctl_switch_func, &t2);
    sleep(1);
    pthread_create(&threads[0], NULL, ioctl_func, &t1);

    pthread_join(threads[0], NULL);
    pthread_join(threads[1], NULL);

    printf("All threads compeleted\n");

    return 0;
}



