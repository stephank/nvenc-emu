#ifndef __NEE_UTIL_H__
#define __NEE_UTIL_H__

#include <stdio.h>

// Whether to enable trace logging.
#define NEE_TRACE_ENABLED 1

// Simple logging helper.
#define NEE_LOG(fmt, ...) \
  fprintf(stderr, "NVENC-EMU: " fmt "\n", ##__VA_ARGS__)

// Trace logging helper.
#ifdef NEE_TRACE_ENABLED
# define NEE_TRACE NEE_LOG
#else
# define NEE_TRACE(...)
#endif

// Log the result of one of our emulated functions.
#define NEE_TRACE_FUNC(fmt, ...) \
  NEE_TRACE("%s" fmt, __func__, ##__VA_ARGS__)

// Check a struct boolean field and return a static string.
// Useful for logging a bunch of boolean fields.
#define NEE_STR_FLAG(st, field) \
  ((st)->field ? " " #field : "")

#endif /* __NEE_UTIL_H__ */
