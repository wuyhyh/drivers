### 设备树（Device Tree）是什么？

设备树（Device Tree，DT）是一种用于 **描述硬件** 的数据结构，特别是在 **嵌入式系统** 和 **ARM 架构** 的 Linux
内核中广泛使用。它独立于具体的操作系统，可以让 Linux 内核或 Bootloader 了解底层硬件的信息，而不需要把这些信息硬编码在内核源码中。

在 ARM 体系结构下，设备树通常以 **DTS（Device Tree Source）** 文件的形式编写，经过编译后变成 **DTB（Device Tree Blob）**，由
Bootloader（如 U-Boot）加载并传递给内核。

---

## 1. 为什么需要设备树？

在 **x86 体系** 下，硬件信息通常由 **BIOS/UEFI** 提供，操作系统可以通过 ACPI 或者直接探测硬件。而在 **ARM** 和 **RISC-V**
体系结构中，硬件种类繁多，没有统一的硬件探测机制，因此需要设备树来告诉操作系统硬件的布局。

**设备树的优点：**

- 使 **同一个内核支持不同的硬件**，无需为每块板子维护不同的内核。
- 将 **硬件描述和内核代码分离**，减少内核修改的需求。
- 适用于 **嵌入式设备**（如树莓派、开发板等），简化硬件适配。

---

## 2. 设备树的基本结构

设备树是一个 **树形结构**，由 **节点（Node）** 和 **属性（Property）** 组成，每个节点代表一个硬件设备，每个属性描述该设备的参数。

**设备树示例（DTS 代码）**

```dts
/dts-v1/;

/ {
	model = "My Custom Board";
	compatible = "myvendor,myboard";

	memory {
		device_type = "memory";
		reg = <0x80000000 0x10000000>; // 起始地址和大小
	};

	cpu {
		compatible = "arm,cortex-a72";
	};

	soc {
		compatible = "simple-bus";

		uart0: serial@10009000 {
			compatible = "ns16550a";
			reg = <0x10009000 0x100>;
			clock-frequency = <50000000>;
		};

		gpio: gpio@1000 {
			compatible = "myvendor,gpio";
			reg = <0x1000 0x100>;
		};
	};
};
```

### 解析：

- `/`：根节点，表示整个系统的信息。
- `memory`：描述 RAM 地址和大小（起始 `0x80000000`，大小 `0x10000000`）。
- `cpu`：描述 CPU 类型（Cortex-A72）。
- `soc`：描述 SoC（System on Chip），包括子设备：
    - `uart0`（串口），使用 `ns16550a` 兼容设备，位于 `0x10009000`。
    - `gpio`（GPIO 控制器），地址 `0x1000`。

---

## 3. 设备树文件的转换过程

设备树的源文件（`.dts`）需要经过编译，转换成二进制格式（`.dtb`）才能被内核加载：

```sh
dtc -I dts -O dtb -o myboard.dtb myboard.dts
```

其中：

- `dtc`（Device Tree Compiler）：设备树编译器
- `-I dts`：输入格式是 DTS（设备树源码）
- `-O dtb`：输出格式是 DTB（设备树二进制）
- `-o myboard.dtb`：生成的设备树二进制文件

---

## 4. Linux 内核如何使用设备树？

1. **Bootloader（如 U-Boot）加载 DTB 并传递给内核**
    - U-Boot 通过 `fdt` 命令加载设备树，例如：
      ```sh
      fdt addr ${fdt_addr} && fdt apply ${fdt_addr}
      ```
2. **Linux 内核解析设备树**
    - 启动时，Linux 内核读取 `dtb`，并根据设备树创建对应的设备（`/proc/device-tree`）。
    - 设备驱动会通过 **设备树匹配机制** 加载驱动。

---

## 5. 设备树驱动的匹配机制

在 Linux 设备驱动中，常使用 `of_match_table` 来匹配设备树中的 `compatible` 字段，例如：

```c
static const struct of_device_id my_gpio_of_match[] = {
    { .compatible = "myvendor,gpio" },
    { }
};
MODULE_DEVICE_TABLE(of, my_gpio_of_match);

static struct platform_driver my_gpio_driver = {
    .probe = my_gpio_probe,
    .remove = my_gpio_remove,
    .driver = {
        .name = "my_gpio",
        .of_match_table = my_gpio_of_match,
    },
};

module_platform_driver(my_gpio_driver);
```

- 这里 `of_match_table` 里的 `compatible = "myvendor,gpio"` 对应设备树中的 `compatible = "myvendor,gpio"`，这样驱动才能正确匹配。

---

## 6. 如何在运行时查看设备树？

在 Linux 运行时，你可以在 `/proc/device-tree` 目录查看解析后的设备树：

```sh
ls /proc/device-tree
```

查看某个设备的具体信息：

```sh
cat /proc/device-tree/soc/uart0/compatible
```

---

## 7. 总结

- **设备树是用于描述硬件的结构化数据**，尤其适用于嵌入式和 ARM 设备。
- **DTS（设备树源码）经过编译变成 DTB（设备树二进制）**，由 Bootloader 传递给内核。
- **设备驱动通过 `compatible` 字段匹配设备树中的设备**，从而正确加载驱动。
- **设备树可以在 `/proc/device-tree` 里查看**，帮助调试设备信息。
