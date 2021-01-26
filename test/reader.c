
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd;
    while (1) {
        fd = open("/tmp/testing", O_RDONLY);
        if (fd < 0) {
            sleep(1);
            continue;
        }
        break;
    }

    uint8_t buf[5 * 1024*1024];

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

    return 0;
}
