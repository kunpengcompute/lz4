# 安装指南

本文档提供开源LZ4优化的鲲鹏LZ4的安装要求与详细编译安装步骤。

## 环境要求

| 软件 | 版本要求 | 说明 |
| ------ | ---------- | ------ |
| 操作系统 | openEuler 22.03 LTS SP3 | Linux发行版 |
| 编译器 | clang 16.0.6版本及以上<br>GCC 9.0版本及以上 | C99/C11标准 |
| Make | GNU Make 4.0+ | 构建工具 |
| Git | 大于等于2.0版本 | 版本控制 |

## 获取代码

提供两种方式获取基于开源LZ4优化的鲲鹏LZ4代码，任选其一即可。

* 方式一：拉取原始代码并应用补丁

  1. 获取LZ4 v1.9.4原始代码。

     ```bash
     git clone -b v1.9.4 https://github.com/lz4/lz4.git lz4-kunpeng
     cd lz4-kunpeng
     ```

  2. 从本代码仓下载 `kunpeng_lz4_v1.0.patch` 文件到本地目录，然后应用补丁。

     ```bash
     curl -L -k "https://gitcode.com/boostkit/lz4/master/kunpeng_lz4_v1.0.patch" -o kunpeng_lz4_v1.0.patch
     patch -p1 < ./kunpeng_lz4_v1.0.patch
     ```

     若补丁成功应用，回显如下：

     ```text
     patching file lib/lz4.c
     patching file lib/Makefile
     ```

      补丁应用后代码目录如下：

     ```text
      lz4/
      ├── lib/                           # 核心库源码
      │   ├── lz4.c                      # LZ4压缩/解压缩核心实现（本patch优化目标）
      │   ├── lz4hc.c                    # LZ4高压缩率模式实现
      │   ├── lz4frame.c                 # LZ4帧格式实现
      │   ├── lz4.h                      # LZ4块压缩/解压缩头文件
      │   ├── lz4hc.h                    # LZ4 HC模式头文件
      │   ├── lz4frame.h                 # LZ4帧格式头文件
      │   ├── lz4frame_static.h          # 帧格式内部头文件
      │   ├── lz4file.c                  # 文件操作辅助
      │   ├── lz4file.h                  # 文件操作辅助头文件
      │   ├── xxhash.c                   # XXHash哈希算法实现
      │   ├── xxhash.h                   # XXHash哈希算法头文件
      │   └── Makefile                   # 库构建文件（本patch优化目标）
      ├── programs/                      # 命令行程序
      │   ├── lz4cli.c                   # 命令行入口
      │   ├── lz4io.c                    # 文件I/O操作
      │   ├── bench.c                    # 性能基准测试
      │   └── datagen.c                  # 测试数据生成
      ├── tests/                         # 测试套件
      ├── doc/                           # 格式说明文档
      ├── ossfuzz/                       # OSS-Fuzz模糊测试
      ├── contrib/                       # 辅助工具与示例
      ├── build/                         # 构建配置（CMake/VS）
      ├── examples/                      # 使用示例
      └── Makefile                       # 顶层构建文件
     ```

* 方式二：直接拉取优化版代码

  直接从鲲鹏优化代码仓拉取已包含优化的LZ4代码。

  ```bash
  git clone -b dev-1.9.4 https://gitcode.com/boostkit/lz4.git lz4-kunpeng
  cd lz4-kunpeng
  ```

  该分支已包含鲲鹏优化代码，无需额外应用补丁。

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

**Make常用变量**

| 变量 | 默认值 | 说明 |
| ------ | -------- | ------ |
| `CFLAGS` | `-O3 -march=native` | 编译器优化标志。 |
| `PREFIX` | `/usr/local` | 安装路径前缀。 |
| `LDFLAGS` | - | 链接器标志。 |

**性能优化编译标志**

针对ARM架构（鲲鹏处理器）的推荐编译标志。

```bash
make CFLAGS="-O3 -march=armv9.2-a+crc+sve+sve2+sve2-bitperm -mtune=native -flto -fno-plt"
```

## 运行测试

1. 进入LZ4源码目录。

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
