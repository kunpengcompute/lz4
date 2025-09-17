/* *
 * Copyright (c) Huawei Technologies Co., Ltd. 2023-2023. All rights reserved.
 * Create: 2023-04
 */

#ifndef LZ4_ACCELERATER_H
#define LZ4_ACCELERATER_H

#include <arm_neon.h>
#include "stddef.h"

#define G_64KLIMIT       ((65536) + (11))
#define G_SMALL_PACKET	 512

#define G_KZLPRIME5BYTES (889523592379ULL)
#define G_KZLPRIME8BYTES (11400714785074694791ULL)

#ifndef KZL_FORCE_INLINE
#  ifdef _MSC_VER
#    define KZL_FORCE_INLINE static __forceinline
#  else
#    if defined (__cplusplus) || defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L   /* C99 */
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

#if defined(__GNUC__) && (__GNUC__ >= 4)
#    define KZL_MEMCPY_2(dst, src, size) __builtin_memcpy(dst, src, size)
#    define KZL_MEMCPY_4(dst, src, size) __builtin_memcpy(dst, src, size)
#    define KZL_MEMCPY_8(dst, src, size) vst1_u8((dst), vld1_u8(src))
#    define KZL_MEMCPY_16(dst, src, size) vst1q_u8((dst), vld1q_u8(src))
#    define KZL_MEMCPY_32(dst, src, size) vst1q_u8((dst), vld1q_u8(src)); vst1q_u8(((dst)+16), vld1q_u8(((src)+16)))
#    define KZL_MEMCPY_16X1(dst, src, size) vst1q_u64((dst), vld1q_u64(src))
#    define KZL_MEMCPY_32X1(dst, src, size) vst1q_u64((dst), vld1q_u64(src)); \
                vst1q_u64(((dst)+16), vld1q_u64(((src)+16)))
#endif

#define SEQUENCE_MOVE 24
#define HASH_SIZE 64

KZL_FORCE_INLINE uint32_t KZL_LittleEndianfastHash5(uint64_t sequence, uint8_t HashLogUsage)
{
    // for data ≤ 64KB
    return (uint32_t)(((sequence << SEQUENCE_MOVE) * G_KZLPRIME5BYTES) >> (HASH_SIZE - HashLogUsage));
}

KZL_FORCE_INLINE uint32_t KZL_BigEndianfastHash5(uint64_t sequence, uint8_t HashLogUsage)
{
    // for data > 64KB
    return (uint32_t)(((sequence >> SEQUENCE_MOVE) * G_KZLPRIME8BYTES) >> (HASH_SIZE - HashLogUsage));
}

#endif