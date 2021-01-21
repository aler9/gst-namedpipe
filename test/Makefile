
all:
	docker build ../ -f Dockerfile -t temp

	xhost +local:root
	docker run --rm -it \
	-e DISPLAY \
	-v /tmp/.X11-unix:/tmp/.X11-unix:ro \
	--network=host \
	temp
