#include "posix.h"

#ifndef _POSIX_C_SOURCE
char *strdup(const char *src) {
    char *dst = malloc(strlen(src) + 1);
    if (dst != NULL) {
        strcpy(dst, src);
    }
    return dst;
}
#endif
