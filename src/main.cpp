/*
 * Copyright © 2019 Roman Serov
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file or http://www.wtfpl.net/ 
 * for more details.
*/

#include "optparser.h"
#include <stdio.h>

int main(int argc, char **argv) {
    //Конструктор принимает в качестве параметров имя исполняемого файла и описание программы
    OptParser *opt = new OptParser(argv[0], "Options parser");

    try {
        //Параметр -t без аргументов
        opt->add("t",           "Without args");
        //Параметр --test без аргументов
        opt->add("test",        "Without args");
        //Параметр -h (длинное имя --help) без аргументов
        opt->add("h,help",      "Help");
        //Параметр -i(--int) с аргументом типа integer, обязательный
        opt->add("i,int",       "Integer",          OPT_INT,    false, false);
        //Параметр -f(--float) с аргументом типа float, не обязательный
        opt->add("f,float",     "Float",            OPT_FLOAT,  false, false);
        //Параметр -s(--string) с аргументом типа string, не обязательный, возможно иметь несколько одинаковых
        opt->add("s,string",    "String",           OPT_STRING, false, true);
        //Параметр -h(--hex) с аргументом типа hex (преобразуется в unsigned long), не обязательный
        opt->add("e,hex",       "Hex",              OPT_HEX,    false, false);
        //Параметр -l(--long) с аргументом типа long, не обязательный
        opt->add("l,long",      "Long",             OPT_LONG,   false, false);
        //Параметр -u(--unsigned) с аргументом типа unsigned int, не обязательный
        opt->add("u,unsigned",  "Unsigned int",     OPT_UINT,   false, false);
        //Параметр -m(--ulong) с аргументом типа unsigned long, не обязательный
        opt->add("m,ulong",     "Unsigned long",    OPT_ULONG,  false, false);
        //Параметр -b(--double) с аргументом типа double, не обязательный
        opt->add("b,double",    "Double",           OPT_DOUBLE, false, false);
        //Параметр -v счётчик (считает количество v)
        opt->add("v",           "Verbose test",     OPT_COUNTER, false, true);

        if (!opt->parse(argc, argv, true)) {
            delete opt;
            return 0;
        }

        if (opt->find("h")) {
            opt->print_help();
            delete opt;
            return 0;
        }

        if (opt->find("t")) {
            printf("t is present!\n");
        }

        if (opt->find("test")) {
            printf("Test is present!\n");
        }

        if (opt->find("int")) {
            printf("int = %d\n", opt->get_int("int"));
        }

        if (opt->find("unsigned")) {
            printf("Unsigned int = %u\n", opt->get_uint("unsigned"));
        }

        if (opt->find("float")) {
            printf("float = %f\n", opt->get_float("float"));
        }

        unsigned int str_cnt;

        if (str_cnt = opt->find("string")) {
            for (unsigned int i = 0; i < str_cnt; ++i) {
                printf("string = %s\n", (const char *) opt->get_string("string", i));
            }
        }

        if (opt->find("hex")) {
            printf("hex = 0x%X\n", opt->get_hex("hex"));
        }

        if (opt->find("long")) {
            printf("Long = %ld\n", opt->get_long("long"));
        }

        if (opt->find("ulong")) {
            printf("Unsigned long = %lu\n", opt->get_ulong("ulong"));
        }

        if (opt->find("double")) {
            printf("Double = %f\n", opt->get_double("double"));
        }

        if (opt->find("v")) {
            printf("Verbose level: %d\n", opt->get_counter("v"));
        }

        size_t c = opt->get_untyped_count();

        for (size_t i = 0; i < c; ++i) {
            printf("Untyped: %s\n", (const char *) opt->get_untyped(i));
        }

    } catch (OptParser_Ex e) {
        printf("%s\n", e.what_c());
    }


    delete opt;

    return 0;
}
