#!/bin/sh -e

/reader &

gst-launch-1.0 \
videotestsrc \
! video/x-raw,width=640,height=320,framerate=20/1 \
! namedpipesink location=/tmp/testing sync=1