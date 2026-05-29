# 安装指南

本文档提供基于鲲鹏优化的LZ4的详细编译安装步骤。

## 环境要求

| 软件 | 版本要求 | 说明 |
| ------ | ---------- | ------ |
| 操作系统 | openEuler 22.03 LTS SP3 | Linux发行版 |
| 编译器 | clang 16.0.6+<br>GCC 9.0+ | C99/C11标准 |
| Make | GNU Make 4.0+ | 构建工具 |
| Git | 2.0或更高版本 | 版本控制 |

## 获取代码

1. 获取LZ4 v1.9.4原始代码。

   ```bash
   git clone -b v1.9.4 https://github.com/lz4/lz4.git lz4-kunpeng
   cd lz4-kunpeng
   ```

2. 获取鲲鹏优化patch。

   从本代码仓下载 `kunpeng_lz4_v1.0.patch` 文件到本地目录。

## 应用patch

将patch文件应用到LZ4源码目录。

```bash
cd lz4-kunpeng
patch -p1 < /path/to/kunpeng_lz4_v1.0.patch
```

如果patch成功应用，回显如下：

```text
patching file lib/lz4.c
patching file lib/Makefile
```

## 编译安装

1. 创建自定义安装目录（可根据实际需求修改路径）。

   ```bash
   mkdir -p /path/to/install/lz4-bin
   export LZ4_INSTALL_DIR=/path/to/install/lz4-bin
   ```

2. 编译。

   - 使用默认优化选项编译（Makefile已包含 `-march=native -flto -fno-plt` 优化选项）。

     ```bash
     make
     ```

   - 或手动指定额外编译选项。

     ```bash
     make CFLAGS="-O3 -march=armv9.2-a+crc+sve+sve2+sve2-bitperm -mtune=native"
     ```

3. 安装。

   ```bash
   make install PREFIX=${LZ4_INSTALL_DIR}
   ```

4. 验证安装目录。

   ```bash
   ls -la ${LZ4_INSTALL_DIR}
   ```

   回显结果如下：

   ```text
   total 28
   drwxr-xr-x  5 user user  4096 May 29 10:00 .
   drwxr-xr-x  33 user user  4096 May 29 16:39 ..
   drwxr-xr-x  2 user user  4096 May 29 10:00 bin
   drwxr-xr-x  3 user user  4096 May 29 10:00 include
   drwxr-xr-x  3 user user  4096 May 29 10:00 lib
   ```

5. 查看动态库。

   ```bash
   ls -la ${LZ4_INSTALL_DIR}/lib/liblz4*
   ```

   回显结果如下：

   ```text
   lrwxrwxrwx. 1 user user      16 May 29 10:00 lib/liblz4.so -> liblz4.so.1.9.4
   -rwxr-xr-x. 1 user user  128000 May 29 09:50 lib/liblz4.so.1.9.4
   ```

6. 查看命令行工具。

   ```bash
   ls -la ${LZ4_INSTALL_DIR}/bin/
   ```

   回显结果如下：

   ```text
   -rwxr-xr-x. 1 user user  30000 May 29 09:50 lz4
   ```

## 编译选项说明

### Make常用变量

| 变量 | 默认值 | 说明 |
| ------ | -------- | ------ |
| `CFLAGS` | `-O3 -march=native` | 编译器优化标志 |
| `PREFIX` | `/usr/local` | 安装路径前缀 |
| `LDFLAGS` | - | 链接器标志 |

### 性能优化编译标志

针对ARM架构（鲲鹏处理器）的推荐编译标志。

```bash
make CFLAGS="-O3 -march=armv9.2-a+crc+sve+sve2+sve2-bitperm -mtune=native -flto -fno-plt"
```

## 运行测试

1. 进入lz4源码目录。

   ```shell
   cd lz4-kunpeng
   ```

2. 运行测试。

   ```shell
   make test
   ```

3. 运行性能基准测试。

   ```shell
   make bench
   ```