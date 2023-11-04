CFLAGS = -std=c++20 -O2
LDFLAGS = -lvulkan  -lxcb -ldl -lpthread # -lX11 -lXxf86vm -lXrandr -lXi

ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

INC = -I/$(ROOT_DIR)include/

SOURCEDIR = $(ROOT_DIR)test/
SOURCESLIST = $(SOURCEDIR)main.cpp

LIBDIR = $(ROOT_DIR)include/crude_vulkan_01/
LIBSOURCELIST = $(LIBDIR)color_blend_state_create_info.cpp $(LIBDIR)depth_stencil_state_create_info.cpp $(LIBDIR)multisample_state_create_info.cpp $(LIBDIR)rasterization_state_create_info.cpp $(LIBDIR)viewport_state_create_info.cpp $(LIBDIR)input_assembly_state_create_info.cpp $(LIBDIR)vertex_input_state_create_info.cpp $(LIBDIR)shader_module.cpp $(LIBDIR)descriptor_set_layout.cpp $(LIBDIR)render_pass.cpp $(LIBDIR)image_view.cpp $(LIBDIR)image.cpp $(LIBDIR)swap_chain_image.cpp $(LIBDIR)swap_chain.cpp $(LIBDIR)device.cpp $(LIBDIR)queue.cpp $(LIBDIR)physical_device.cpp $(LIBDIR)surface.cpp $(LIBDIR)debug_utils_messenger.cpp $(LIBDIR)instance.cpp $(LIBDIR)application.cpp

OUTPUTNAME = test_vulkan

.PHONY: test clean

test_vulkan: $(SOURCESLIST) $(LIBSOURCELIST)
	g++ $(CFLAGS) $(INC) -o $(OUTPUTNAME) $(LIBSOURCELIST) $(SOURCESLIST) $(LDFLAGS)

test: $(OUTPUTNAME)
	./$(OUTPUTNAME)

clean:
	rm -f $(OUTPUTNAME)
