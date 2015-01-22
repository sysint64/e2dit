#ifndef H_MEMORY
#define H_MEMORY

//
#ifdef PORTABLE_64_BIT 
	#define PtrSize 8
#else
	#define PtrSize 4
#endif
#define PageSize 65536

// Pools
PoolBuffer   PoolBufPtr;
PoolBuffer   PoolBuf32b;
PoolBuffer   PoolBuf32kb;
StackBuffer  TexturesBuffer;
StackBuffer  ObjectsBuffer;
StackBuffer  LinBuffer;
LinearBuffer LongTimeLifeBuffer;
LinearBuffer DrawDataBuffer;
LinearBuffer TablesBuffer;
//
#include "Memory.cpp"
#endif
