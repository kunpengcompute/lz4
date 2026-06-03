# 快速入门

执行本文档操作前确保已安装基于开源LZ4优化的鲲鹏LZ4，详细安装步骤请参见《[安装指南](./installation_guide.md)》。安装成功后，主要生成产物位于安装目录中（例如：`/path/to/install/lz4-bin`）。

```text
/path/to/install/lz4-bin/
├── bin/
│   └── lz4                         # LZ4命令行工具
├── include/
│   └── lz4.h                       # LZ4块压缩/解压头文件
│   └── lz4hc.h                     # LZ4 HC模式头文件
│   └── lz4frame.h                  # LZ4帧格式头文件
└── lib/ 或 lib64/
    ├── liblz4.so                    # LZ4运行时共享库
    └── pkgconfig/                   # pkg-config配置
```

## 设置环境变量

```bash
export PATH=/path/to/install/lz4-bin/bin:$PATH
export LD_LIBRARY_PATH=/path/to/install/lz4-bin/lib:$LD_LIBRARY_PATH
```

## 使用命令行工具

**基本压缩与解压**

1. 压缩文件。

   ```bash
   lz4 filename filename.lz4
   ```

2. 解压文件。

   ```bash
   lz4 -d filename.lz4 filename
   ```

3. 查看压缩信息。

   ```bash
   lz4 --list filename.lz4
   ```

**常用压缩选项**

| 选项 | 说明 |
| -- | -- |
| `-1` ~ `-9` | 设置压缩级别（默认1，数字越大压缩率越高但速度越慢）。 |
| `--fast` | 最快压缩模式。 |
| `-f` | 强制覆盖输出文件。 |
| `--no-frame-crc` | 禁用帧CRC校验。 |

## 使用C语言API

### 示例代码

C语言块压缩、解压示例代码如下。

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lz4.h>

int main() {
    const char* source = "LZ4 is a very fast compression algorithm, "
                         "providing compression speed at 500 MB/s per core.";
    int source_size = (int)(strlen(source) + 1);

    int max_compressed_size = LZ4_compressBound(source_size);
    char* compressed = (char*)malloc(max_compressed_size);
    if (compressed == NULL) {
        fprintf(stderr, "内存分配失败\n");
        return -1;
    }

    /* 块压缩 */
    int compressed_size = LZ4_compress_default(source, compressed, source_size, max_compressed_size);
    if (compressed_size <= 0) {
        fprintf(stderr, "压缩失败\n");
        free(compressed);
        return -1;
    }
    printf("原始大小: %d 字节\n", source_size);
    printf("压缩后大小: %d 字节\n", compressed_size);
    printf("压缩率: %.2f%%\n", (float)compressed_size / source_size * 100);

    /* 块解压 */
    char* decompressed = (char*)malloc(source_size);
    if (decompressed == NULL) {
        fprintf(stderr, "内存分配失败\n");
        free(compressed);
        return -1;
    }
    int decompressed_size = LZ4_decompress_safe(compressed, decompressed,
                                                  compressed_size, source_size);
    if (decompressed_size <= 0) {
        fprintf(stderr, "解压失败\n");
        free(compressed);
        free(decompressed);
        return -1;
    }
    printf("解压后大小: %d 字节\n", decompressed_size);
    printf("解压后内容: %s\n", decompressed);

    /* 验证数据一致性 */
    if (memcmp(source, decompressed, source_size) == 0) {
        printf("数据一致性验证: 通过\n");
    } else {
        printf("数据一致性验证: 失败\n");
    }

    free(compressed);
    free(decompressed);
    return 0;
}
```

### 编译运行

1. 编译（假设安装在 `/path/to/install/lz4-bin` 目录）。

   ```bash
   gcc -o lz4_example lz4_example.c \
       -I/path/to/install/lz4-bin/include \
       -L/path/to/install/lz4-bin/lib \
       -llz4
   ```

2. 运行。

   ```bash
   ./lz4_example
   ```

   回显结果如下：

   ```text
   原始大小: 92 字节
   压缩后大小: 85 字节
   压缩率: 92.39%
   解压后大小: 92 字节
   解压后内容: LZ4 is a very fast compression algorithm, providing compression speed at 500 MB/s per core.
   数据一致性验证: 通过
   ```

## 常用操作

**快速压缩**

使用指定加速级别进行压缩，加速越高速度越快但压缩率越低。

```c
int compressed_size = LZ4_compress_fast(source, compressed,
                                         source_size, max_compressed_size,
                                         acceleration);
```

**HC高压缩率模式**

使用HC模式获得更高的压缩率（速度较慢）。compression_level取值范围为1~12，默认为9。

```c
#include <lz4hc.h>

int compressed_size = LZ4_compress_HC(source, compressed,
                                       source_size, max_compressed_size,
                                       compression_level);
```

**帧格式压缩、解压**

使用帧格式支持流式压缩与校验。

```c
#include <lz4frame.h>

LZ4F_compressionContext_t cctx;
LZ4F_createCompressionContext(&cctx, LZ4F_VERSION);

size_t dst_capacity = LZ4F_compressBound(src_size, &(LZ4F_preferences_t){0});
char* dst = malloc(dst_capacity);

size_t compressed_size = LZ4F_compressUpdate(cctx, dst, dst_capacity,
                                              src, src_size, NULL);
LZ4F_freeCompressionContext(cctx);
```

**部分解压**

在目标缓冲区不足时解压尽可能多的数据。

```c
int decompressed_size = LZ4_decompress_safe_partial(compressed, decompressed,
                                                      compressed_size,
                                                      target_capacity,
                                                      source_size);
```
