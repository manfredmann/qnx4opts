# qnx4opts
Простая библиотека разбора параметров командной строки для компилятора Watcom 10.6 из среды QNX4.25. Реализация "в лоб".

### Основные методы

```C++
void 			add(String opt_name, String opt_desc);
void 			add(String opt_name, String opt_desc, opt_types_t opt_type, bool req = false);
```
Добавить параметр

`String opt_name`
Имя параметра. Короткое и длинное имя пишутся через разделитель ",". При отсутствии разделителя, длинное это или короткое имя определяется по длине строки.

`String opt_desc`
Описание параметра

`opt_types_t opt_type`
Тип аргумента. Возможные варианты:
```C++
typedef enum {
	OPT_BOOL, //Параметр без аргумента, просто определяется его наличие
	OPT_STRING,
	OPT_INT,
	OPT_LONG,
	OPT_FLOAT,
	OPT_HEX,
	OPT_COUNTER,
} opt_types_t;
```

`bool req`
Обязателен параметр или нет
***
```C++
bool 			parse(int argc, char **argv, bool help);
```
Разобрать параметры.

Думаю, про argc и argv нет смысла объяснять.

`bool help`
Вывод справки при отсутствии необходимы аргументов
***
```C++	
bool 			find(String opt_name);
```
Найти параметр
***
```C++
void 			print_help();
```
Вывести справку
***
```C++
String 			get_string(String opt_name);
int 			get_int(String opt_name);
long 			get_long(String opt_name);
float 			get_float(String opt_name);
bool 			get_bool(String opt_name);
unsigned long 	get_hex(String opt_name);
unsigned int 	get_counter(String opt_name);
```
Получить значение аргумента

## Пример

```C++
#include "optparser.h"
#include <stdio.h>

int main(int argc, char **argv) {
	//Конструктор принимает в качестве параметров имя исполняемого файла и описание программы
	OptParser opt(argv[0], "Options parser");

	//Параметр -t без аргументов (OPT_BOOL)
	opt.add("t", 		"Without args");
	//Параметр --test без аргументов (OPT_BOOL)
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

```
Пример вывода:
```
# ./bin/optparser -i 1 --float 3.14 -s "Hello World" -e 0xAA55 -b --test -vvv      
Test is present!
int = 1
float = 3.140000
string = Hello World
hex = 0xAA55
Verbose level: 3
```
Пример вывода справки:
```
# ./bin/optparser -h
./bin/optparser - Options parser

        -t                              Without args
        --test                          Without args
        -h        --help                Help
        -i        --int                 Integer
        -f        --float               Float
        -s        --string              String
        -e        --hex                 Hex
        -v                              Verbose test
```
