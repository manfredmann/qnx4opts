/*
 * Copyright © 2019 Roman Serov
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See the COPYING file or http://www.wtfpl.net/ 
 * for more details.
*/

#include "optparser.h"
#include <stdio.h>
#include <math.h>

OptParser::OptParser(String app_name, String app_desc) {
	this->app_desc = app_desc;
	this->app_name = app_name;
}

OptParser::~OptParser() {
	for (int i = 0; i < opt_list.entries(); ++i) {
		if (opt_list[i]->param != NULL) {
			delete opt_list[i]->param;
		}
		delete opt_list[i];
	}

	opt_list.clear();
}

void OptParser::add(String opt_name, String opt_desc) {
	opt_t *opt = new opt_t;

	int sep = opt_name.index(",");

	if (sep == -1) {
		if (opt_name.length() == 1) {
			opt->name	= opt_name.lower();
			opt->name_long	= "";
		} else {
			opt->name_long	= opt_name.lower();
			opt->name	= "";
		}
	} else {
		opt->name		= String(opt_name, 0, sep).lower();
		opt->name_long		= String(opt_name, sep + 1).lower();
	}

	opt->desc		= opt_desc;
	opt->type		= OPT_BOOL;
	opt->req 		= false;
	opt->present 		= false;
	opt->param 		= NULL;

	this->opt_list.append(opt);
}

void OptParser::add(String opt_name, String opt_desc, opt_types_t opt_type, bool req) {
	opt_t *opt = new opt_t;

	int sep = opt_name.index(",");

	if (opt_type == OPT_COUNTER) {
		opt->name	= opt_name[0];
		opt->name_long	= "";
	} else {
		if (sep == -1) {
			if (opt_name.length() == 1) {
				opt->name	= opt_name.lower();
				opt->name_long	= "";
			} else {
				opt->name_long	= opt_name.lower();
				opt->name	= "";
			}
		} else {
			opt->name 	= String(opt_name, 0, sep).lower();
			opt->name_long 	= String(opt_name, sep + 1).lower();
		}
	}

	opt->desc		= String(opt_desc);
	opt->type		= opt_type;
	opt->req 		= req;
	opt->present		= false;
	opt->param 		= NULL;

	this->opt_list.append(opt);
}

bool OptParser::is_param(String val) {
	if (val.length() < 2) {
		return false;
	}

	if (val[0] == '-') {
		return true;
	}

	return false;
}

bool OptParser::is_param_long(String val) {
	if (val.length() < 3) {
		return false;
	}

	if (val[0] == '-' && val[1] == '-') {
		return true;
	}

	return false;
}

bool OptParser::parse(int argc, char **argv, bool help) {
	unsigned int argi = 1;
	unsigned int i;

	if (argc < 2) {
		goto on_help;
	}

	while (argi != argc) {
		String val(argv[argi]);
		String param;

		if (this->is_param_long(val)) {
			param = String(String(val).lower(), 2);
		} else if (this->is_param(val)) {
			param = String(String(val).lower(), 1);
		} else {
			argi++;
			continue;
		}

		for (i = 0; i < this->opt_list.entries(); ++i) {
			opt_t *opt = this->opt_list[i];

			if (opt->type == OPT_COUNTER && !this->is_param_long(val) && this->is_param(val) && param[0] == opt->name) {
				char param_char = param[0];
				bool is_counter = true;

				for (int j = 1; j < param.length(); ++j) {
					if (param[j] != param_char) {
						is_counter = false;
						break;
					}
				}

				if (!is_counter) {
					continue;
				}

				opt->present = true;

				opt->param = new unsigned int;
				*(unsigned int *) opt->param = param.length();
				break;
			}

			if (this->is_param_long(val) && opt->name_long != param) {
				continue;
			}

			if (!this->is_param_long(val) && this->is_param(val) && opt->name != param) {
				continue;
			}

			if (opt->present) {
				goto on_help;
			}

			if (opt->type == OPT_BOOL) {
				opt->present = true;
				break;
			}
			
			argi++;
			char *arg_opt = argv[argi];

			if (argi == argc) {
				goto on_help;
			}

			if (this->is_param(argv[argi]) || this->is_param_long(argv[argi])) {
				unsigned long tmp;
				char *endptr = NULL;

				tmp = strtol(arg_opt, &endptr, 10);

				if (arg_opt == endptr)
					goto on_help;
			}

			switch(opt->type) {
				case OPT_STRING: {
					opt->param = new String(arg_opt);
					break;
				}
				case OPT_INT: {
					int param = atoi(arg_opt);

					opt->param = new int;
					memcpy(opt->param, &param, sizeof(int));
					break;
				}
				case OPT_UINT: {
					unsigned int param = strtoul(arg_opt, NULL, 10);

					opt->param = new unsigned int;
					memcpy(opt->param, &param, sizeof(unsigned int));
					break;
				}
				case OPT_LONG: {
					long int param = strtol(arg_opt, NULL, 10);

					opt->param = new long int;
					memcpy(opt->param, &param, sizeof(long int));
					break;
				}
				case OPT_ULONG: {
					unsigned long param = strtoul(arg_opt, NULL, 10);

					opt->param = new unsigned long;
					memcpy(opt->param, &param, sizeof(unsigned long));
					break;
				}
				case OPT_FLOAT: {
					float param = atof(arg_opt);

					opt->param = new float;
					memcpy(opt->param, &param, sizeof(float));
					break;
				}
				case OPT_DOUBLE: {
					double param = strtod(arg_opt, NULL);

					opt->param = new double;
					memcpy(opt->param, &param, sizeof(double));
					break;
				}
				case OPT_HEX: {
					unsigned long param = strtoul(arg_opt, NULL, 16);

					opt->param = new unsigned long;
					memcpy(opt->param, &param, sizeof(unsigned long));
					break;
				}
			}
			opt->present = true;
			break;
		}
		argi++;
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

bool OptParser::find(String opt_name) {
	for (unsigned int i = 0; i < this->opt_list.entries(); ++i) {
		opt_t *opt = this->opt_list[i];

		if (opt_name.length() > 1) {
			if (opt->name_long == opt_name.lower() && opt->present) {
				return true;
			}
		} else {
			if (opt->name == opt_name.lower() && opt->present) {
				return true;
			}	
		}
	}

	return false;
}

void OptParser::print_help() {
	unsigned int size = this->opt_list.entries();

	printf("%s - %s\n\n", (const char *) this->app_name, (const char *) this->app_desc);

	int max_len = 0;
	unsigned int i;
	
	for (i = 0; i < size; ++i) {
		opt_t *opt = this->opt_list[i];

		int opt_length = 0;

		if (opt->name.length() != 0) {
			opt_length += opt->name.length() + 1;

			if (opt->name_long.length() != 0) {
				opt_length += 8;
			}
		}

		if (opt->name_long.length() != 0) {
			opt_length += opt->name_long.length() + 2;
		}	

		if (max_len < opt_length) {
			max_len = opt_length;
		}
	}

	max_len = (int) ceil(max_len / 8.0);

	for (i = 0; i < size; ++i) {
		opt_t *opt = this->opt_list[i];

		printf("\t");

		if (opt->name.length() > 0) {
			printf("-%s", (const char *) opt->name);
		}

		if (opt->name_long.length() > 0) {
			if (opt->name.length() > 0) {
				printf("        ");
			}
			printf("--%s", (const char *) opt->name_long);
		}

		printf("\t");

		int opt_length = 0;

		if (opt->name.length() != 0) {
			opt_length += opt->name.length() + 1;

			if (opt->name_long.length() != 0) {
				opt_length += 8;
			}
		}

		if (opt->name_long.length() != 0) {
			opt_length += opt->name_long.length() + 2;
		}		

		for (int j = 0; j < max_len - (int) floor(opt_length / 8.0); j++) {
			printf("\t");
		}

		printf("%s\n", (const char *)opt->desc);
	}
}

opt_t *OptParser::find_opt(String opt_name) {
	for (unsigned int i = 0; i < this->opt_list.entries(); ++i) {
		opt_t *opt = this->opt_list[i];

		if (opt_name.length() > 1) {
			if (opt->name_long == opt_name.lower() && opt->present) {
				return opt;
			}
		} else {
			if (opt->name == opt_name.lower() && opt->present) {
				return opt;
			}	
		}
	}

	return NULL;
}

String OptParser::get_string(String opt_name) {
	opt_t *opt = find_opt(opt_name);

	if (opt != NULL && opt->present && opt->type == OPT_STRING) {
		return *(String *)opt->param;
	} else {
		return "";
	}
}

int OptParser::get_int(String opt_name) {
	opt_t *opt = find_opt(opt_name);

	if (opt != NULL && opt->present && opt->type == OPT_INT) {
		return *(int *)opt->param;
	} else {
		return 0;
	}
}

unsigned int OptParser::get_uint(String opt_name) {
	opt_t *opt = find_opt(opt_name);

	if (opt != NULL && opt->present && opt->type == OPT_UINT) {
		return *(unsigned int *) opt->param;
	} else {
		return 0;
	}
}

long int OptParser::get_long(String opt_name) {
	opt_t *opt = find_opt(opt_name);

	if (opt != NULL && opt->present && opt->type == OPT_LONG) {
		return *(long int *) opt->param;
	} else {
		return 0;
	}
}

unsigned long OptParser::get_ulong(String opt_name) {
	opt_t *opt = find_opt(opt_name);

	if (opt != NULL && opt->present && opt->type == OPT_ULONG) {
		return *(unsigned long *) opt->param;
	} else {
		return 0;
	}
}

float OptParser::get_float(String opt_name) {
	opt_t *opt = find_opt(opt_name);

	if (opt != NULL && opt->present && opt->type == OPT_FLOAT) {
		return *(float *)opt->param;
	} else {
		return 0;
	}
}

double OptParser::get_double(String opt_name) {
	opt_t *opt = find_opt(opt_name);

	if (opt != NULL && opt->present && opt->type == OPT_DOUBLE) {
		return *(double *) opt->param;
	} else {
		return 0;
	}
}

bool OptParser::get_bool(String opt_name) {
	opt_t *opt = find_opt(opt_name);

	if (opt != NULL && opt->present && opt->type == OPT_BOOL) {
		return true;
	} else {
		return false;
	}
}

unsigned long OptParser::get_hex(String opt_name) {
	opt_t *opt = find_opt(opt_name);

	if (opt != NULL && opt->present && opt->type == OPT_HEX) {
		return *(unsigned long *) opt->param;
	} else {
		return 0x00;
	}
}

unsigned int OptParser::get_counter(String opt_name) {
	opt_t *opt = find_opt(opt_name);

	if (opt != NULL && opt->present && opt->type == OPT_COUNTER) {
		return *(unsigned int *) opt->param;
	} else {
		return 0;
	}
}
