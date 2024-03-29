#ifndef DEVICE_ACC_H
#define DEVICE_ACC_H

#include<stdio.h>
#include<stdlib.h>
#include <openacc.h>

#define GPUMM_ALLOC(ptr, size) \
{\
    ptr = acc_malloc(size); \
    if ( ptr == NULL ) \
    { \
        fprintf(stderr, "error: 'malloc ptr is null' at %s:%d\n", __FILE__, __LINE__); \
        exit(EXIT_FAILURE);\
    } \
}

#define GPUMM_MEMCPY_HtD(dst,src,size) \
{\
    acc_memcpy_to_device(dst, src, size); \
}

#define GPUMM_MEMCPY_DtH(dst,src,size) \
{\
    acc_memcpy_from_device(dst,src,size); \
}

#define GPUMM_FREE(ptr) \
{\
    acc_free(ptr); \
}
#endif