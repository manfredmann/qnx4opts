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
	OPT_LONG,
	OPT_FLOAT,
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

	String			param_string;
	int			param_int;
	long			param_long;
	float			param_float;
	unsigned long		param_hex;
	unsigned int		param_counter;
} opt_t;

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
		int			get_int(String opt_name);
		long			get_long(String opt_name);
		float			get_float(String opt_name);
		bool			get_bool(String opt_name);
		unsigned long		get_hex(String opt_name);
		unsigned int		get_counter(String opt_name);

	private:
		String 			app_name;
		String 			app_desc;
		opt_vector_t		opt_list;

		bool			is_param(String val);
		bool			is_param_long(String val);
		opt_t *			find_opt(String opt_name);
};
