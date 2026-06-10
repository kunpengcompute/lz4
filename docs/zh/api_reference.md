# API参考

本文档提供基于开源LZ4优化的鲲鹏LZ4中新增优化宏与函数的定义说明，宏和函数仅在AArch64平台上生效，在其他平台上回退为原始实现。

## 宏与函数说明

鲲鹏优化LZ4中新增的优化宏与函数如[**表1** 鲲鹏优化宏与函数列表](#鲲鹏优化宏与函数列表)所示。

**表1** 鲲鹏优化宏与函数列表<a id="鲲鹏优化宏与函数列表"></a>

|名称|类型|说明|
|--|--|--|
|KZL_MEMCPY_2|宏|2字节NEON向量化内存拷贝，AArch64上使用 `__builtin_memcpy`，其他平台回退为 `memcpy`。|
|KZL_MEMCPY_4|宏|4字节NEON向量化内存拷贝，AArch64上使用 `__builtin_memcpy`，其他平台回退为 `memcpy`。|
|KZL_MEMCPY_8|宏|8字节NEON向量化内存拷贝，AArch64上使用 `vst1_u8/vld1_u8`，其他平台回退为 `memcpy`。|
|KZL_MEMCPY_16|宏|16字节NEON向量化内存拷贝，AArch64上使用 `vst1q_u8/vld1q_u8`，其他平台回退为 `memcpy`。|
|KZL_MEMCPY_32|宏|32字节NEON向量化内存拷贝，AArch64上使用两次 `vst1q_u8/vld1q_u8`，其他平台回退为两次 `LZ4_memcpy`。|
|KZL_LittleEndianfastHash5|函数|小端模式下5字节快速哈希计算，使用预计算常量优化乘法与移位。|
|KZL_BigEndianfastHash5|函数|大端模式下5字节快速哈希计算，使用预计算常量优化乘法与移位。|
|LZ4_hashPositionIgnoreBufferLength|函数|忽略缓冲区长度限制的哈希位置计算，强制使用5字节哈希（LZ4_hash5）以提升匹配精度。|
|skipTrigger|函数|自适应跳步阈值计算，根据输入数据大小动态选择跳步步长（小数据4步、大数据2步）。|
|LZ4_wildCopy16|函数|16字节步长的快速内存拷贝，用于压缩侧字面量拷贝。|
|LZ4_wildCopy64|函数|64字节步长的快速内存拷贝，用于解压缩侧匹配数据拷贝。|
|KZL_FORCE_INLINE|宏|强制内联宏，确保关键优化函数被内联编译。|

## 宏定义

### KZL_MEMCPY系列

**宏功能**

在AArch64平台上，将LZ4压缩、解压缩中的小块内存拷贝替换为NEON SIMD指令，提升内存拷贝吞吐量。在非AArch64平台或不支持GCC 4+的编译器上，回退为标准`memcpy`或`__builtin_memcpy`。

**宏定义**

```c
#if defined(__GNUC__) && (__GNUC__ >= 4)
#    define KZL_MEMCPY_2(dst, src, size) __builtin_memcpy(dst, src, size)
#    define KZL_MEMCPY_4(dst, src, size) __builtin_memcpy(dst, src, size)
#    define KZL_MEMCPY_8(dst, src, size) vst1_u8((dst), vld1_u8(src))
#    define KZL_MEMCPY_16(dst, src, size) vst1q_u8((dst), vld1q_u8(src))
#    define KZL_MEMCPY_32(dst, src, size) vst1q_u8((dst), vld1q_u8(src)); \
                                          vst1q_u8(((dst)+16), vld1q_u8(((src)+16)))
#endif
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|dst|目标地址指针。|有效指针|输出|
|src|源地址指针。|有效指针|输入|
|size|拷贝字节数。|2，4，8，16，32（与宏名对应）|输入|

**平台行为**

|平台|KZL_MEMCPY_2/4|KZL_MEMCPY_8|KZL_MEMCPY_16|KZL_MEMCPY_32|
|--|--|--|--|--|
|AArch64（GCC 4+）|`__builtin_memcpy`|`vst1_u8/vld1_u8`|`vst1q_u8/vld1q_u8`|两次`vst1q_u8/vld1q_u8`|
|其他平台|`memcpy`|`memcpy`|`memcpy`|两次`LZ4_memcpy`|

### KZL_FORCE_INLINE

**宏功能**

强制内联宏，确保关键优化函数被编译器内联，避免函数调用开销。

**宏定义**

```c
#ifndef KZL_FORCE_INLINE
#  ifdef _MSC_VER
#    define KZL_FORCE_INLINE static __forceinline
#  else
#    if defined (__cplusplus) || defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L
#      ifdef __GNUC__
#        define KZL_FORCE_INLINE static inline __attribute__((always_inline))
#      else
#        define KZL_FORCE_INLINE static inline
#      endif
#    else
#      define KZL_FORCE_INLINE static
#    endif
#  endif
#endif
```

## 函数定义

### KZL_LittleEndianfastHash5

**函数功能**

小端模式下5字节快速哈希计算，使用预计算常量`G_KZLPRIME5BYTES`和`SEQUENCE_MOVE`优化乘法与移位操作，提升哈希计算性能。

**函数定义**

```c
KZL_FORCE_INLINE uint32_t KZL_LittleEndianfastHash5(uint64_t sequence, uint8_t HashLogUsage)
{
    return (uint32_t)(((sequence << SEQUENCE_MOVE) * G_KZLPRIME5BYTES) >> (HASH_SIZE - HashLogUsage));
}
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|sequence|从输入数据中读取的64位序列值。|`uint64_t`|输入|
|HashLogUsage|哈希表大小对数。|取决于`tableType`（byU16：LZ4_HASHLOG+1, 其他：LZ4_HASHLOG）|输入|

**返回值**

返回哈希索引值（`uint32_t`），用于在哈希表中查找匹配位置。

**相关常量**

|常量名|值|说明|
|--|--|--|
|SEQUENCE_MOVE|24|序列左移位数。|
|G_KZLPRIME5BYTES|889523592379ULL|5字节哈希乘法常量。|
|HASH_SIZE|64|右移基准位数。|

### KZL_BigEndianfastHash5

**函数功能**

大端模式下5字节快速哈希计算，使用预计算常量`G_KZLPRIME8BYTES`和`SEQUENCE_MOVE`优化乘法与移位操作。

**函数定义**

```c
KZL_FORCE_INLINE uint32_t KZL_BigEndianfastHash5(uint64_t sequence, uint8_t HashLogUsage)
{
    return (uint32_t)(((sequence >> SEQUENCE_MOVE) * G_KZLPRIME8BYTES) >> (HASH_SIZE - HashLogUsage));
}
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|sequence|从输入数据中读取的64位序列值。|`uint64_t`|输入|
|HashLogUsage|哈希表大小对数。|取决于`tableType`|输入|

**返回值**

返回哈希索引值（`uint32_t`）。

**相关常量**

|常量名|值|说明|
|--|--|--|
|SEQUENCE_MOVE|24|序列右移位数。|
|G_KZLPRIME8BYTES|11400714785074694791ULL|8字节哈希乘法常量。|
|HASH_SIZE|64|右移基准位数。|

### LZ4_hashPositionIgnoreBufferLength

**函数功能**

忽略缓冲区长度限制的哈希位置计算函数。与原始`LZ4_hashPosition`不同，此函数在所有情况下（包括`sizeof(reg_t)==4`的场景）均强制使用5字节哈希（LZ4_hash5），以提升在AArch64平台上的匹配精度。

**函数定义**

```c
LZ4_FORCE_INLINE U32 LZ4_hashPositionIgnoreBufferLength(const void* const p, tableType_t const tableType)
{
    return LZ4_hash5(LZ4_read_ARCH(p), tableType);
}
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|p|输入数据位置指针。|有效指针|输入|
|tableType|哈希表类型。|`byU16`、`byU32`、`byPtr`|输入|

**返回值**

返回哈希索引值（`U32`）。

**使用场景**

在鲲鹏优化版本中，`LZ4_hashPosition`的所有调用均替换为`LZ4_hashPositionIgnoreBufferLength`，确保始终使用5字节哈希以获得更好的匹配质量。

### skipTrigger

**函数功能**

自适应跳步阈值计算函数。根据输入数据大小动态选择压缩时的跳步步长，小数据（<64KB+11）使用较大的跳步步长（4）减少无效匹配尝试，大数据使用较小的跳步步长（2）加快匹配查找速度。

**函数定义**

```c
static void skipTrigger(int srcSize, uint8_t *skipStep)
{
    if (skipStep == NULL || srcSize <= 0) {
        return;
    }

    if (srcSize >= G_64KLIMIT) {
        *skipStep = 2;
    } else {
        *skipStep = 4;
    }
}
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|srcSize|输入数据大小。|非负整数|输入|
|skipStep|输出跳步步长指针。|有效指针|输出|

**返回值**

无返回值，通过`skipStep`指针输出结果。

**输出值说明**

|输入条件|输出值|说明|
|--|--|--|
|`srcSize >= G_64KLIMIT` (65547)|2|大数据场景，使用较小跳步加快匹配查找。|
|`srcSize < G_64KLIMIT`|4|小数据场景，使用较大跳步减少无效尝试。|
|`skipStep == NULL`或`srcSize <= 0`|不修改|安全检查，直接返回。|

**相关常量**

|常量名|值|说明|
|--|--|--|
|G_64KLIMIT|65536 + 11|64KB边界阈值，区分大小数据场景。|

### LZ4_wildCopy16

**函数功能**

16字节步长的快速内存拷贝函数，用于压缩侧字面量数据的快速拷贝。相比原始`LZ4_wildCopy8`（8字节步长），每次拷贝16字节，提升拷贝吞吐量。

**函数定义**

```c
LZ4_FORCE_INLINE
void LZ4_wildCopy16(void* dstPtr, const void* srcPtr, void* dstEnd)
{
    BYTE* d = (BYTE*)dstPtr;
    const BYTE* s = (const BYTE*)srcPtr;
    BYTE* const e = (BYTE*)dstEnd;

    do { LZ4_memcpy(d,s,16); d+=16; s+=16; } while (d<e);
}
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|dstPtr|目标地址指针。|有效指针|输入/输出|
|srcPtr|源地址指针。|有效指针|输入|
|dstEnd|目标结束地址指针。|有效指针，`dstEnd >= dstPtr`|输入|

**返回值**

无返回值。

### LZ4_wildCopy64

**函数功能**

64字节步长的快速内存拷贝函数，用于解压缩侧大块匹配数据的快速拷贝。相比原始`LZ4_wildCopy32`（32字节步长），每次拷贝64字节（4次16字节拷贝），大幅提升解压缩吞吐量。

**函数定义**

```c
LZ4_FORCE_INLINE void
LZ4_wildCopy64(void* dstPtr, const void* srcPtr, void* dstEnd)
{
    BYTE* d = (BYTE*)dstPtr;
    const BYTE* s = (const BYTE*)srcPtr;
    BYTE* const e = (BYTE*)dstEnd;

    do {
        LZ4_memcpy(d,s,16);
        LZ4_memcpy(d+16,s+16,16);
        LZ4_memcpy(d+32,s+32,16);
        LZ4_memcpy(d+48,s+48,16);

        d+=64; s+=64;
    } while (d<e);
}
```

**参数说明**

|参数名|描述|取值范围|输入/输出|
|--|--|--|--|
|dstPtr|目标地址指针。|有效指针|输入/输出|
|srcPtr|源地址指针。|有效指针|输入|
|dstEnd|目标结束地址指针。|有效指针，`dstEnd >= dstPtr`|输入|

**返回值**

无返回值。

## 编译优化选项说明

本仓库的补丁在`lib/Makefile`中新增以下编译优化选项。

|选项|说明|
|--|--|
|`-march=native`|针对当前CPU架构生成最优指令（在鲲鹏平台上自动使能NEON/SVE等扩展）。|
|`-flto`|链接时优化（Link-Time Optimization），跨模块内联与死代码消除。|
|`-fno-plt`|禁用PLT（Procedure Linkage Table）跳转，减少动态链接函数调用开销。|
