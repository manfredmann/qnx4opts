/*
 * Copyright © 2019-2020 Roman Serov
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file or http://www.wtfpl.net/ 
 * for more details.
*/

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
