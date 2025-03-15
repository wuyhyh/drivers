# drivers
linux drivers demo

如果你计划在 `driver/` 目录下不断添加新的子目录和代码，你可以优化 `Makefile` 使其更加**通用**，自动适应新的子目录，而不需要每次手动修改 `Makefile`。

---

## **1. 目录结构**
```
driver/
├── Makefile
├── hello/
│   ├── Makefile
│   ├── hello.c
├── workqueue/
│   ├── Makefile
│   ├── workqueue.c
├── newmodule/
│   ├── Makefile
│   ├── newmodule.c
```
未来可以随时添加新的模块目录，如 `newmodule/`，而无需手动修改 `Makefile`。

---

## **2. **自动查找所有子目录的 `Makefile`（driver/Makefile）**
你可以写一个 **自动遍历子目录** 的 `Makefile`，这样任何新增的子目录都会被自动编译：
```make
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

# 自动查找所有包含 Makefile 的子目录
SUBDIRS := $(shell find . -maxdepth 1 -type d ! -name "." ! -name "..")

all: $(SUBDIRS)

# 遍历所有子目录，并调用它们的 Makefile 进行编译
$(SUBDIRS):
	$(MAKE) -C $(KDIR) M=$(PWD)/$@ modules

clean:
	for dir in $(SUBDIRS); do $(MAKE) -C $(KDIR) M=$(PWD)/$$dir clean; done
```

### **🔹 如何扩展**
- 你只需 **创建新的目录** 并在其中放置 `Makefile` 和 `.c` 代码，无需修改 `driver/Makefile`。
- `SUBDIRS` **会自动检测所有子目录**（例如 `hello/`、`workqueue/`、`newmodule/`）。
- `make` **会递归遍历每个子目录**，调用 `Makefile` 进行编译。

---

## **3. 子目录 `Makefile` 示例**
你可以在每个子目录（如 `hello/`、`workqueue/`）中使用相同的 `Makefile`，这样你不需要为每个模块单独写不同的 `Makefile`。

### **示例：hello/Makefile**
```make
# 只编译 hello.o 为 hello.ko
obj-m := hello.o

KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
```
其他模块，如 `workqueue/`、`newmodule/` 目录的 `Makefile` 结构相同，只需修改 `obj-m` 变量的值。

---

## **4. 编译 & 清理**
### **编译所有模块**
```bash
make
```
所有子目录中的 `.ko` 模块都会被自动编译。

### **清理所有编译文件**
```bash
make clean
```
会自动清理 `hello/`、`workqueue/`、`newmodule/` 目录中的 `.o` 和 `.ko` 文件。

---

## **5. 未来如何扩展**
如果以后你想添加更多的模块：
1. **创建新目录**（如 `newmodule/`）。
2. **在该目录中添加 `newmodule.c` 和 `Makefile`**（可以复制 `hello/Makefile`）。
3. **运行 `make`**，新模块会被自动编译，无需修改 `driver/Makefile`。

---

## **6. 终极优化：让 `driver/Makefile` 自动检测模块名**
如果你想简化子目录 `Makefile`，可以让 `driver/Makefile` 直接管理子目录：
```make
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
```
### **🚀 这样做的好处**
- **完全不需要子目录 `Makefile`**，你只需放入 `.c` 代码。
- 你添加 `.c` 文件后，`Makefile` **会自动检测并编译**，比如：
    - `hello.c → hello.ko`
    - `workqueue.c → workqueue.ko`
    - `newmodule.c → newmodule.ko`

---

## **7. 总结**
| 方式 | 适用场景 | 适合项目 |
|------|---------|---------|
| **递归调用子目录 `Makefile`** | 每个子目录有独立 `Makefile`，适合复杂项目 | **模块较少，手动管理** |
| **自动检测所有子目录** | `driver/Makefile` 自动遍历所有子目录 | **模块较多，扩展性强** |
| **终极优化（无需子目录 `Makefile`）** | 只需放入 `.c` 文件，Makefile 自动编译 | **最简化、自动管理** |

---

这样，你未来只需：
1. **新增目录和 `.c` 代码**。
2. **运行 `make`**，自动完成编译。
3. **无需手动修改 `Makefile`**，新增模块时无需调整配置。

你可以试试看，看看哪种方式适合你的需求！🚀
