#include "arglib.h"
#include "arglib_defines.h"

const char	*is_cli_flag_handler_valid(const cli_flag_handler *flags)
{
	if (flags == NULL)
		return ("Error : The 'flags' array must at least contain an empty entry"); // flags array must exist

	size_t	i = 0;
	while (flags[i].handler)
	{
		if (flags[i].metadata.long_flag == NULL || flags[i].metadata.desc == NULL) {
			return ("Error : Flags cannot contain NULL strings"); // only the last flag may have NULL strings
		}
		i++;
	}

	if (flags[i].handler == NULL && flags[i].metadata.long_flag != NULL && flags[i].metadata.desc != NULL) {
		return ("Error : All values of the last flags must be set to 0 or NULL"); // last flag must be set to 0
	}
	return (NULL);
}

cli_context *handle_long_flag(char *current_arg, cli_context *ctx, size_t flag_list_length, char **argv)
{
	char	*separator = strchr(current_arg, '=');

	printf("long flag check\n");
	
	if (separator != NULL) {
		*separator = '\0';
		printf("long flag check, separator : %s\n", &separator[1]);
	}

	for (size_t i = 0; i < flag_list_length; i++)
	{
		if (flags[i].metadata.long_flag && strcmp(current_arg, flags[i].metadata.long_flag) == 0) {
			if (separator != NULL)
				*separator = '=';
			flags[i].handler(ctx, current_arg, argv, false);
			return (NULL);
		}
	}

	return (NULL);
}

// handle these edge cases 'ls --W' 'ls -W'
cli_context *handle_short_flag(char *current_arg, cli_context *ctx, size_t flag_list_length, char **argv)
{
	cli_context	*new = NULL;
	bool		flag_found = false;
	// ctx->is_short_flag = true;
	if (current_arg[0] == '\0')
		return (false);

	size_t	j = 0;
	for ( ; current_arg[j]; j++)
	{
		for (size_t i = 0; i < flag_list_length; i++)
		{
			if (current_arg[j] == flags[i].metadata.short_flag) {
				new = flags[i].handler(ctx, current_arg, argv, true);
				if (ctx != NULL) {
					ctx->next = new;
				}
				ctx = new;
				flag_found = true;

				if (ctx->flag_stop == true)
					return (ctx);
			}

		}

		if (flag_found == false)		// handle error better
			return (NULL);

		flag_found = false;
	}

	if (flag_found == false)
		printf("invalid argument %c\n", current_arg[j]);
	return (ctx);

	// flag invalid (stop all + error)
	// flag found (ok) true and false
	// flag with mandatory option found (ok but stop iterating) true and true
}

bool set_double_dash_separator()
{
	if (flag_config.allow_double_dash_separator == false) {
		printf("Error : Invalid argument '--'\n");
		return (true);
	}
	return (false);
}

size_t get_flag_list_length()
{
	size_t	i = 0;
	while (flags[i].handler)
	{
		i++;
	}
	return (i);
}

cli_context *arglib(int argc, char **argv)
{
	cli_context	*head = NULL;
	cli_context	*ctx = NULL;
	size_t		flag_list_length = get_flag_list_length();
	bool		double_dash = false;
	size_t		i = 0;

	for ( ; i < (size_t)argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] == '-' && argv[i][2] == '\0') {
			double_dash = set_double_dash_separator();
		} else if (argv[i][0] == '-' && argv[i][1] == '-') {
			ctx = handle_long_flag(&argv[i][2], ctx, flag_list_length, argv);
		} else if (argv[i][0] == '-') {
			ctx = handle_short_flag(&argv[i][1], ctx, flag_list_length, argv);
		} else {
			printf("> %s\n", argv[i]);
			if (flag_config.positional_argument_order_sensitive == true)
				break;
		}

		if (i == 0)
			head = ctx;

		printf("ctx check : %p\n", ctx);

		if (double_dash == true)
			break;
		// if (error == true) {
		// 	printf("error\n");
		// 	return (1);
		// }
	}

	for ( ; i < (size_t)argc; i++)
	{
		printf("%s\n", argv[i]);		// handle double dash (text arguments)
	}

	return (head);
}
