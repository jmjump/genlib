#include "Common.h"

#include "stdarg.h"

int g_debugLevel = 0;

//////////////////////////////////////////////////////////////////////////////////

std::string makeString (const char* fmt, ...)
{
    int size = 512;
    char* buffer = new char[size];
    va_list vl;
    va_start(vl, fmt);
    int nsize = vsnprintf(buffer, size, fmt, vl);
    if (size <= nsize) { //fail delete buffer and try again
        delete[] buffer;
        buffer = new char[nsize+1]; //+1 for /0
        nsize = vsnprintf(buffer, size, fmt, vl);
    }
    std::string ret(buffer);
    va_end(vl);
    delete[] buffer;
    return ret;
}

//////////////////////////////////////////////////////////////////////////////////

std::string classFunction (const char* className, const char* functionName)
{
	return makeString("%s::%s", className, functionName);
}

