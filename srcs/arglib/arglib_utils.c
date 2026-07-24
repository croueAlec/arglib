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

cli_context *get_first_ctx_node(cli_context *ctx)
{
	while (ctx && ctx->prev)
		ctx = ctx->prev;
	return (ctx);
}

flag_handler_function get_str_flag_handler(size_t flag_list_length)
{
	size_t	i = 0;
	while (i < flag_list_length)
	{
		if (flags[i].metadata.required_arg == STR_FLAG_ARG)
			return (flags[i].handler);
		i++;
	}

	return (NULL);
}
