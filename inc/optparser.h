/*
 * Copyright © 2019 Roman Serov
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file or http://www.wtfpl.net/ 
 * for more details.
*/

#ifndef OPTPARSER_H
#define OPTPARSER_H

#include <stdlib.h>
#include <string.h>
#include <String.h>

#include "optparam.h"

typedef struct {
    String          name;
    String          name_long;
    String          desc;
    opt_types_t     type;
    bool            req;
    bool            present;
    bool            multiple;
    params_vector_t params;
} opt_t;

typedef enum {
    OPT_RAW_SHORT,
    OPT_RAW_LONG,
    OPT_RAW_SHORTWPARAM,
    OPT_RAW_NO,
} opt_raw_type_t;

typedef WCValOrderedVector<opt_t *> opt_vector_t ;

class OptParser_Ex {
    public:
        OptParser_Ex(String error) {
            this->error = error;
        }

        String what() {
            return this->error;
        }

        const char *what_c() {
            return (const char *) this->error;
        }

    private:
        String error;

};

class OptParser {
    public:
        OptParser(String app_name, String app_desc);
        ~OptParser();
        
        void            add(String opt_name, String opt_desc);
        void            add(String opt_name, String opt_desc, opt_types_t opt_type, bool req, bool multiple);

        unsigned int    find(String opt_name);
        bool            parse(int argc, char **argv, bool help);
        void            print_help();

        String          get_string(String opt_name, unsigned int index = 0);
        int             get_int(String opt_name, unsigned int index = 0);
        unsigned int    get_uint(String opt_name, unsigned int index = 0);
        long int        get_long(String opt_name, unsigned int index = 0);
        unsigned long   get_ulong(String opt_name, unsigned int index = 0);
        float           get_float(String opt_name, unsigned int index = 0);
        double          get_double(String opt_name, unsigned int index = 0);
        unsigned long   get_hex(String opt_name, unsigned int index = 0);
        unsigned int    get_counter(String opt_name);

    private:
        String          app_name;
        String          app_desc;
        opt_vector_t    opt_list;

        int             find_eq(const char *str);
        opt_raw_type_t  opt_raw_type(const char *str);
        bool            is_negative_decimal(const char *str);
        bool            is_decimal(const char *str);
        bool            is_double(const char *str);
        bool            is_negative_double(const char *str);

        void            opt_add_param(opt_t *opt, void *param, opt_types_t type);
        void            opt_add_param(opt_t *opt, String param);
        void            opt_add_param(opt_t *opt, int param);
        void            opt_add_param(opt_t *opt, unsigned int param);
        void            opt_add_param(opt_t *opt, float param);
        void            opt_add_param(opt_t *opt, double param);
        void            opt_add_param(opt_t *opt, long param);
        void            opt_add_param(opt_t *opt, unsigned long param);

        opt_t *         opt_find(String opt_name);
        void *          opt_find_param(opt_t *opt, unsigned int index);
};

#endif