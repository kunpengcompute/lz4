# API Reference

This document provides the definitions for the optimization macros and functions newly added to the Kunpeng LZ4 library, which is optimized based on open-source LZ4. These macros and functions take effect only on the AArch64 platform and fall back to the original implementation on other platforms.

## Macro and Function Description

[**Table 1** Kunpeng optimization macros and functions](#kunpeng-optimization-macros-and-functions) lists the optimization macros and functions.

**Table 1** Kunpeng optimization macros and functions<a id="kunpeng-optimization-macros-and-functions"></a>

|Name|Type|Description|
|--|--|--|
|KZL_MEMCPY_2|Macro|2-byte NEON vectorized memory copy. Uses `__builtin_memcpy` on AArch64 and falls back to `memcpy` on other platforms.|
|KZL_MEMCPY_4|Macro|4-byte NEON vectorized memory copy. Uses `__builtin_memcpy` on AArch64 and falls back to `memcpy` on other platforms.|
|KZL_MEMCPY_8|Macro|8-byte NEON vectorized memory copy. Uses `vst1_u8/vld1_u8` on AArch64 and falls back to `memcpy` on other platforms.|
|KZL_MEMCPY_16|Macro|16-byte NEON vectorized memory copy. Uses `vst1q_u8/vld1q_u8` on AArch64 and falls back to `memcpy` on other platforms.|
|KZL_MEMCPY_32|Macro|32-byte NEON vectorized memory copy. Uses `vst1q_u8/vld1q_u8` twice on AArch64 and falls back to `LZ4_memcpy` twice on other platforms.|
|KZL_LittleEndianfastHash5|Function|5-byte fast hash calculation in little-endian mode, using pre-computed constants to optimize multiplication and shift operations.|
|KZL_BigEndianfastHash5|Function|5-byte fast hash calculation in big-endian mode, using pre-computed constants to optimize multiplication and shift operations.|
|LZ4_hashPositionIgnoreBufferLength|Function|Hash position calculation that ignores the buffer length limit, forcing the use of a 5-byte hash (`LZ4_hash5`) to improve matching accuracy.|
|skipTrigger|Function|Adaptive skip threshold calculation that dynamically selects the skip step size based on the input data size (4 steps for small data, 2 steps for large data).|
|LZ4_wildCopy16|Function|Fast memory copy with a 16-byte step size, used for literal copy on the compression side.|
|LZ4_wildCopy64|Function|Fast memory copy with a 64-byte step size, used for matched data copy on the decompression side.|
|KZL_FORCE_INLINE|Macro|Forced inline macro to ensure critical optimization functions are compiled as inline.|

## Macro Definitions

### KZL_MEMCPY Series

**Macro Function**

Replaces small-block memory copies in LZ4 compression and decompression with NEON SIMD instructions on the AArch64 platform to improve memory copy throughput, and falls back to the standard `memcpy` or `__builtin_memcpy` on non-AArch64 platforms or compilers that do not support GCC 4+.

**Macro Definition**

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

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|dst|Pointer to the destination address.|Valid pointer|Output|
|src|Pointer to the source address.|Valid pointer|Input|
|size|Number of bytes to be copied.|2, 4, 8, 16, 32 (corresponding to the macro name)|Input|

**Platform Behavior**

|Platform|KZL_MEMCPY_2/4|KZL_MEMCPY_8|KZL_MEMCPY_16|KZL_MEMCPY_32|
|--|--|--|--|--|
|AArch64 (GCC 4+)|`__builtin_memcpy`|`vst1_u8/vld1_u8`|`vst1q_u8/vld1q_u8`|Two `vst1q_u8/vld1q_u8`|
|Other platforms|`memcpy`|`memcpy`|`memcpy`|Two `LZ4_memcpy`|

### KZL_FORCE_INLINE

**Macro Function**

Forces the compiler to inline critical optimization functions, eliminating function call overhead.

**Macro Definition**

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

## Function Definitions

### KZL_LittleEndianfastHash5

**Function Usage**

Calculates a 5-byte fast hash in little-endian mode, using pre-computed constants `G_KZLPRIME5BYTES` and `SEQUENCE_MOVE` to optimize multiplication and shift operations, thereby improving hash calculation performance.

**Function Syntax**

```c
KZL_FORCE_INLINE uint32_t KZL_LittleEndianfastHash5(uint64_t sequence, uint8_t HashLogUsage)
{
    return (uint32_t)(((sequence << SEQUENCE_MOVE) * G_KZLPRIME5BYTES) >> (HASH_SIZE - HashLogUsage));
}
```

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|sequence|64-bit sequence value read from the input data.|`uint64_t`|Input|
|HashLogUsage|Logarithm of the hash table size.|Depending on `tableType` (byU16: LZ4_HASHLOG + 1; others: LZ4_HASHLOG)|Input|

**Return Value**

Returns the hash index value (`uint32_t`), which is used to find the matching position in the hash table.

**Related Constants**

|Constant|Value|Description|
|--|--|--|
|SEQUENCE_MOVE|24|Number of bits for left-shifting the sequence.|
|G_KZLPRIME5BYTES|889523592379ULL|5-byte hash multiplication constant.|
|HASH_SIZE|64|Baseline number of bits for right-shifting.|

### KZL_BigEndianfastHash5

**Function Usage**

Calculates a 5-byte fast hash in big-endian mode, using pre-computed constants `G_KZLPRIME8BYTES` and `SEQUENCE_MOVE` to optimize multiplication and shift operations.

**Function Syntax**

```c
KZL_FORCE_INLINE uint32_t KZL_BigEndianfastHash5(uint64_t sequence, uint8_t HashLogUsage)
{
    return (uint32_t)(((sequence >> SEQUENCE_MOVE) * G_KZLPRIME8BYTES) >> (HASH_SIZE - HashLogUsage));
}
```

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|sequence|64-bit sequence value read from the input data.|`uint64_t`|Input|
|HashLogUsage|Logarithm of the hash table size.|Depending on `tableType`|Input|

**Return Value**

Returns the hash index value (`uint32_t`).

**Related Constants**

|Constant|Value|Description|
|--|--|--|
|SEQUENCE_MOVE|24|Number of bits for right-shifting the sequence.|
|G_KZLPRIME8BYTES|11400714785074694791ULL|8-byte hash multiplication constant.|
|HASH_SIZE|64|Baseline number of bits for right-shifting.|

### LZ4_hashPositionIgnoreBufferLength

**Function Usage**

Calculates the hash position by ignoring the buffer length limit. Unlike the original `LZ4_hashPosition`, this function forces the use of a 5-byte hash (`LZ4_hash5`) in all scenarios (including those where `sizeof(reg_t)==4`) to improve matching accuracy on the AArch64 platform.

**Function Syntax**

```c
LZ4_FORCE_INLINE U32 LZ4_hashPositionIgnoreBufferLength(const void* const p, tableType_t const tableType)
{
    return LZ4_hash5(LZ4_read_ARCH(p), tableType);
}
```

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|p|Pointer to the input data position.|Valid pointer|Input|
|tableType|Hash table type.|`byU16`, `byU32`, `byPtr`|Input|

**Return Value**

Returns the hash index value (`U32`).

**Application Scenario**

In the Kunpeng-optimized version, all invocations of `LZ4_hashPosition` are replaced with `LZ4_hashPositionIgnoreBufferLength` to ensure that a 5-byte hash is always used for better matching quality.

### skipTrigger

**Function Usage**

Calculates an adaptive skip threshold. It dynamically selects the skip step during compression based on the input data size: using a larger step size (`4`) for small data (< 64 KB + 11 bytes) to reduce ineffective match attempts, and a smaller step size (`2`) for large data to accelerate match lookup.

**Function Syntax**

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

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|srcSize|Input data size.|Non-negative integer|Input|
|skipStep|Pointer to the output step.|Valid pointer|Output|

**Return Value**

None. The result is output via the `skipStep` pointer.

**Output Value Description**

|Input Condition|Output Value|Description|
|--|--|--|
|`srcSize >= G_64KLIMIT` (65547)|2|Indicates the large data scenario, where a smaller skip step is used to accelerate match lookup.|
|`srcSize < G_64KLIMIT`|4|Indicates the small data scenario, where a larger skip step is used to reduce ineffective attempts.|
|`skipStep == NULL` or `srcSize <= 0`|Unchanged|Indicates the safety check scenario, where the function returns directly.|

**Related Constants**

|Constant|Value|Description|
|--|--|--|
|G_64KLIMIT|65536 + 11|64 KB boundary threshold to differentiate between large and small data scenarios.|

### LZ4_wildCopy16

**Function Usage**

Performs a fast memory copy with a 16-byte step, used for the fast copy of literal data on the compression side. Compared to the original `LZ4_wildCopy8` (8-byte step), it copies 16 bytes per operation to improve copy throughput.

**Function Syntax**

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

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|dstPtr|Pointer to the destination address.|Valid pointer|Input/Output|
|srcPtr|Pointer to the source address.|Valid pointer|Input|
|dstEnd|Pointer to the end destination address.|Valid pointer, `dstEnd >= dstPtr`|Input|

**Return Value**

None

### LZ4_wildCopy64

**Function Usage**

Performs a fast memory copy with a 64-byte step, used for the fast copy of large-block matched data on the decompression side. Compared to the original `LZ4_wildCopy32` (32-byte step), it copies 64 bytes per operation (via four 16-byte copies) to significantly improve decompression throughput.

**Function Syntax**

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

**Parameters**

|Parameter|Description|Value Range|Input/Output|
|--|--|--|--|
|dstPtr|Pointer to the destination address.|Valid pointer|Input/Output|
|srcPtr|Pointer to the source address.|Valid pointer|Input|
|dstEnd|Pointer to the end destination address.|Valid pointer, `dstEnd >= dstPtr`|Input|

**Return Value**

None

## Compilation Optimization Options

The patch in this repository introduces the following compilation optimization options to `lib/Makefile`.

|Option|Description|
|--|--|
|`-march=native`|Generates optimal instructions for the current CPU architecture (automatically enabling NEON/SVE extensions on the Kunpeng platform).|
|`-flto`|Enables link-time optimization (LTO) to perform cross-module inlining and dead code elimination.|
|`-fno-plt`|Disables procedure linkage table (PLT) jumps to reduce the overhead of dynamic link function calls.|
