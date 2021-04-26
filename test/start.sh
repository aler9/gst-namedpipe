#!/bin/sh -e

do_read() {
    /reader &

    gst-launch-1.0 \
    videotestsrc \
    ! video/x-raw,width=1280,height=720,framerate=20/1 \
    ! namedpipesink location=/tmp/testing sync=1 &
    PID=$!

    sleep 5
    kill $PID
    rm /tmp/testing
}

do_write() {
    /writer &
    PID=$!

    gst-launch-1.0 -v \
    namedpipesrc location=/tmp/testing caps="video/x-raw,width=800,height=600,format=RGB" \
    ! videoconvert \
    ! autovideosink sync=1 &

    sleep 5
    kill $PID
    rm /tmp/testing
}

mkfifo /tmp/testing

do_read

wait
