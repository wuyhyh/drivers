KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
CC := x86_64-linux-gnu-gcc-13

# 获取所有子目录（去掉末尾的 /）
SUBDIRS := $(patsubst %/, %, $(wildcard */))

# 调试输出，检查 SUBDIRS 变量是否正确
$(info SUBDIRS = $(SUBDIRS))

all: $(SUBDIRS)

# 确保 make 进入子目录
$(SUBDIRS): FORCE
	$(MAKE) -C $@ -f Makefile all

clean:
	for dir in $(SUBDIRS); do \
	    $(MAKE) -C $$dir -f Makefile clean; \
	done

FORCE:
