#ifndef ARGLIB_H
# define ARGLIB_H

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum arg_requirement {
	NO_FLAG_ARG = 0,
	MANDATORY_FLAG_ARG = 1,
	OPTIONNAL_FLAG_ARG = 2
} arg_requirement;

typedef struct cli_flag_metadata
{
	char short_flag;
	const char *long_flag;
	arg_requirement required_arg;
	const char *desc;
}	cli_flag;

typedef struct cli_context
{
	void	*userdata;
	bool	is_short_flag;
	bool	flag_stop;

	struct cli_context	*next;
} cli_context;

typedef cli_context *(*flag_handler_function)(cli_context *cli_context, char *clean_arg, char **argv, bool is_short_flag);

typedef struct cli_flag_handler
{
	cli_flag	metadata;
	flag_handler_function handler;
}	cli_flag_handler;

typedef struct cli_flag_config
{
	bool	allow_double_dash_separator;
	bool	positional_argument_order_sensitive;
	bool	short_flag_fatal;
	bool	long_flag_fatal;
}	cli_flag_config;

extern const cli_flag_handler flags[];
extern const cli_flag_config flag_config;

cli_context	*arglib(int argc, char **argv);
const char	*is_cli_flag_handler_valid(const cli_flag_handler *flags);

#endif
