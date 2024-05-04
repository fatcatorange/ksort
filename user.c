#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define KSORT_DEV "/dev/sort"

int main(int argc, char *argv[])
{
    int fd = open(KSORT_DEV, O_RDWR);
    if (fd < 0) {
        perror("Failed to open character device");
        goto error;
    }

    if (argc != 2) {
        goto error;
    }

    printf("%s\n", argv[1]);
    ssize_t r_sz = write(fd, argv[1], strlen(argv[1]));

    size_t n_elements = 1000;
    size_t size = n_elements * sizeof(int);
    int *inbuf = malloc(size);
    if (!inbuf)
        goto error;

    for (size_t i = 0; i < n_elements; i++)
        inbuf[i] = rand() % n_elements;

    r_sz = read(fd, inbuf, size);
    if (r_sz != size) {
        perror("Failed to write character device");
        goto error;
    }

    bool pass = true;
    int ret = 0;
    /* Verify the result of sorting */
    for (size_t i = 1; i < n_elements; i++) {
        if (inbuf[i] < inbuf[i - 1]) {
            pass = false;
            break;
        }
    }

    printf("Sorting %s!\n", pass ? "succeeded" : "failed");

error:
    free(inbuf);
    if (fd > 0)
        close(fd);
    return ret;
}
