# Installation Guide

This document describes the installation requirements and detailed compilation and installation procedure of Kunpeng LZ4 optimized based on open-source LZ4.

## Environment Requirements

| Software| Version Requirement| Description|
| ------ | ---------- | ------ |
| OS| openEuler 22.03 LTS SP3| Linux distribution|
| Compiler| Clang 16.0.6 or later<br>GCC 9.0 or later| C99/C11 standard|
| Make | GNU Make 4.0+ | Build tool|
| Git | 2.0 or later| Version control|

## Obtaining Code

You can use either of the following methods to obtain the Kunpeng LZ4 code:

* Method 1: Cloning the original code and applying a patch

  1. Obtain the LZ4 v1.9.4 source code.

     ```bash
     git clone -b v1.9.4 https://github.com/lz4/lz4.git lz4-kunpeng
     cd lz4-kunpeng
     ```

  2. Download the `kunpeng_lz4_v1.0.patch` file from this repository to a local directory and apply the patch.

     ```bash
     patch -p1 < /path/to/kunpeng_lz4_v1.0.patch
     ```

     If the patch is successfully applied, the following information is displayed:

     ```text
     patching file lib/lz4.c
     patching file lib/Makefile
     ```

* Method 2: Directly cloning the optimized code

  Directly clone the optimized LZ4 code from the Kunpeng-optimized code repository.

  ```bash
  git clone -b dev-1.9.4 https://gitcode.com/boostkit/lz4.git lz4-kunpeng
  cd lz4-kunpeng
  ```

  This branch already contains the Kunpeng-optimized code, so no additional patch application is required.

## Compilation and Installation

1. Create a custom installation directory. You can change the directory as required.

   ```bash
   mkdir -p /path/to/install/lz4-bin
   export LZ4_INSTALL_DIR=/path/to/install/lz4-bin
   ```

2. Perform compilation.

   * Use the default optimization options for compilation. (The Makefile contains the `-march=native -flto -fno-plt` optimization options.)

     ```bash
     make
     ```

   * Alternatively, manually specify additional compilation options.

     ```bash
     make CFLAGS="-O3 -march=armv9.2-a+crc+sve+sve2+sve2-bitperm -mtune=native"
     ```

3. Perform installation.

   ```bash
   make install PREFIX=${LZ4_INSTALL_DIR}
   ```

4. Verify the installation directory.

   ```bash
   ls -la ${LZ4_INSTALL_DIR}
   ```

   Command output:

   ```text
   total 28
   drwxr-xr-x  5 user user  4096 May 29 10:00 .
   drwxr-xr-x  33 user user  4096 May 29 16:39 ..
   drwxr-xr-x  2 user user  4096 May 29 10:00 bin
   drwxr-xr-x  3 user user  4096 May 29 10:00 include
   drwxr-xr-x  3 user user  4096 May 29 10:00 lib
   ```

5. Verify the dynamic libraries.

   ```bash
   ls -la ${LZ4_INSTALL_DIR}/lib/liblz4*
   ```

   Command output:

   ```text
   lrwxrwxrwx. 1 user user      16 May 29 10:00 lib/liblz4.so -> liblz4.so.1.9.4
   -rwxr-xr-x. 1 user user  128000 May 29 09:50 lib/liblz4.so.1.9.4
   ```

6. Verify the command-line tool.

   ```bash
   ls -la ${LZ4_INSTALL_DIR}/bin/
   ```

   Command output:

   ```text
   -rwxr-xr-x. 1 user user  30000 May 29 09:50 lz4
   ```

## Compilation Option Description

**Common Make Variables**

| Variable| Default Value| Description|
| ------ | -------- | ------ |
| `CFLAGS` | `-O3 -march=native` | Compiler optimization flag.|
| `PREFIX` | `/usr/local` | Installation path prefix.|
| `LDFLAGS` | - | Linker flag.|

**Performance Optimization Compilation Flags**

Recommended compilation flags for the Arm architecture (Kunpeng processors).

```bash
make CFLAGS="-O3 -march=armv9.2-a+crc+sve+sve2+sve2-bitperm -mtune=native -flto -fno-plt"
```

## Running Test

1. Go to the LZ4 source code directory.

   ```shell
   cd lz4-kunpeng
   ```

2. Run the test.

   ```shell
   make test
   ```

3. Run the performance benchmark test.

   ```shell
   make bench
   ```
