#ifndef H_LOGGER
#define H_LOGGER

#include <cstdio>
#include <cstring>
#include <cstdarg>

#define LOG_DEBUG(str,...) LoggerWrite("DEBUG " str, ##__VA_ARGS__)
#define LOG_ERROR(str,...) LoggerWrite("ERROR " str, ##__VA_ARGS__)

#define ASSERT(x) assert(x)

void LoggerCreate(const char *fileName);
void LoggerDestroy();
void LoggerWrite(const char *format, ...);

#include "src/Logger.cpp"
#endif
