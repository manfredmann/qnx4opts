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
