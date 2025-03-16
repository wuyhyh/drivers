obj-m := helloworld.o helloworld_params.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
CC = x86_64-linux-gnu-gcc-13

all default: modules
install: modules_install

modules modules_install help clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) CC=$(CC) $@
