// Copyright (c) 2025 Arye Gross
// SPDX-License-Identifier: Apache-2.0

#define _GNU_SOURCE
#include <stdint.h>
#include <time.h>
#include "cmsis_os2.h"
#include "cmsisPosix_Config.h"

static_assert(CP_CONFIG_NANO_IN_TICK <= 1000000000, "Having tick greater than second may cause math overlapping");
// Converts CMSIS timeout to absolute timespec
void cp_timeoutToTimespec(uint32_t timeout, struct timespec *ts)
{
    // Get current time
    clock_gettime(CLOCK_REALTIME, ts);  // Or CLOCK_MONOTONIC depending on use

    // Total nanoseconds to wait
    int64_t total_nanos = timeout * (int64_t)CP_CONFIG_NANO_IN_TICK;

    // Add to current time
    ts->tv_sec += total_nanos / 1000000000L;
    ts->tv_nsec += total_nanos % 1000000000L;

    // Normalize nsec if it overflows
    if (ts->tv_nsec >= 1000000000L)
    {
        ts->tv_sec += ts->tv_nsec / 1000000000L;
        ts->tv_nsec %= 1000000000L;
    }
}