#include "main.h"

cli_context	*get_first_ctx_node(cli_context *ctx);

static void free_arglib_node(cli_context *ctx)
{
	cli_userdata	*userdata = (cli_userdata*)ctx->userdata;

	if (userdata) {
		if (userdata->str) {
			free(userdata->str);
		}
		free(userdata);
	}

	free(ctx);
}

void free_arglib_list(cli_context *ctx)
{
	cli_context	*tmp = NULL;
	
	ctx = get_first_ctx_node(ctx);

	while (ctx && ctx->next)
	{
		tmp = ctx;
		ctx = ctx->next;
		free_arglib_node(tmp);
	}
}

static cli_context	*new_context(cli_context *current_ctx)
{
	cli_context	*new = calloc(1, sizeof(cli_context));
	if (new == NULL)
		return (NULL);

	if (current_ctx != NULL) {
		current_ctx->next = new;
		new->prev = current_ctx;
	}

	new->userdata = calloc(1, sizeof(cli_userdata));
	if (new->userdata == NULL)
		return (free(new), NULL);

	return (new);
}

static cli_context *flag_verbose_handle(cli_context *ctx, char *clean_arg, char **argv, bool is_short_flag)
{
	ctx = new_context(ctx);
	if (ctx == NULL)
		return (free_arglib_node(ctx), NULL);

	((cli_userdata*)ctx->userdata)->flag_verbose = true;

	(void)clean_arg;
	(void)argv;
	(void)is_short_flag;
	return (ctx);
}

static cli_context *flag_help_handle(cli_context *ctx, char *clean_arg, char **argv, bool is_short_flag)
{
	ctx = new_context(ctx);
	if (ctx == NULL)
		return (free_arglib_node(ctx), NULL);

	((cli_userdata*)ctx->userdata)->flag_help = true;

	(void)clean_arg;
	(void)argv;
	(void)is_short_flag;
	return (ctx);
}

static cli_context *create_str_ctx_node(cli_context *ctx, char *str)
{
	cli_context		*new = new_context(ctx);
	if (!new)
		return (NULL);

	cli_userdata	*userdata = new->userdata;

	userdata->str = strdup(str);
	if (userdata->str == NULL)
		return (free_arglib_node(ctx), NULL);

	return (new);
}

static cli_context *flag_str_option_handle(cli_context *ctx, char *clean_arg, char **argv, bool is_double_dash)
{
	ctx = create_str_ctx_node(ctx, clean_arg);
	if (ctx == NULL)
		return (free_arglib_node(ctx), NULL);

	((cli_userdata*)ctx->userdata)->flag_str = true;
	((cli_userdata*)ctx->userdata)->flag_cut_str = is_double_dash;

	(void)argv;
	return (ctx);
}

static cli_context *flag_file_option_handle(cli_context *ctx, char *clean_arg, char **argv, bool is_short_flag)
{
	cli_context *new = new_context(ctx);
	if (new == NULL)
		return (free_arglib_list(ctx), NULL);
	else
		ctx = new;

	((cli_userdata *)ctx->userdata)->flag_infile = true;

	if (is_short_flag == false) {
		clean_arg = strchr(clean_arg, '=');
		if (clean_arg == NULL) {
			return (ctx);
		}

		if (strlen(&clean_arg[1]) == 0) {
			return (NULL);
		}
	}

	if (is_short_flag == true && strlen(clean_arg) == 1) {
		ctx->flag_stop = true;
		return (ctx);
	}

	if (is_short_flag == false || (is_short_flag == true && strlen(clean_arg) > 1)) {
		ctx->next = create_str_ctx_node(ctx, &clean_arg[1]);
		if (ctx->next == NULL)
			return (NULL);

		ctx = ctx->next;
	}

	if (is_short_flag == true)
		ctx->flag_stop = true;

	(void)argv;
	return (ctx);
}

const cli_flag_config flag_config = { true, false };

const cli_flag_handler flags[] = {
	{{ 'v', "verbose", NO_FLAG_ARG, "Defines the verbosity of the program."}, flag_verbose_handle},
	{{ 'h', "help", NO_FLAG_ARG, "Shows this help page and exits."}, flag_help_handle},
	{{ 'f', "file", MANDATORY_FLAG_ARG, "Sets the file used as input"}, flag_file_option_handle},
	{{ '\0', "", STR_FLAG_ARG, ""}, flag_str_option_handle},
	{{ 0, NULL, 0, NULL}, NULL}
};
