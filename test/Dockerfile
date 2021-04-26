####################################
FROM ubuntu:18.04 AS build

RUN apt update && apt install -y --no-install-recommends \
    gcc \
    meson \
    libgstreamer-plugins-base1.0-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /s
COPY *.c *.h meson.build ./
RUN meson --prefix=/usr build \
    && cd build \
    && ninja \
    && ninja install

####################################
FROM ubuntu:18.04

RUN apt update && apt install -y --no-install-recommends \
    gstreamer1.0-tools \
    gstreamer1.0-plugins-good \
    gstreamer1.0-plugins-bad \
    gstreamer1.0-plugins-ugly \
    gstreamer1.0-rtsp \
    gstreamer1.0-x \
    gstreamer1.0-libav \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

COPY test/reader.c /
RUN gcc -o reader reader.c

COPY --from=build /usr/lib/x86_64-linux-gnu/gstreamer-1.0/libgstnamedpipe.so \
    /usr/lib/x86_64-linux-gnu/gstreamer-1.0/libgstnamedpipe.so

COPY test/start.sh /
RUN chmod +x /start.sh

ENTRYPOINT [ "/start.sh" ]
