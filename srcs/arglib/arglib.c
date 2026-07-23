#include "arglib.h"
#include "arglib_defines.h"

static cli_context *handle_long_flag(char *current_arg, cli_context *ctx, size_t flag_list_length, char **argv)
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
			ctx = flags[i].handler(ctx, current_arg, argv, false);
			return (ctx);
		}
	}

	printf("error --flag does not match\n");
	return (NULL);
}

// handle these edge cases 'ls --W' 'ls -W'
static cli_context *handle_short_flag(char *current_arg, cli_context *ctx, size_t flag_list_length, char **argv)
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

static cli_context *handle_trailing_str_flags(cli_context *ctx, char **argv,  size_t i, bool double_dash, flag_handler_function handle_str_flag)
{
	for ( ; argv[i]; i++)
	{
		printf("str arguments ok\n");
		printf("%s\n", argv[i]);
		ctx = handle_str_flag(ctx, argv[i], argv, double_dash);
		if (ctx == NULL)
			return (NULL);
	}

	return (ctx);
}

cli_context *arglib(size_t argc, char **argv)
{
	if (argc == 0)
		return ((cli_context*)0x1);

	cli_context				*ctx = NULL;
	size_t					flag_list_length = get_flag_list_length();
	bool					double_dash = false;
	size_t					i = 0;
	flag_handler_function	handle_str_flag = get_str_flag_handler(flag_list_length);

	for ( ; i < (size_t)argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] == '-' && argv[i][2] == '\0') {
			double_dash = true;
			break;
		} else if (argv[i][0] == '-' && argv[i][1] == '-') {
			ctx = handle_long_flag(&argv[i][2], ctx, flag_list_length, argv);
		} else if (argv[i][0] == '-') {
			ctx = handle_short_flag(&argv[i][1], ctx, flag_list_length, argv);
		} else if (flag_config.positional_argument_order_sensitive == true || i == 0) {
				break;
		} else {
			ctx = handle_str_flag(ctx, argv[i], argv, double_dash);
		}
	}

	ctx = handle_trailing_str_flags(ctx, argv, i, double_dash, handle_str_flag);

	return (get_first_ctx_node(ctx));
}
