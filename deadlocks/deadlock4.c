#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <signal.h>
#include <sys/ioctl.h>

#define DEV_FILE "/dev/a6"
#define CDRV_IOC_MAGIC 'Z'
#define E2_IOCMODE1 _IO(CDRV_IOC_MAGIC, 1)
#define E2_IOCMODE2 _IO(CDRV_IOC_MAGIC, 2)

void *read_thread_func(void *arg) {
    int fd = *((int*)arg);
    char buffer[128];
    ssize_t ret;

    printf("Thread READ: Starting read on device...\n");
    ret = read(fd, buffer, sizeof(buffer));
    if (ret < 0) {
        perror("read");
    } else {
        printf("Thread READ: Read %zd bytes\n", ret);
    }

    return NULL;
}

void *ioctl_thread_func(void *arg) {
    int fd = *((int*)arg);

    sleep(1);
    printf("Thread IOCTL: Calling ioctl to switch mode...\n");

    int ret = ioctl(fd, E2_IOCMODE1);
    if (ret < 0) {
        perror("ioctl");
    } else {
        printf("Thread IOCTL: ioctl completed\n");
    }
    return NULL;
}

int main() {
    int fd_read, fd_ioctl;
    pthread_t tid_read, tid_ioctl;

    fd_read = open(DEV_FILE, O_RDONLY);
    if (fd_read < 0) {
        perror("open for read");
        exit(EXIT_FAILURE);
    }
    
    fd_ioctl = open(DEV_FILE, O_RDONLY);
    if (fd_ioctl < 0) {
        perror("open for ioctl");
        close(fd_read);
        exit(EXIT_FAILURE);
    }
    
    printf("Main: Device opened; launching threads...\n");

    if (pthread_create(&tid_read, NULL, read_thread_func, &fd_read) != 0) {
        perror("pthread_create for read thread");
        close(fd_read);
        close(fd_ioctl);
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&tid_ioctl, NULL, ioctl_thread_func, &fd_ioctl) != 0) {
        perror("pthread_create for ioctl thread");
        close(fd_read);
        close(fd_ioctl);
        exit(EXIT_FAILURE);
    }

    pthread_join(tid_read, NULL);
    pthread_join(tid_ioctl, NULL);

    printf("Main: Threads completed. If this message prints, the expected deadlock did not occur.\n");

    close(fd_read);
    close(fd_ioctl);
    return 0;
}

