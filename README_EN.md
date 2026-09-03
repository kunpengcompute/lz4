# Introduction to LZ4

## Latest Updates

- [2026-06-30] Released Kunpeng LZ4 optimized based on LZ4 v1.9.4, including optimizations such as NEON vectorized memory copy, branch prediction, and data prefetch.

## Project Introduction

### Overview

LZ4 is a **highly fast, lossless compression algorithm** developed by Yann Collet. Belonging to the LZ77 family, it is renowned for its compression speed (reaching over 500 MB/s per core) while providing reasonable compression ratios. Its core advantages include extremely fast compression and decompression speeds, a small memory footprint, support for both streaming and block compression modes, and multi-language bindings for C, C++, Java, Python, and more.

### Core Modules

LZ4 is divided into two core components: the **compression module** and the **decompression module**. Its internal module structure is detailed below:

- **Compression module**: Provides basic capabilities such as block compression (`LZ4_compress_default`, `LZ4_compress_fast`, and `LZ4_compress_HC`) and streaming compression (`LZ4_compress_continue` and `LZ4_compress_fastContinue`).
- **Decompression module**: Provides basic capabilities such as block decompression (`LZ4_decompress_safe` and `LZ4_decompress_fast`) and partial decompression (`LZ4_decompress_safe_partial`).
- **Hash module**: Implements 4-byte and 5-byte fast hash calculations used for match lookup during compression.
- **Memory copy module**: Implements highly efficient data copy paths including `wildCopy8`, `wildCopy16`, `wildCopy32`, and `wildCopy64`.
- **Frame format module**: Provides compression and decompression interfaces for the LZ4 frame format (`lz4frame`), supporting streaming operations and verification.

### Core External API Functions

The following table lists the core external API functions of LZ4 (using C as an example), covering essential capabilities such as compression, decompression, and streaming operations.

| Module| API Function| Description|
| -- | -- | -- |
| Block compression| `LZ4_compress_default` | Compresses source data into a target buffer using the default acceleration level (`1`).|
| Block compression| `LZ4_compress_fast` | Compresses source data using a specified acceleration level (1 to 65537). Higher levels result in faster speeds but lower compression ratios.|
| Block compression| `LZ4_compress_HC` | Compresses source data using high compression mode, which is slower but delivers higher compression ratios.|
| Block compression| `LZ4_compress_destSize` | Compresses data with a constraint on the output size, ensuring that the output does not exceed the specified size.|
| Block decompression| `LZ4_decompress_safe` | Performs safe decompression. It requires the original data size to prevent buffer overflow.|
| Block decompression| `LZ4_decompress_fast` | Performs fast decompression. It requires the original data size but skips boundary checks (faster but less safe).|
| Block decompression| `LZ4_decompress_safe_partial` | Performs partial safe decompression, decompressing as much data as possible when the target buffer space is insufficient.|
| Streaming compression| `LZ4_compress_continue` | Performs streaming compression (using an internal state), supporting the continuous compression of multiple blocks.|
| Streaming compression| `LZ4_compress_fastContinue` | Performs streaming compression (using an internal state), supporting a specified acceleration level.|
| Frame format| `LZ4F_compressBegin` | Begins frame format compression and initializes the compression context.|
| Frame format| `LZ4F_compressUpdate` | Compresses data blocks in frame format (streaming).|
| Frame format| `LZ4F_decompress` | Performs decompression in frame format, supporting streaming decompression.|

## Directory Structure

The project directory structure is as follows:

```text
# Documentation directory
README_EN.md                       # Project description
LICENSE                            # Code license
kunpeng_lz4_v1.0.patch             # Kunpeng optimization patch file
docs/
├── LICENSE                        # Document license
└── en/
    ├── api_reference.md           # API reference
    ├── installation_guide.md      # Installation guide
    ├── quick_start.md             # Quick start guide
    ├── menu_lz4.md                # Navigation menu
    └── release_notes.md           # Release Notes

# Code directory (original LZ4 v1.9.4 source code + patch applied)
lz4/
├── lib/                           # Core library source code
│   ├── lz4.c                      # Core LZ4 compression/decompression implementation (target of this optimization patch)
│   ├── lz4hc.c                    # LZ4 high compression mode implementation
│   ├── lz4frame.c                 # LZ4 frame format implementation
│   ├── lz4.h                      # Header file for LZ4 block compression/decompression
│   ├── lz4hc.h                    # Header file for LZ4 high compression mode
│   ├── lz4frame.h                 # Header file for LZ4 frame format
│   ├── lz4frame_static.h          # Internal header file for frame format
│   ├── lz4file.c                  # Helper for file operations
│   ├── lz4file.h                  # Helper header file for file operations
│   ├── xxhash.c                   # XXHash hash algorithm implementation
│   ├── xxhash.h                   # XXHash hash algorithm header file
│   └── Makefile                   # Library build file (target of this optimization patch)
├── programs/                      # Command-line utilities
│   ├── lz4cli.c                   # Command-line entry point
│   ├── lz4io.c                    # File I/O operations
│   ├── bench.c                    # Performance benchmarking
│   └── datagen.c                  # Test data generation
├── tests/                         # Test suites
├── doc/                           # Format specification document
├── ossfuzz/                       # OSS-Fuzz test
├── contrib/                       # Auxiliary tools and examples
├── build/                         # Build configurations (CMake/VS)
├── examples/                      # Usage examples
└── Makefile                       # Top-level build file
```

## Release Notes

For details about the feature changes in each release of Kunpeng LZ4 optimized based on open-source LZ4, see [Release Notes](docs/en/release_notes.md).

## Environment Deployment

For details about the compilation environment, dependency obtaining, and installation procedure of Kunpeng LZ4, see [Installation Guide](docs/en/installation_guide.md).

## Quick Start

For details about how to quickly get started with LZ4, see [Quick Start](docs/en/quick_start.md).

## Documentation

|Document Name|Description|
|--|--|
|[Release Notes](docs/en/release_notes.md)|Provides basic information and feature updates of each release of Kunpeng LZ4.|
|[Quick Start](docs/en/quick_start.md)|Provides quick start examples and compilation instructions for Kunpeng LZ4.|
|[API Reference](docs/en/api_reference.md)|Provides descriptions and definitions of macros and functions related to NEON vectorized memory copy and hash optimization.|
|[Installation Guide](docs/en/installation_guide.md)|Provides detailed instructions on configuring, compiling, and installing Kunpeng LZ4.|

## Disclaimer

**To LZ4 Users**

- This software is intended solely for debugging and development. You are responsible for any risks and should carefully review the following information:
    - This code repository contributes to the LZ4 open-source project solely for performance optimization of certain LZ4 functions on Kunpeng processors. It strictly adheres to the coding style and methods, as well as security design of the original open-source software. Any vulnerability and security issues of the software shall be resolved by the corresponding upstream communities according to their response mechanisms. Please pay attention to the notifications and version updates released by the upstream communities. The Kunpeng computing community does not assume any responsibility for software vulnerabilities and security issues.
    - Data processing and deletion: Users are responsible for managing and deleting any data generated while using this software. You are advised to promptly delete any related data after use to prevent information leaks.
    - Data confidentiality and transmission: Users understand and agree not to share or transmit any data generated by this software. Neither the software nor its developers are responsible for any information leaks, data breaches, or other negative consequences.
    - User input security: Users are responsible for the security of any commands they enter and for any risks or losses resulting from improper input. The software and its developers are not liable for issues caused by incorrect command usage.

- Disclaimer scope: This disclaimer applies to all individuals and entities using this software. By using the software, you acknowledge and accept this statement and assume all risks and responsibilities arising from its use. If you do not agree, please stop using the software immediately.
- Before using this software, please **read and understand the preceding disclaimer**. If you have any questions, contact the developer.

**To Data Owners**

If you do not want your dataset to be mentioned in the LZ4 optimization repository, or if you wish to update its description, please submit an issue on GitCode. We will delete or update your description according to your request. Thank you for your understanding and support.

## License

- The core LZ4 library is licensed under BSD-2-Clause. For details, see [LICENSE](LICENSE).

- The documents of LZ4 are licensed under CC-BY 4.0. For details, see [LICENSE](docs/LICENSE).

## Contribution Statement

We welcome your contributions to the community. If you have any questions/suggestions or want to provide feedback on feature requirements and bug reports, you can submit issues. For details, see Contribution Guideline. You are also welcome to share insights in Discussions. Thank you for your support.

## Acknowledgments

LZ4 is primarily contributed by Huawei.

Thank you to everyone in the community for your PRs. We warmly welcome contributions to LZ4!
