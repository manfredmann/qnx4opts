/*
 * Copyright © 2019 Roman Serov
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file or http://www.wtfpl.net/ 
 * for more details.
*/

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <String.h>
#include <wcvector.h>

typedef uint8_t bool;

#define true 1
#define false 0

typedef enum {
	OPT_BOOL,
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

typedef struct {
	String			name;
	String			name_long;
	String			desc;
	opt_types_t		type;
	bool			req;
	bool			present;
	void 			*param;
} opt_t;

typedef enum {
	ARG_SHORT,
	ARG_LONG,
	ARG_SHORTWPARAM,
	ARG_NO,
} arg_type_t;

typedef WCValOrderedVector<opt_t *> opt_vector_t;

class OptParser {
	public:
		OptParser(String app_name, String app_desc);
		~OptParser();
		
		void 			add(String opt_name, String opt_desc);
		void 			add(String opt_name, String opt_desc, opt_types_t opt_type, bool req = false);

		bool 			find(String opt_name);
		bool 			parse(int argc, char **argv, bool help);
		void 			print_help();

		String			get_string(String opt_name);
		int				get_int(String opt_name);
		unsigned int	get_uint(String opt_name);
		long int		get_long(String opt_name);
		unsigned long	get_ulong(String opt_name);
		float			get_float(String opt_name);
		double			get_double(String opt_name);
		bool			get_bool(String opt_name);
		unsigned long	get_hex(String opt_name);
		unsigned int	get_counter(String opt_name);

	private:
		String 			app_name;
		String 			app_desc;
		opt_vector_t	opt_list;

		int 			find_eq(const char *str);
		arg_type_t 		is_arg(const char *str);
		bool 			is_negatvie_decimal(const char *str);
		bool 			is_decimal(const char *str);
		bool 			is_double(const char *str);
		bool 			is_negative_double(const char *str);

		opt_t *			find_opt(String opt_name);
};
