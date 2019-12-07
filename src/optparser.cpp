/*
 * Copyright © 2019 Roman Serov
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file or http://www.wtfpl.net/ 
 * for more details.
*/

#include "optparser.h"
#include "re.h"

#include <stdio.h>
#include <math.h>

OptParser::OptParser(String app_name, String app_desc) {
    this->app_desc = app_desc;
    this->app_name = app_name;
}

OptParser::~OptParser() {
    for (int i = 0; i < opt_list.entries(); ++i) {
        for (int j = 0; j < opt_list[i]->params.entries(); ++j) {
            delete opt_list[i]->params[j];
        }

        opt_list[i]->params.clear();

        delete opt_list[i];
    }

    opt_list.clear();
}

void OptParser::add(String opt_name, String opt_desc) {
    if (opt_name.length() == 0) {
        throw OptParser_Ex("Option name couldn't be empty");
    }

    opt_t *opt = new opt_t;

    int sep = opt_name.index(",");

    if (sep == -1) {
        if (opt_name.length() == 1) {
            opt->name       = opt_name;
            opt->name_long  = "";
        } else {
            opt->name_long  = opt_name;
            opt->name       = "";
        }
    } else {
        opt->name       = String(opt_name, 0, sep);
        opt->name_long  = String(opt_name, sep + 1);
    }

    opt->desc       = opt_desc;
    opt->type       = OPT_BOOL;
    opt->req        = false;
    opt->present    = false;
    opt->multiple   = false;

    for (unsigned int i = 0; i < this->opt_list.entries(); ++i) {
        opt_t *e_opt = this->opt_list[i];

        if (opt->name.length() != 0 && e_opt->name == opt->name) {
            throw OptParser_Ex("Option already exists");            
        }

        if (opt->name_long.length() != 0 && e_opt->name_long == opt->name_long) {
            throw OptParser_Ex("Option already exists");            
        }
    }

    this->opt_list.append(opt);
}

void OptParser::add(String opt_name, String opt_desc, opt_types_t opt_type, bool req, bool multiple) {
    if (opt_name.length() == 0) {
        throw OptParser_Ex("Option name couldn't be empty");
    }

    opt_t *opt = new opt_t;

    int sep = opt_name.index(",");

    if (opt_type == OPT_COUNTER) {
        if (opt_name.length() != 1) {
            throw OptParser_Ex("Option name length must be 1 for OPT_COUNTER type");
        }

        opt->name       = opt_name[0];
        opt->name_long  = "";
    } else {
        if (sep == -1) {
            if (opt_name.length() == 1) {
                opt->name       = opt_name;
                opt->name_long  = "";
            } else {
                opt->name_long  = opt_name;
                opt->name       = "";
            }
        } else {
            opt->name       = String(opt_name, 0, sep);
            opt->name_long  = String(opt_name, sep + 1);
        }
    }

    opt->desc       = String(opt_desc);
    opt->type       = opt_type;
    opt->req        = req;
    opt->present    = false;
    opt->multiple   = multiple;

    bool is_found = false;

    for (unsigned int i = 0; i < this->opt_list.entries(); ++i) {
        opt_t *e_opt = this->opt_list[i];

        if (opt->name.length() != 0 && e_opt->name == opt->name) {
            throw OptParser_Ex("Option already exists");            
        }

        if (opt->name_long.length() != 0 && e_opt->name_long == opt->name_long) {
            throw OptParser_Ex("Option already exists");            
        }
    }

    this->opt_list.append(opt);
}

arg_type_t OptParser::is_arg(const char *str) {
    int index = re_match("^-[^-]$", str);

    if (index != -1) {
        return ARG_SHORT;
    }

    index = re_match("^-[^-]\\=.+$", str);

    if (index != -1) {
        return ARG_SHORT;
    }

    index = re_match("^-[^-].+$", str);

    if (index != -1) {
        return ARG_SHORTWPARAM;
    }

    index = re_match("^--.+$", str);

    if (index != -1) {
        return ARG_LONG;
    }

    return ARG_NO;
}

int OptParser::find_eq(const char *str) {
    return re_match("\\=", str) + 1;
}

bool OptParser::is_negatvie_decimal(const char *str) {
    int index = re_match("^-\\d+$", str);

    return index + 1;
}

bool OptParser::is_decimal(const char *str) {
    int index = re_match("^\\d+$", str);

    return index + 1;
}

bool OptParser::is_double(const char *str) {
    int index = re_match("^\\d+\\.\\d+$", str);

    return index + 1;   
}

bool OptParser::is_negative_double(const char *str) {
    int index = re_match("^-\\d+\\.\\d+$", str);

    return index + 1;   
}

bool OptParser::parse(int argc, char **argv, bool help) {
    unsigned int argi = 1;
    unsigned int i;

    if (argc < 2) {
        bool is_req = false;

        for (i = 0; i < this->opt_list.entries(); ++i) {
            opt_t *opt = this->opt_list[i];

            if (opt->req) {
                is_req = true;
            }
        }

        if (is_req) {
            goto on_help;
        } else {
            return true;
        }
    }

    while (argi != argc) {
        String      val(argv[argi]);
        String      name;
        String      param;
        arg_type_t  type = is_arg(argv[argi]);

        switch(type) {
            case ARG_SHORT: {
                name = String(val, 1, 1);

                if (argi + 1 != argc) {
                    if (is_arg(argv[argi + 1]) == ARG_NO) {
                        param = String(argv[argi + 1]);
                    }
                }

                int eq = find_eq(val);

                if (eq) {
                    param = String(val, eq);
                }

                break;
            }
            case ARG_SHORTWPARAM: {
                name = String(val, 1, 1);
                param = String(val, 2);

                break;
            }
            case ARG_LONG: {
                name = String(val, 2);

                if (argi + 1 != argc) {
                    if (is_arg(argv[argi + 1]) == ARG_NO) {
                        param = String(argv[argi + 1]);
                    }
                }

                int eq = find_eq(name);

                if (eq) {
                    param = String(name, eq);
                    name = String(name, 0, eq - 1);
                }

                break;
            }
            default: {
                ++argi;
                continue;
            }
        }

        for (i = 0; i < this->opt_list.entries(); ++i) {
            opt_t *opt = this->opt_list[i];

            switch(type) {
                case ARG_SHORT:
                case ARG_SHORTWPARAM: {
                    if (opt->name != name) {
                        continue;
                    }

                    if (opt->type == OPT_BOOL) {
                        opt->present = true;
                        continue;
                    }

                    if (opt->present && !opt->multiple) {
                        goto on_help;
                    }

                    break;
                }
                case ARG_LONG: {
                    if (opt->name_long != name) {
                        continue;
                    }

                    if (opt->type == OPT_BOOL) {
                        opt->present = true;
                        continue;
                    }

                    if (opt->present && !opt->multiple) {
                        goto on_help;
                    }

                    break;
                }
            }

            if (param.length() == 0 && opt->type != OPT_COUNTER) {
                goto on_help;
            }

            switch(opt->type) {
                case OPT_COUNTER: {
                    char param_char = name[0];
                    bool is_counter = true;

                    for (int j = 0; j < param.length(); ++j) {
                        if (param[j] != param_char) {
                            is_counter = false;
                            break;
                        }
                    }

                    if (!is_counter) {
                        goto on_help;
                    }

                    opt->present = true;

                    unsigned int raw_param = param.length() + 1;

                    opt_add_param(opt, raw_param);

                    break;
                }
                case OPT_STRING: {
                    opt_add_param(opt, param);

                    break;
                }
                case OPT_INT: {
                    if ((!is_negatvie_decimal(param)) && (!is_decimal(param))) {
                        goto on_help;
                    }

                    int raw_param = atoi((const char *) param);

                    opt_add_param(opt, raw_param);

                    break;
                }
                case OPT_UINT: {
                    if (is_negatvie_decimal(param) || !is_decimal(param)) {
                        goto on_help;
                    }

                    unsigned int raw_param = strtoul((const char *) param, NULL, 10);

                    opt_add_param(opt, raw_param);

                    break;
                }
                case OPT_LONG: {
                    if ((!is_negatvie_decimal(param)) && (!is_decimal(param))) {
                        goto on_help;
                    }

                    long int raw_param = strtol((const char *) param, NULL, 10);

                    opt_add_param(opt, raw_param);

                    break;
                }
                case OPT_ULONG: {
                    if (is_negatvie_decimal(param) || !is_decimal(param)) {
                        goto on_help;
                    }

                    unsigned long raw_param = strtoul((const char *) param, NULL, 10);

                    opt_add_param(opt, raw_param);

                    break;
                }
                case OPT_FLOAT: {
                    float raw_param = atof((const char *) param);

                    opt_add_param(opt, raw_param);

                    break;
                }
                case OPT_DOUBLE: {
                    if (!is_double(param) && !is_negative_double(param) && !is_decimal(param) && !is_negatvie_decimal(param)) {
                        goto on_help;
                    }

                    double raw_param = strtod((const char *) param, NULL);

                    opt_add_param(opt, raw_param);

                    break;
                }
                case OPT_HEX: {
                    unsigned long raw_param = strtoul((const char *) param, NULL, 16);

                    opt_add_param(opt, raw_param);

                    break;
                }
            }

            opt->present = true;
        }

        ++argi;
    }

    for (i = 0; i < this->opt_list.entries(); ++i) {
        opt_t *opt = this->opt_list[i];

        if (opt->req && !opt->present) {
            goto on_help;
        }
    }

    return true;

on_help:
    if (help) {
        this->print_help();
    }

    return false;
}

unsigned int OptParser::find(String opt_name) {
    for (unsigned int i = 0; i < this->opt_list.entries(); ++i) {
        opt_t *opt = this->opt_list[i];

        if (opt_name.length() > 1) {
            if (opt->name_long == opt_name && opt->present) {

                if (opt->type == OPT_BOOL) {
                    return 1;
                }

                return opt->params.entries();
            }
        } else {
            if (opt->name == opt_name && opt->present) {

                if (opt->type == OPT_BOOL) {
                    return 1;
                }

                return opt->params.entries();
            }
        }
    }

    return 0;
}

void OptParser::print_help() {
    unsigned int size = this->opt_list.entries();

    printf("%s - %s\n\n", (const char *) this->app_name, (const char *) this->app_desc);

    int max_len = 0;
    unsigned int i;

    for (i = 0; i < size; ++i) {
        opt_t *opt = this->opt_list[i];

        int opt_len = 0;

        if (opt->name.length() != 0) {
            opt_len += opt->name.length() + 1;
        }

        if (opt->name_long.length() != 0) {
            opt_len += opt->name_long.length() + 2;
        }

        if (max_len < opt_len) {
            max_len = opt_len;
        }
    }

    for (i = 0; i < size; ++i) {
        opt_t *opt = this->opt_list[i];     

        int opt_len = 0;

        printf("  ");

        if (opt->name.length() != 0) {
            opt_len += opt->name.length() + 1;

            printf("-%s", (const char *) opt->name);
        }

        if (opt->name_long.length() != 0) {
            if (opt->name.length() != 0) {
                printf(", ");
                opt_len += 2;
            }

            opt_len += opt->name_long.length() + 2;

            printf("--%s", (const char *) opt->name_long);
        }

        for (int j = 0; j < max_len - opt_len + 4; ++j) {
            printf(" ");
        }

        switch(opt->type) {
            case OPT_COUNTER:
            case OPT_BOOL: {
                printf("        ");
                break;
            }
            case OPT_INT: {
                printf("<int>   ");
                break;
            }
            case OPT_UINT: {
                printf("<uint>  ");
                break;
            }
            case OPT_LONG: {
                printf("<long>  ");
                break;
            }
            case OPT_ULONG: {
                printf("<ulong> ");
                break;
            }
            case OPT_FLOAT: {
                printf("<float> ");
                break;
            }
            case OPT_DOUBLE: {
                printf("<double>");
                break;
            }
            case OPT_HEX: {
                printf("<hex>   ");
                break;
            }
            case OPT_STRING: {
                printf("<string>");
                break;
            }
        }

        printf(" - %s\n", (const char *)opt->desc);
    }
}

opt_t *OptParser::opt_find(String opt_name) {
    for (unsigned int i = 0; i < this->opt_list.entries(); ++i) {
        opt_t *opt = this->opt_list[i];

        if (opt_name.length() > 1) {
            if (opt->name_long == opt_name && opt->present) {
                return opt;
            }
        } else {
            if (opt->name == opt_name && opt->present) {
                return opt;
            }
        }
    }

    throw OptParser_Ex("Couldn't find option");
}

void *OptParser::opt_find_param(opt_t *opt, unsigned int index) {
    if (index >= opt->params.entries()) {
        throw OptParser_Ex("Index out of range");
    }

    return opt->params[index]->get_data();
}

void OptParser::opt_add_param(opt_t *opt, void *data, opt_types_t type) {
    OptParam *param = new OptParam(data, type);

    opt->params.append(param);
}

void OptParser::opt_add_param(opt_t *opt, String data) {
    String *str = new String(data);

    opt_add_param(opt, (void *)str, OPT_STRING);
}

void OptParser::opt_add_param(opt_t *opt, int data) {
    int *i = new int;

    *i = data;

    opt_add_param(opt, (void *)i, OPT_INT);
}

void OptParser::opt_add_param(opt_t *opt, unsigned int data) {
    unsigned int *ui = new unsigned int;

    *ui = data;

    opt_add_param(opt, (void *)ui, OPT_UINT);
}

void OptParser::opt_add_param(opt_t *opt, float data) {
    float *f = new float;

    *f = data;

    opt_add_param(opt, (void *)f, OPT_FLOAT);
}

void OptParser::opt_add_param(opt_t *opt, double data) {
    double *d = new double;

    *d = data;

    opt_add_param(opt, (void *)d, OPT_DOUBLE);
}

void OptParser::opt_add_param(opt_t *opt, long data) {
    long *l = new long;

    *l = data;

    opt_add_param(opt, (void *)l, OPT_LONG);
}

void OptParser::opt_add_param(opt_t *opt, unsigned long data) {
    unsigned long *ul = new unsigned long;

    *ul = data;

    opt_add_param(opt, (void *)ul, OPT_ULONG);
}

String OptParser::get_string(String opt_name, unsigned int index) {
    opt_t *opt = opt_find(opt_name);

    if (opt != NULL && opt->present && opt->type == OPT_STRING) {
        return *(String *) opt_find_param(opt, index);
    } else {
        return "";
    }
}

int OptParser::get_int(String opt_name, unsigned int index) {
    opt_t *opt = opt_find(opt_name);

    if (opt != NULL && opt->present && opt->type == OPT_INT) {
        return *(int *) opt_find_param(opt, index);
    } else {
        return 0;
    }
}

unsigned int OptParser::get_uint(String opt_name, unsigned int index) {
    opt_t *opt = opt_find(opt_name);

    if (opt != NULL && opt->present && opt->type == OPT_UINT) {
        return *(unsigned int *) opt_find_param(opt, index);
    } else {
        return 0;
    }
}

long int OptParser::get_long(String opt_name, unsigned int index) {
    opt_t *opt = opt_find(opt_name);

    if (opt != NULL && opt->present && opt->type == OPT_LONG) {
        return *(long int *) opt_find_param(opt, index);
    } else {
        return 0;
    }
}

unsigned long OptParser::get_ulong(String opt_name, unsigned int index) {
    opt_t *opt = opt_find(opt_name);

    if (opt != NULL && opt->present && opt->type == OPT_ULONG) {
        return *(unsigned long *) opt_find_param(opt, index);
    } else {
        return 0;
    }
}

float OptParser::get_float(String opt_name, unsigned int index) {
    opt_t *opt = opt_find(opt_name);

    if (opt != NULL && opt->present && opt->type == OPT_FLOAT) {
        return *(float *) opt_find_param(opt, index);
    } else {
        return 0;
    }
}

double OptParser::get_double(String opt_name, unsigned int index) {
    opt_t *opt = opt_find(opt_name);

    if (opt != NULL && opt->present && opt->type == OPT_DOUBLE) {
        return *(double *) opt_find_param(opt, index);
    } else {
        return 0;
    }
}

unsigned long OptParser::get_hex(String opt_name, unsigned int index) {
    opt_t *opt = opt_find(opt_name);

    if (opt != NULL && opt->present && opt->type == OPT_HEX) {
        return *(unsigned long *) opt_find_param(opt, index);
    } else {
        return 0x00;
    }
}

unsigned int OptParser::get_counter(String opt_name) {
    opt_t *opt = opt_find(opt_name);

    if (opt != NULL && opt->present && opt->type == OPT_COUNTER) {
        return *(unsigned int *) opt_find_param(opt, 0);
    } else {
        return 0;
    }
}
