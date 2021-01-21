
# gst-namedpipesink

GStreamer element that allows to write incoming data to fifo / named pipes; these have a couple of advantages with respect to udp and `udpsink`:

* named pipes are more efficient than UDP
* frame size is unlimited, while in UDP there's a limit of 65536 bytes

Frames are prefixed with their size, in order to allow readers to correctly split them.

## Installation

Install build dependencies:
```
sudo apt install -y --no-install-recommends \
gcc \
meson \
libgstreamer-plugins-base1.0-dev
```

Compile and install:
```
meson --prefix=/usr build \
&& cd build \
&& ninja \
&& sudo ninja install
```

## Usage

Write a reader, for instance this is a reader written in C

```c
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>

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

    uint8_t buf[1024*1024];

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
```

Compile and launch the reader

```
gcc -o reader reader.c
/reader &
```

Launch a pipeline

```
gst-launch-1.0 \
videotestsrc \
! video/x-raw,width=640,height=320,framerate=20/1 \
! namedpipesink location=/tmp/testing sync=1
```

Data is now available to the reader.
