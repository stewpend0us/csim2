#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr,\
        "[DEBUG] %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define get_errno() (errno == 0 ? "None" : strerror(errno))

// log functions
#define log_err(M, ...) fprintf(stderr,\
        "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, get_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr,\
        "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, get_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr,\
        "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

// following functions require "goto error:" defined in the function
// just like assert except it doesn't get compiled away and jumps to error:
#define check(A, M, ...) if(!(A)){\
        log_err(M, ##__VA_ARGS__); errno=0; goto error; }

// intended for branches that shouldn't be reached
#define sentinel(M, ...)  {\
        log_err(M, ##__VA_ARGS__); errno=0; goto error; }

// use this after calling malloc
#define check_mem(A) check((A), "Out of memory.")

// this seems like a bad idea? use log_err or debug instead
#define check_debug(A, M, ...) if(!(A)) {\
         debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif
