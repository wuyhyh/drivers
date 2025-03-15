KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

# 获取所有子目录
SUBDIRS := $(shell find . -maxdepth 1 -type d ! -name "." ! -name "..")

# 获取每个子目录中的 .c 文件，并生成 obj-m 规则
MODULES := $(foreach dir,$(SUBDIRS),$(notdir $(wildcard $(dir)/*.c)))
MODULES := $(MODULES:.c=.o)

# 生成 obj-m 规则
obj-m := $(MODULES)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean