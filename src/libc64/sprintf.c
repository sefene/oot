#include "libc64/sprintf.h"
#include "stdarg.h"
#include "string.h"
#include "ultra64/xstdio.h"

#if PLATFORM_N64
// Generated by CVS "$Id$" keyword
char sSprintfFileInfo[] = "$Id: sprintf.c,v 1.5 1997/03/19 02:28:53 hayakawa Exp $";
#endif

void* proutSprintf(void* dst, const char* fmt, size_t size) {
    return (char*)memcpy(dst, fmt, size) + size;
}

int vsprintf(char* dst, const char* fmt, va_list args) {
    int ret = _Printf(proutSprintf, dst, fmt, args);

    if (ret > -1) {
        dst[ret] = '\0';
    }
    return ret;
}

int sprintf(char* dst, const char* fmt, ...) {
    int ret;
    va_list args;
    va_start(args, fmt);

    ret = _Printf(proutSprintf, dst, fmt, args);
    if (ret > -1) {
        dst[ret] = '\0';
    }

    va_end(args);

    return ret;
}