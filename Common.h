#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>

#include <string>

#include <sys/time.h>

#define MIN(a,b)		(a) < (b) ? (a) : (b)
#define MAX(a,b)		(a) > (b) ? (a) : (b)

#define ArraySize(a)	(sizeof(a) / sizeof((a)[0]))

extern int				g_debugLevel;

#define TRACE(level, ...) 											\
	if (g_debugLevel >= (level)) {									\
		struct timeval now;											\
		gettimeofday(&now, NULL);									\
		struct tm tmNow;											\
		localtime_r(&now.tv_sec, &tmNow);							\
		char timeStr[64];											\
		strftime(timeStr, sizeof(timeStr), "%b %d %T", &tmNow);		\
		printf("%s.%03d: ", timeStr, now.tv_usec / 1000);			\
		printf(__VA_ARGS__);										\
	}

#define FATAL(...)                                                  \
    {                                                               \
        TRACE(0, __VA_ARGS__);                                      \
        exit(1);                                                    \
    }

#define THROW(...)													\
	{																\
		throw makeString(__VA_ARGS__);								\
	}

//////////////////////////////////////////////////////////////////////////////////
// Forward references
extern std::string makeString (const char* fmt, ...);

extern std::string classFunction (const char* className, const char* functionName);

#include <typeinfo>
#define __CLASSNAME__   typeid(*this).name()

#define __CLASSFUNCTION__   classFunction(__CLASSNAME__, __FUNCTION__).c_str()

//////////////////////////////////////////////////////////////////////////////////

#endif
