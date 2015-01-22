// Author Kabilin Andrey 2013
// Basic memory allocators

#ifndef H_ALLOCA
#define H_ALLOCA

#include "stdint.h"
#include <sys/mman.h>

//
typedef struct _AllocaBuffer {
	uint8_t  *mem;
	uint32_t  totalSize;
} AllocaBuffer;
//
typedef struct _LinearBuffer : public AllocaBuffer {
	
	uint32_t  offset;
} LinearBuffer;

//
typedef uint32_t StackHandle;
typedef struct _StackBuffer : public AllocaBuffer {
	uint32_t  offset;
} StackBuffer;

//
typedef struct _StackBuffer2 : public AllocaBuffer {
	uint32_t  offset1;
	uint32_t  offset2;
} StackBuffer2;

//
typedef struct _PoolBuffer : public AllocaBuffer {
	uint8_t  *head;
	uint32_t  chunkSize;
} PoolBuffer;

//
typedef struct _RingBuffer : public AllocaBuffer {
	
} RingBuffer;

#include "Alloca.cpp"
#endif
