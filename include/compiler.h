/*
 * Copyright © 2017 Samuel Holland <samuel@sholland.org>
 * SPDX-License-Identifier: GPL-2.0
 */

#ifndef COMPILER_H
#define COMPILER_H

/* Attributes */
#define __alias(symbol)        __attribute__((alias(#symbol)))
#define __alloc_size(...)      __attribute__((alloc_size(__VA_ARGS__)))
#define __cold                 __attribute__((cold))
#define __const                __attribute__((__const__))
#define __malloc               __attribute__((malloc))
#define __must_check           __attribute__((warn_unused_result))
#define __printf(f, a)         __attribute__((format(printf, f, a)))
#define __packed               __attribute__((packed))
#define __pure                 __attribute__((pure))
#define __unused               __attribute__((unused))
#define __used                 __attribute__((used))
#define __visible              __attribute__((externally_visible))
#define __weak                 __attribute__((weak))
#define always_inline          __attribute__((__always_inline__)) inline
#define noinline               __attribute__((__noinline__))

/* Builtins */
#define likely(e)              __builtin_expect(!!(e), 1)
#define offsetof(type, member) __builtin_offsetof(type, member)
#define unlikely(e)            __builtin_expect(e, 0)
#define unreachable()          __builtin_unreachable()

/* Keywords */
#define alignas                _Alignas
#define alignof                _Alignof
#define asm                    __asm__
#define noreturn               _Noreturn
#define static_assert          _Static_assert

#endif /* COMPILER_H */
