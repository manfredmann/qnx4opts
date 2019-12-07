#ifndef OPTBASE_H
#define OPTBASE_H

#include <String.h>
#include <wcvector.h>

typedef unsigned char bool;

#define true 1
#define false 0

typedef enum {
    OPT_BOOL,
    OPT_STRING,
    OPT_INT,
    OPT_UINT,
    OPT_LONG,
    OPT_ULONG,
    OPT_FLOAT,
    OPT_DOUBLE,
    OPT_HEX,
    OPT_COUNTER,
} opt_types_t;

#endif