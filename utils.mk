
UBUNTU_BASE_IMAGE := ubuntu:20.04

.PHONY: $(shell ls)

help:
	@echo "usage: make [action]"
	@echo ""
	@echo "available actions:"
	@echo ""
	@echo "  indent"
	@echo "  test"
	@echo ""

indent:
	# https://gstreamer.freedesktop.org/documentation/frequently-asked-questions/developing.html?gi-language=c#what-is-the-coding-style-for-gstreamer-code
	docker run --rm -it -v $(PWD):/s $(UBUNTU_BASE_IMAGE) sh -c \
	"apt update && apt install -y indent \
	&& echo ok \
	&& cd /s && indent \
	--braces-on-if-line \
	--case-brace-indentation0 \
	--case-indentation2 \
	--braces-after-struct-decl-line \
	--line-length80 \
	--no-tabs \
	--cuddle-else \
	--dont-line-up-parentheses \
	--continuation-indentation4 \
	--honour-newlines \
	--tab-size8 \
	--indent-level2 \
	*.c *.h \
	&& rm *~"

test:
	cd test && make
