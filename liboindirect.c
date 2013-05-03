/*
  Bugs: avati@gluster.com
*/

#include <dlfcn.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <ctype.h>

#define __cons __attribute__((constructor))

#define TRAP(func, params)                                              \
        evil_##func params;                                             \
        static int (*real_##func) params;                               \
        static void set_real_##func (void) __cons;                      \
        static void set_real_##func (void)                              \
        { real_##func = dlsym (RTLD_NEXT, #func); }                     \
        int __REDIRECT (evil_##func, params, func);                     \
        int evil_##func params


int
TRAP (open, (const char *path, int flags, mode_t mode))
{
        if (flags & O_DIRECT) 
                flags &= ~O_DIRECT;

        return real_open (path, flags, mode);
}


int
TRAP (open64, (const char *path, int flags, mode_t mode))
{
        if (flags & O_DIRECT) 
                flags &= ~O_DIRECT;

        return real_open64 (path, flags, mode);
}


int
TRAP (openat, (int dirfd, const char *path, int flags, mode_t mode))
{
        if (flags & O_DIRECT) 
                flags &= ~O_DIRECT;

        return real_openat (dirfd, path, flags, mode);

}
