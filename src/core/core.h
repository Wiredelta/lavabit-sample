
/**
 * @file /magma/core/core.h
 *
 * @brief	A collection of types, declarations and includes needed when accessing the core module and the type definitions needed to parse the header files that follow.
 */

#ifndef MAGMA_CORE_H
#define MAGMA_CORE_H

//#include <gnu/libc-version.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <signal.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <pthread.h>
#include <fcntl.h>
#include <math.h>
#include <semaphore.h>
#include <dirent.h>
#include <limits.h>
#include <ftw.h>
#include <time.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <sys/mman.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// RFC 2181 specifies a maximum legal length of 253 characters for a dotted domain name. Individual levels of the overall name
// may contain a maximum of 63 characters.
#define MAGMA_HOSTNAME_MAX _POSIX_HOST_NAME_MAX

// The maximum number of characters in a file path... currently 4096.
#define MAGMA_FILEPATH_MAX PATH_MAX

// The maximum number of characters in a file name... currently 255.
#define MAGMA_FILENAME_MAX NAME_MAX

// The amount of memory allocated by default to hold the stack for spawned threads.
#define MAGMA_THREAD_STACK_SIZE 1048576

// The size of the thread local buffer.
#define MAGMA_THREAD_BUFFER_SIZE 1024

// The maximum number of worker threads allowed, even if the system limit is higher.
#define MAGMA_WORKER_THREAD_LIMIT 16384

// The amount of data used to seed the random number generator.
#define MAGMA_CRYPTOGRAPHY_SEED_SIZE 256

/**
 * The type definitions used by Magma that are not defined by the system headers.
 * The bool type requires the inclusion of stdbool.h and the use of the C99.
 */

/// @typedef bool_t
#ifndef __bool_t_defined
#ifdef __bool_true_false_are_defined
typedef bool bool_t;
#else
typedef char bool_t;
#endif
#define __bool_t_defined
#endif

/// @typedef chr_t
#ifndef __chr_t_defined
typedef char chr_t;
#define __chr_t_defined
#endif

/// @typedef uchr_t
#ifndef __u_chr_t_defined
typedef unsigned char uchr_t;
#define __u_chr_t_defined
#endif

/// @typedef byte_t
#ifndef __byte_t_defined
typedef unsigned char byte_t;
#define __byte_t_defined
#endif

/// @typedef int_t
#ifndef __int_t_defined
typedef int32_t int_t;
#define __int_t_defined
#endif

/// @typedef uint_t
#ifndef __uint_t_defined
typedef uint32_t uint_t;
#define __uint_t_defined
#endif

/// @typedef int24_t
#ifndef __int24_t_defined
typedef struct __attribute__ ((packed)) {
	uint8_t byte0;
	uint8_t byte1;
	int8_t byte2;
} __int24_t;
typedef __int24_t int24_t;
#define __int24_t_defined
#endif

/// @typedef uint24_t
#ifndef __uint24_t_defined
typedef struct __attribute__ ((packed)) {
	uint8_t byte0;
	uint8_t byte1;
	uint8_t byte2;
} __uint24_t;
typedef __uint24_t uint24_t;
#define __uint24_t_defined
#endif

#ifndef INT24_MIN
#define INT24_MIN (-8388607)
#endif

#ifndef INT24_MAX
#define INT24_MAX (8388607)
#endif

#ifndef UINT24_MIN
#define UINT24_MIN (0)
#endif

#ifndef UINT24_MAX
#define UINT24_MAX (16777215)
#endif

/*

Should we ever need to create a 128 bit integer on a 64 bit system, GCC 3.1 and higher will allow it. Note that
this will not work on 32 bit systems, and don't forget to add the 128 bit type to the M_TYPE enumerator.

# if defined(__GNUC__) && (__GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 6)) && (WORDSIZE == 64)
typedef __uint128_t uint128_t;
typedef __int128_t int128_t;
# else
#  error "A 64 bit system, and GCC 4.6.0 or later is required to define the 128 bit integer types."
# endif

*/

/**
 * Different types used throughout.
 */
typedef enum {
	M_TYPE_EMPTY = 0,
	M_TYPE_MULTI = 1,   //!< M_TYPE_MULTI is multi_t
	M_TYPE_ENUM,		//!< M_TYPE_ENUM is enum
	M_TYPE_BOOLEAN, //!< M_TYPE_BOOLEAN is bool_t
	M_TYPE_BLOCK,   //!< M_TYPE_BLOCK is void pointer
	M_TYPE_NULLER,  //!< M_TYPE_NULLER is char pointer
	M_TYPE_PLACER,  //!< M_TYPE_PLACER is placer_t struct
	M_TYPE_STRINGER,//!< M_TYPE_STRINGER is stringer_t pointer
	M_TYPE_INT8,    //!< M_TYPE_INT8 is int8_t
	M_TYPE_INT16,   //!< M_TYPE_INT16 is int16_t
	M_TYPE_INT32,   //!< M_TYPE_INT32 is int32_t
	M_TYPE_INT64,   //!< M_TYPE_INT64 is int64_t
	M_TYPE_UINT8,   //!< M_TYPE_UINT8 is uint8_t
	M_TYPE_UINT16,  //!< M_TYPE_UINT16 is uint16_t
	M_TYPE_UINT32,  //!< M_TYPE_UINT32 is uint32_t
	M_TYPE_UINT64,  //!< M_TYPE_UINT64 is uint64_t
	M_TYPE_FLOAT,   //!< M_TYPE_FLOAT is float
	M_TYPE_DOUBLE   //!< M_TYPE_DOUBLE is double
} M_TYPE;

enum {
	EMPTY = 0
};

/************ TYPE ************/
char * type(M_TYPE type);
/************ TYPE ************/

#include "memory/memory.h"
#include "strings/strings.h"
#include "classify/classify.h"
#include "encodings/encodings.h"
#include "thread/thread.h"
#include "buckets/buckets.h"
#include "checksum/checksum.h"
#include "host/host.h"
#include "status/status.h"
#include "context/context.h"
#include "indexes/indexes.h"
#include "parsers/parsers.h"
#include "compare/compare.h"
#include "parsers/parsers.h"
#include "core_log.h"

typedef struct {

	struct {
		bool_t output_config; /* Dump the configuration to the log file. */
		bool_t output_resource_limits; /* Should attempts to increase system limits trigger an error. */

		// LOW: Filenames are limited to 255 characters, but file paths can be up to 4096. As such we should probably be storing this using a block of memory off the heap.
		chr_t file[MAGMA_FILEPATH_MAX + 1]; /* Path to the magmad.config file. */
	} config;

	struct {
		struct {
			bool_t enable; /* Should the secure memory sub-system be enabled. */
			uint64_t length; /* The size of the secure memory pool. The pool must fit within any memory locking limits. */
		} memory;

		stringer_t *salt; /* The string added to hash operations to improve security. */
		stringer_t *links; /* The string used to encrypt links that reflect back to the daemon. */
		stringer_t *sessions; /* The string used to encrypt session tokens. */
	} secure;

	struct {
		bool_t daemonize; /* Spawn a daemon process and release the console session. */
		char * root_directory; /* Change the root path to the provided value. */
		char * impersonate_user; /* Change the effective user account of the process to the user provided. */
		bool_t increase_resource_limits; /* Attempt to increase system limits. */
		uint32_t thread_stack_size; /* How much memory should be allocated for thread stacks? */
		uint32_t worker_threads; /* How many worker threads should we spawn? */
		uint32_t network_buffer; /* The size of the network buffer? */

		bool_t enable_core_dumps; /* Should fatal errors leave behind a core dump. */
		uint64_t core_dump_size_limit; /* If core dumps are enabled, what size should they be limited too. */

		stringer_t *domain; /* The default domain name used in new user email addresses and for unqualified login names. */
	} system;

	chr_t * spool; /* The spool directory. */
	int_t page_length; /* The memory page size. This value is used to align memory mapped files to page boundaries. */
} magma_core_t;
extern magma_core_t magma_core;

extern __thread char threadBuffer[1024];
#define bufptr (char *)&(threadBuffer)
#define buflen sizeof(threadBuffer)

#endif

