//

#ifndef CPP_MEMORY
#define CPP_MEMORY

//
void InitMemory() {
	/*CreatePoolBuffer  (&PoolBufPtr    , PtrSize*1024, PtrSize);
	CreatePoolBuffer  (&PoolBuf32kb   , PageSize, 64);
	CreateStackBuffer (&TexturesBuffer, PageSize);
	CreateStackBuffer (&ObjectsBuffer , PageSize);
	CreateStackBuffer (&LinBuffer     , PageSize);
	CreateLinearBuffer(&DrawDataBuffer, PageSize);
	CreatePoolBuffer  (&PoolBuf32b    , PageSize, 32);
	CreateLinearBuffer(&TablesBuffer  , PageSize);
	//
	CreateLinearBuffer(&LongTimeLifeBuffer, PageSize);*/
}

//
void FreeMemory() {
	/*DestroyPoolBuffer  (&PoolBufPtr);
	DestroyPoolBuffer  (&PoolBuf32kb);
	DestroyStackBuffer (&TexturesBuffer);
	DestroyStackBuffer (&LinBuffer);
	DestroyStackBuffer (&ObjectsBuffer);
	DestroyLinearBuffer(&LongTimeLifeBuffer);
	DestroyLinearBuffer(&DrawDataBuffer);
	DestroyPoolBuffer  (&PoolBuf32b);
	DestroyLinearBuffer(&TablesBuffer);*/
}

#endif
