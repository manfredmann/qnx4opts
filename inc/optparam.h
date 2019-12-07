#ifndef OPTPARAM_H
#define OPTPARAM_H

#include "optbase.h"

typedef class OptParam OptParam;

typedef WCValOrderedVector<OptParam *> params_vector_t ;

class OptParam {
    public:
        OptParam(void *data, opt_types_t type);
        ~OptParam();

        void    set_data(void *data);
        void *  get_data();

    private:
        void *          data;
        opt_types_t     type;
};


#endif
