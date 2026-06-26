# Quick Start

Before performing the operations described in this document, ensure that Kunpeng LZ4—which is built upon the open-source LZ4—has been installed. For detailed installation steps, see [Installation Guide](./installation_guide.md). After the installation is successful, the generated files are stored in the installation directory, for example, `/path/to/install/lz4-bin`.

```text
/path/to/install/lz4-bin/
├── bin/
│   └── lz4                         # LZ4 command-line tool
├── include/
│   └── lz4.h                       # Header file for LZ4 block compression/decompression
│   └── lz4hc.h                     # Header file for LZ4 high compression mode
│   └── lz4frame.h                  # Header file for LZ4 frame format
└── lib/ or lib64/
    ├── liblz4.so                    # LZ4 runtime shared library
    └── pkgconfig/
```

## Setting Environment Variables

```bash
export PATH=/path/to/install/lz4-bin/bin:$PATH
export LD_LIBRARY_PATH=/path/to/install/lz4-bin/lib:$LD_LIBRARY_PATH
```

## Using the Command-Line Tool

**Basic Compression and Decompression**

1. Compress a file.

   ```bash
   lz4 filename filename.lz4
   ```

2. Decompress the file.

   ```bash
   lz4 -d filename.lz4 filename
   ```

3. View compression information.

   ```bash
   lz4 --list filename.lz4
   ```

**Common Compression Options**

| Option| Description|
| -- | -- |
| `-1` to `-9`| Specifies the compression level (The default value is `1`; larger numbers provide higher compression ratios but slower speeds).|
| `--fast` | Enables the fastest compression mode.|
| `-f` | Forces the overwriting of the output file.|
| `--no-frame-crc` | Disables frame CRC checksums.|

## Using C APIs

### Sample Code

The sample code for C block compression and decompression is as follows:

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
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    /* Block compression */
    int compressed_size = LZ4_compress_default(source, compressed, source_size, max_compressed_size);
    if (compressed_size <= 0) {
        fprintf(stderr, "Compression failed\n");
        free(compressed);
        return -1;
    }
    printf("Original size: %d bytes\n", source_size);
    printf("Compressed size: %d bytes\n", compressed_size);
    printf("Compression ratio: %.2f%%\n", (float)compressed_size / source_size * 100);

    /* Block decompression */
    char* decompressed = (char*)malloc(source_size);
    if (decompressed == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(compressed);
        return -1;
    }
    int decompressed_size = LZ4_decompress_safe(compressed, decompressed,
                                                  compressed_size, source_size);
    if (decompressed_size <= 0) {
        fprintf(stderr, "Decompression failed\n");
        free(compressed);
        free(decompressed);
        return -1;
    }
    printf("Decompressed size: %d bytes\n", decompressed_size);
    printf("Decompressed content: %s\n", decompressed);

    /* Verify data consistency */
    if (memcmp(source, decompressed, source_size) == 0) {
        printf("Data consistency verification: Passed\n");
    } else {
        printf("Data consistency verification: Failed\n");
    }

    free(compressed);
    free(decompressed);
    return 0;
}
```

### Compilation and Execution

1. Compile the code (assuming the installation is in the `/path/to/install/lz4-bin` directory).

   ```bash
   gcc -o lz4_example lz4_example.c \
       -I/path/to/install/lz4-bin/include \
       -L/path/to/install/lz4-bin/lib \
       -llz4
   ```

2. Run the executable.

   ```bash
   ./lz4_example
   ```

   Command output:

   ```text
   Original size: 92 bytes
   Compressed size: 85 bytes
   Compression ratio: 92.39%
   Decompressed size: 92 bytes
   Decompressed content: LZ4 is a very fast compression algorithm, providing compression speed at 500 MB/s per core.
   Data consistency verification: Passed
   ```

## Common Operations

**Fast Compression**

Performs compression using a specified acceleration level. A higher acceleration level results in faster compression speeds but lower compression ratios.

```c
int compressed_size = LZ4_compress_fast(source, compressed,
                                         source_size, max_compressed_size,
                                         acceleration);
```

**High Compression Mode**

Enables high compression mode to achieve a higher compression ratio at a lower speed. The value of `compression_level` ranges from 1 to 12, with a default value of 9.

```c
#include <lz4hc.h>

int compressed_size = LZ4_compress_HC(source, compressed,
                                       source_size, max_compressed_size,
                                       compression_level);
```

**Frame Format Compression and Decompression**

Uses the frame format to support streaming compression and verification.

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

**Partial Decompression**

Decompresses as much data as possible when the target buffer space is insufficient.

```c
int decompressed_size = LZ4_decompress_safe_partial(compressed, decompressed,
                                                      compressed_size,
                                                      target_capacity,
                                                      source_size);
```
