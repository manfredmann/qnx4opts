/*
 * Copyright © 2019 Roman Serov
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file or http://www.wtfpl.net/ 
 * for more details.
*/

#include "optparam.h"

OptParam::OptParam(void *data, opt_types_t type) {
    this->data = data;
    this->type = type;
}

OptParam::~OptParam() {
    switch(this->type) {
        case OPT_STRING: {
            delete (String *) this->data;
            break;
        }
        case OPT_INT: {
            delete (int *) this->data;
            break;
        }
        case OPT_UINT: {
            delete (unsigned int *) this->data;
            break;
        }
        case OPT_LONG: {
            delete (long int *) this->data;
            break;
        }
        case OPT_ULONG: {
            delete (unsigned long *) this->data;
            break;
        }
        case OPT_FLOAT: {
            delete (float *) this->data;
            break;
        }
        case OPT_DOUBLE: {
            delete (double *) this->data;
            break;
        }
        case OPT_HEX: {
            delete (unsigned long *) this->data;
            break;
        }
    }
}

void OptParam::set_data(void *data) {
    this->data = data;
}

void *OptParam::get_data() {
    return this->data;
}
