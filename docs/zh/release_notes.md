# 版本说明书

## 版本配套说明

### 产品版本信息

| 产品名称 | Kunpeng BoostKit |
| :--- | :--- |
| **产品版本** | 26.0.RC1 |
| **软件名称** | lz4（Kunpeng LZ4优化版） |
| **软件包版本** | 1.0.0 |
| **基础版本** | lz4 v1.9.4 |

### 与操作系统、编译器和CPU配套说明

|操作系统|CPU类型|编译器|
|--|--|--|
|openEuler 22.03 LTS SP3|鲲鹏920处理器|Clang 16.0.6+<br>GCC 9.0+|

## 版本更新说明

### V1.0.0

**新增特性**

|特性描述|更新说明|
|--|--|
|新增NEON向量化内存拷贝优化|在aarch64平台上，将LZ4压缩/解压缩中的小块内存拷贝（2/4/8/16/32字节）替换为NEON SIMD指令（`vld1_u8`/`vst1_u8`/`vld1q_u8`/`vst1q_u8`），提升内存拷贝吞吐量。通过 `KZL_MEMCPY_2/4/8/16/32` 系列宏实现平台自适应。|
|新增分支预测优化|在压缩/解压缩关键路径中，对冷路径分支添加 `unlikely()` 标记（如 `tableType == byPtr`、`dictDirective == usingDictCtx`、`outputDirective == limitedOutput` 等），引导CPU分支预测器优化热路径执行。|
|新增数据预取优化|在解压缩快速循环中添加 `__builtin_prefetch` 预取指令，对即将读取的输入数据和匹配数据提前加载到缓存，减少访存延迟。|
|新增自适应哈希与跳步优化|新增 `LZ4_hashPositionIgnoreBufferLength` 函数，在aarch64上强制使用5字节哈希（LZ4_hash5），提升匹配精度；新增 `skipTrigger` 自适应跳步函数，根据输入数据大小动态调整跳步阈值（小数据使用4步、大数据使用2步），减少无效匹配尝试。|
|新增循环对齐优化|在压缩和解压缩主循环入口添加 `__asm__(".p2align 6")` 指令，确保循环起始地址按64字节对齐，改善CPU流水线取指效率。|
|新增更宽拷贝路径优化|新增 `LZ4_wildCopy16`（16字节步长）和 `LZ4_wildCopy64`（64字节步长）函数，在解压缩快速循环中将拷贝步长从32字节提升至64字节，提升大块数据拷贝性能；压缩侧字面量拷贝从 `wildCopy8` 升级为 `wildCopy16`。|
|新增switch-case重排优化|将哈希表操作函数（`LZ4_clearHash`、`LZ4_putIndexOnHash`、`LZ4_putPositionOnHash`）中的 `byPtr` case分支移至switch末尾，利用fallthrough机制减少分支跳转开销（因 `byPtr` 在大多数场景下为冷路径）。|
|新增编译优化选项|在 `lib/Makefile` 中添加 `-march=native`（针对当前CPU架构优化）、`-flto`（链接时优化）、`-fno-plt`（减少PLT跳转开销）编译选项。|
|新增LZ4_STATIC_ASSERT优化|在非DEBUG模式下将 `LZ4_STATIC_ASSERT` 改为空实现，避免编译期断言在Release构建中产生额外开销。|
|新增LZ4_isLittleEndian分支预测优化|在 `LZ4_hash5` 中对 `LZ4_isLittleEndian()` 使用 `__builtin_expect` 标记为热路径，减少大小端判断的分支开销。|

**修改特性**

|特性描述|更新说明|
|--|--|
|修改LZ4_prepareTable条件判断顺序|将 `inputSize >= 4 KB` 条件提前至首位，避免在小数据场景下不必要的哈希表重置。|
|修改read_variable_length溢出检测顺序|调整 `read_variable_length` 中溢出检测条件的判断顺序，将边界检查优先执行，提升安全性。|