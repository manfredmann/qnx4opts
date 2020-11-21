# qnx4opts
Простая библиотека разбора параметров командной строки для компилятора Watcom 10.6 из среды QNX4.25. Реализация "в лоб".

### Основные методы

```C++
void add(String opt_name, String opt_desc);
void add(String opt_name, String opt_desc, opt_types_t opt_type, bool req, bool multiple);
```
Добавить параметр

`String opt_name`
Имя параметра. Короткое и длинное имя пишутся через разделитель ",". При отсутствии разделителя, длинное это или короткое имя определяется по длине строки.

`String opt_desc`
Описание параметра

`opt_types_t opt_type`
Тип параметра. Возможные варианты:
```C++
typedef enum {
    OPT_BOOL, //Параметр без аргумента, просто определяется его наличие
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
```

`bool req`
Обязателен параметр или нет

`bool multiple`
Можно ли принять несколько параметров с одинаковым именем

***
```C++
bool parse(int argc, char **argv, bool help);
```
Разобрать параметры.

Думаю, про argc и argv нет смысла объяснять.

`bool help`
Вывод справки при отсутствии необходимых параметров
***
```C++  
int find(String opt_name);
```
Найти параметр. 
Возвращает количество параметров.
***
```C++
void print_help();
```
Вывести справку
***
```C++
String          get_string(String opt_name, unsigned int index = 0);
int             get_int(String opt_name, unsigned int index = 0);
unsigned int    get_uint(String opt_name, unsigned int index = 0);
long int        get_long(String opt_name, unsigned int index = 0);
unsigned long   get_ulong(String opt_name, unsigned int index = 0);
float           get_float(String opt_name, unsigned int index = 0);
double          get_double(String opt_name, unsigned int index = 0);
unsigned long   get_hex(String opt_name, unsigned int index = 0);
unsigned int    get_counter(String opt_name);
```
Получить значение параметра.

## Пример

```C++
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
        opt->add("i,int",       "Integer",          OPT_INT,    true, false);
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

```
Пример вывода:
```
./bin/qnx4opts -t --test -i 1 -f=-3.14 -s hello -s world -s lalala -s kot -e 0xaa55 -l=-1 -u 5 -m 4 -b 3.44 -vvvvvv azaza 100500
t is present!
Test is present!
int = 1
Unsigned int = 5
float = -3.140000
string = hello
string = world
string = lalala
string = kot
hex = 0xAA55
Long = -1
Unsigned long = 4
Double = 3.440000
Verbose level: 6
Untyped: azaza
Untyped: 100500
```

Пример вывода справки:

```
# ./bin/qnx4opts -h
./qnx4opts - Options parser

  -t                       - Without args
  --test                   - Without args
  -h, --help               - Help
  -i, --int       <int>    - Integer
  -f, --float     <float>  - Float
  -s, --string    <string> - String
  -e, --hex       <hex>    - Hex
  -l, --long      <long>   - Long
  -u, --unsigned  <uint>   - Unsigned int
  -m, --ulong     <ulong>  - Unsigned long
  -b, --double    <double> - Double
  -v                       - Verbose test
```
