#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "../srcs/arglib/arglib.h"

typedef struct cli_userdata
{
	bool	flag_infile;
	bool	flag_outfile;
	bool	flag_help;
	bool	flag_verbose;
	bool	flag_cut_str;
	bool	flag_str;

	char	*str;
} cli_userdata;

#endif
