# 生成 hello.ko
obj-m := hello.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
CC := x86_64-linux-gnu-gcc-13

all:
	$(MAKE) -C $(KDIR) M=$(PWD) CC=$(CC) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
