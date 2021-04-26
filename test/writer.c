
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("/tmp/testing", O_WRONLY);
    if (fd < 0) {
        printf("unable to open named pipe\n");
        return 1;
    }

    uint8_t buf[800*600*3];

    for (size_t i = 0; i < 800*600; i++) {
        buf[i*3+0] = 255;
        buf[i*3+1] = 0;
        buf[i*3+2] = 0;
    }

    while (1) {
        uint32_t size = 800*600*3;
        size_t n = write(fd, &size, sizeof(uint32_t));
        if (n != sizeof(uint32_t)) {
            break;
        }

        n = write(fd, buf, size);
        if (n != size) {
            break;
        }

        printf("written %d\n", size);
        usleep(100000);
    }

    printf("exited\n");
    return 0;
}
