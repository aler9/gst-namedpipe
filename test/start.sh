#!/bin/sh -e

mkfifo /tmp/testing

/reader &

gst-launch-1.0 \
videotestsrc num-buffers=10 \
! video/x-raw,width=1280,height=720,framerate=20/1 \
! namedpipesink location=/tmp/testing sync=1 &

#echo exiting
#touch /tmp/testing
#rm /tmp/testing
wait
