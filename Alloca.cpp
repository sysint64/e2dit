// Author Kabilin Andrey 2013
// Basic memory allocators

#ifndef CPP_ALLOCA
#define CPP_ALLOCA

/*!< Linear Allocator >!*/
//
inline size_t align(size_t x, size_t a) { return ((x-1) | (a-1)) + 1; }
//
inline void CreateLinearBuffer(LinearBuffer *buf, uint32_t size) {
	//
	buf->totalSize = size;
	buf->offset = 0;
	buf->mem = (uint8_t*) malloc (size);
}
//
inline void DestroyLinearBuffer(LinearBuffer *buf) {
	free (buf->mem);
}
//
inline void FreeLinearBuffer(LinearBuffer *buf) {
	buf->offset = 0;
}
//

/* non-aligned allocation from linear buffer. */
inline void* LinearAlloc(LinearBuffer *buf, uint32_t size, bool zero = false, uint32_t alignment = PtrSize) {
	//size += alignment-1;
	
	uint32_t newOffset = buf->offset+size;//+align(size, alignment);
	
	if (newOffset <= buf->totalSize) {
		void* ptr = buf->mem + buf->offset;

		//if (zero) memset (ptr, 0, size);
		
		buf->offset = newOffset;
		return ptr;
	}
	
	return 0; /* out of memory */
}


/*!< Stack Allocator >!*/
//
inline void CreateStackBuffer(StackBuffer *buf, uint32_t size) {
	buf->totalSize = size;
	buf->offset = 0;
	buf->mem = (uint8_t*)mmap(0, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

//
inline void DestroyStackBuffer(StackBuffer *buf) {
	munmap(buf->mem, buf->totalSize);
}
//
inline void FreeStackBuffer(StackBuffer *buf) {
	buf->offset = 0;
}

//
inline void* StackAlloc(StackBuffer *buf, uint32_t size, StackHandle* handle) {
	//
	const uint32_t currOffset = buf->offset;

	if (currOffset + size <= buf->totalSize) {
		uint8_t* ptr = buf->mem + currOffset;
		buf->offset += size;
	
		if (handle) *handle = currOffset;
		return (void*)ptr;
	}
	
	return 0;
}

//
inline void StackSet(StackBuffer* buf, StackHandle handle) {
	//
	buf->offset = handle;
}

//
/*!< Stack with two-forked allocation >!*/
//
inline void CreateStackBuffer2(StackBuffer2 *buf, uint32_t size) {
	//
	buf->totalSize = size;
	buf->offset1 = 0;
	buf->offset2 = size;
	buf->mem = (uint8_t*)mmap(0, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

//
inline void DestroyStackBuffer2(StackBuffer2 *buf) {
	munmap(buf->mem, buf->totalSize);
}
//
inline void FreeStackBuffer2(StackBuffer2 *buf) {
	buf->offset1 = 0;
	buf->offset2 = 0;
}
//
inline void FreeStackBuffer2Left(StackBuffer2 *buf) {
	buf->offset1 = 0;
}
//
inline void FreeStackBuffer2Right(StackBuffer2 *buf) {
	buf->offset2 = 0;
}

//
inline void* Stack2AllocLeft(StackBuffer2 *buf, uint32_t size, StackHandle *handle) {
	//
	const uint32_t currOffset = buf->offset1;

	if (currOffset + size <= buf->offset2) {
		uint8_t* ptr = buf->mem+currOffset;
		buf->offset1 += size;
	
		if (handle) *handle = currOffset;
		return (void*)ptr;
	}
	
	return 0;
}

//
inline void* Stack2AllocRight(StackBuffer2 *buf, uint32_t size, StackHandle *handle) {
	//	
	const uint32_t currOffset = buf->offset2;

	if (currOffset-size >= buf->offset1) {
		uint8_t* ptr = buf->mem+buf->totalSize-currOffset;
		buf->offset2 -= size;
	
		if (handle) *handle = currOffset;
		return (void*)ptr;
	}
	
	return 0;
}

//
inline void StackSetLeft(StackBuffer2* buf, StackHandle handle) {
	//
	buf->offset1 = handle;
}

//
inline void StackSetRight(StackBuffer2* buf, StackHandle handle) {
	//
	buf->offset2 = handle;
}

//
/*!< Pool Allocator >!*/
//
inline void CreatePoolBuffer(PoolBuffer *buf, uint32_t size, uint32_t chunkSize) {
	//
	buf->totalSize = size;
	buf->chunkSize = chunkSize;
	buf->mem = (uint8_t*)mmap(0, size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	//
	const uint32_t chunkCount = (size / chunkSize) - 1;
	
	for (uint32_t i = 0; i < chunkCount; ++i) {
		uint8_t* currChunk = buf->mem + (chunkSize*i);
		*((uint8_t**)currChunk) = currChunk + chunkSize;
	}
	
	*((uint8_t**)&buf->mem[chunkCount * chunkSize]) = 0;
	buf->head = buf->mem;
	//
}

//
inline void DestroyPoolBuffer(PoolBuffer *buf) {
	munmap(buf->mem, buf->totalSize);
}

//
inline void* PoolAlloc(PoolBuffer* buf) {
	//
	uint8_t* currPtr = buf->head;
	buf->head = (*((uint8_t**)(buf->head)));
	return currPtr;
}

//
inline void* PoolZAlloc(PoolBuffer* buf) {
	//
	uint8_t* currPtr = buf->head;
	buf->head = (*((uint8_t**)(buf->head)));
	
	// Zeroinitializer
	for (int i = 0; i < buf->chunkSize; i++) {
		currPtr[i] = 0;
	}
	
	return currPtr;
}

inline void PoolFree(PoolBuffer* buf, void* ptr) {
	//
	*((uint8_t**)ptr) = buf->head;
	buf->head = (uint8_t*)ptr;
}

#endif
