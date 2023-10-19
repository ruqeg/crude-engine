CFLAGS = -std=c++20 -O2
LDFLAGS = -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

INC = -I/$(ROOT_DIR)include/

SOURCEDIR = $(ROOT_DIR)test/
SOURCESLIST = $(SOURCEDIR)main.cpp

LIBDIR = $(ROOT_DIR)include/crude_vulkan_01/
LIBSOURCELIST = $(LIBDIR)instance.cpp $(LIBDIR)application.cpp

OUTPUTNAME = test_vulkan

.PHONY: test clean

test_vulkan: $(SOURCESLIST) $(LIBSOURCELIST)
	g++ $(CFLAGS) $(INC) -o $(OUTPUTNAME) $(SOURCESLIST) $(LIBSOURCELIST) $(LDFLAGS)

test: $(OUTPUTNAME)
	./$(OUTPUTNAME)

clean:
	rm -f $(OUTPUTNAME)
