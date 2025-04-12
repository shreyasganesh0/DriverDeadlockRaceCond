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



