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
	OptParser opt(argv[0], "Options parser");

	//Параметр -t без аргументов
	opt.add("t", 		"Without args");
	//Параметр --test без аргументов
	opt.add("test", 	"Without args");
	//Параметр -h (длинное имя --help) без аргументов
	opt.add("h,help", 	"Help");
	//Параметр -i(--int) с аргументом типа integer, обязательный
	opt.add("i,int", 	"Integer", 		OPT_INT, 	true);
	//Параметр -f(--float) с аргументом типа float, обязательный
	opt.add("f,float", 	"Float", 		OPT_FLOAT, 	true);
	//Параметр -s(--string) с аргументом типа string, обязательный
	opt.add("s,string", "String", 		OPT_STRING, true);
	//Параметр -h(--hex) с аргументом типа hex (преобразуется в unsigned long), не обязательный
	opt.add("e,hex", 	"Hex", 			OPT_HEX, 	false);
	//Параметр -v счётчик (считает количество v)
	opt.add("v", 		"Verbose test", OPT_COUNTER);

	if (!opt.parse(argc, argv, true)) {
		return 0;
	}

	if (opt.find("h")) {
		opt.print_help();
		return 0;
	}

	if (opt.find("t")) {
		printf("t is present!\n");
	}

	if (opt.find("test")) {
		printf("Test is present!\n");
	}

	if (opt.find("int")) {
		printf("int = %d\n", opt.get_int("int"));
	}

	if (opt.find("float")) {
		printf("float = %f\n", opt.get_float("float"));
	}

	if (opt.find("string")) {
		printf("string = %s\n", (const char *) opt.get_string("string"));
	}

	if (opt.find("hex")) {
		printf("hex = 0x%X\n", (const char *) opt.get_hex("hex"));
	}

	if (opt.find("v")) {
		printf("Verbose level: %d\n", opt.get_counter("v"));
	}

	return 0;
}