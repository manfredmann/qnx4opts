/*
 * Copyright © 2019 Roman Serov
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file or http://www.wtfpl.net/ 
 * for more details.
*/

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
