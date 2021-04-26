
# gst-namedpipe

GStreamer elements that allow to read/write incoming data to/from named pipes:

* `namedpipesrc location=/path/to/namedpipe`
* `namedpipesink location=/path/to/namedpipe`

These have a couple of advantages with respect to `udpsrc` and `udpsink`:

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

1. Create a named pipe

   ```
   mkfifo /tmp/testing
   ```

2. Write a reader, for instance this is a reader written in C

   ```c
   #include <stdlib.h>
   #include <stdint.h>
   #include <fcntl.h>
   #include <unistd.h>

   int main() {
       int fd = open("/tmp/testing", O_RDWR);
       if (fd < 0) {
           printf("unable to open named pipe\n");
           return 1;
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

3. Launch a pipeline

   ```
   gst-launch-1.0 \
   videotestsrc \
   ! video/x-raw,width=640,height=320,framerate=20/1 \
   ! namedpipesink location=/tmp/testing sync=1
   ```

   Data is now available to the reader.
