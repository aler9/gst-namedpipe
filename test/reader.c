
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("/tmp/testing", O_RDONLY);
    if (fd < 0) {
        printf("unable to open named pipe\n");
        return 1;
    }

    uint8_t buf[5*1024*1024];

    while (1) {
        uint32_t size = 0;
        size_t n = read(fd, &size, sizeof(uint32_t));
        if (n != sizeof(uint32_t)) {
            break;
        }

        n = read(fd, buf, size);
        if (n != size) {
            break;
        }

        printf("received %d\n", size);
    }

    printf("exited\n");
    return 0;
}
