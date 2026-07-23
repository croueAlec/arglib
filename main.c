#include "main.h"
#include "arglib.h"
#include "arglib_defines.h"

const cli_flag_config flag_config = { true, false, false, false };

cli_context	*new_context(cli_context *current_ctx)
{
	cli_context	*new = calloc(1, sizeof(cli_context));
	if (new == NULL)
		return (NULL);

	new->userdata = calloc(1, sizeof(cli_userdata));
	if (new->userdata == NULL)
		return (NULL);

	if (current_ctx != NULL) {
		current_ctx->next = new;
		new->prev = current_ctx;
	}

	printf("new context: [%p]\tuserdata: [%p]\tcurrent ctx: [%p]\n", new, new->userdata, current_ctx);
	return (new);
}

cli_context *flag_verbose_handle(cli_context *ctx, char *clean_arg, char **argv, bool is_short_flag)
{
	ctx = new_context(ctx);
	if (ctx == NULL)
		return (NULL);

	((cli_userdata*)ctx->userdata)->flag_verbose = true;

	(void)clean_arg;
	(void)argv;
	(void)is_short_flag;
	return (ctx);
}

cli_context *flag_help_handle(cli_context *ctx, char *clean_arg, char **argv, bool is_short_flag)
{
	ctx = new_context(ctx);
	if (ctx == NULL)
		return (NULL);

	((cli_userdata*)ctx->userdata)->flag_help = true;

	(void)clean_arg;
	(void)argv;
	(void)is_short_flag;
	return (ctx);
}

cli_context *create_str_ctx_node(cli_context *ctx, char *str)
{
	cli_context		*new = new_context(ctx);
	if (!new)
		return (NULL);

	cli_userdata	*userdata = new->userdata;

	userdata->str = strdup(str);
	if (userdata->str == NULL)
		return (NULL);

	printf("\t\t\tstr node : '%s'\t%p==%p\n", userdata->str, userdata, new->userdata);

	new->flag_stop = true;

	return (new);
}

cli_context *flag_file_option_handle(cli_context *ctx, char *clean_arg, char **argv, bool is_short_flag)
{
	printf("clean arg : %s\n", clean_arg);

	ctx = new_context(ctx);	// check leaks userdata malloc fail
	if (ctx == NULL)
		return (NULL);

	printf("\t\t\tuserdata check : %p\n", ctx->userdata);

	((cli_userdata *)ctx->userdata)->flag_infile = true;

	if (is_short_flag == false) {					// --file=file
		clean_arg = strchr(clean_arg, '=');						// --file=file --file file --file=
		if (clean_arg == NULL) {
			return (ctx);					// --file
		}

		if (strlen(&clean_arg[1]) == 0) {
			return (NULL);					// handle error --file= ... (missing argument after =)
		}
	}

	if (is_short_flag == true && strlen(clean_arg) == 1)
		return (ctx);

	if (is_short_flag == false || (is_short_flag == true && strlen(clean_arg) > 1)) {	// -ffile
		ctx->next = create_str_ctx_node(ctx, &clean_arg[1]);
		if (ctx->next == NULL)
			return (NULL);					// check leaks in the original new in case of failure

		printf("context: [%p]\tnext ctx: [%p]\tnext userdata [%p]\tstr: [%s]\n", ctx, ctx->next, ctx->next->userdata, ((cli_userdata*)ctx->next->userdata)->str);

		ctx = ctx->next;
	}

	(void)argv;
	return (ctx);
}

const cli_flag_handler flags[] = {
	{{ 'v', "verbose", NO_FLAG_ARG, "Defines the verbosity of the program."}, flag_verbose_handle},
	{{ 'h', "help", NO_FLAG_ARG, "Shows this help page and exits."}, flag_help_handle},
	{{ 'f', "file", MANDATORY_FLAG_ARG, "Sets the file used as input"}, flag_file_option_handle},
	{{ 0, NULL, 0, NULL}, NULL}
};

void	print_userdata(cli_context *list)
{
	if (!list)
		return ((void) printf("no list created, exiting\n"));
	printf("\n\n\n\nlist check : %p\n", list);
	printf("list->userdata check : %p\n", list->userdata);

	do
	{
		cli_userdata	*userdata = list->userdata;
		printf("\tinfile %d\n", userdata->flag_infile);
		printf("\toutfile %d\n", userdata->flag_outfile);
		printf("\thelp %d\n", userdata->flag_help);
		printf("\tverbose %d\n", userdata->flag_verbose);

		printf("\tstr %s\n", userdata->str);
		printf("\n");
		list = list->next;
	} while (list);
	
}

int	main(int argc, char **argv)
{
	const char	*cli_flags_error_message = is_cli_flag_handler_valid(flags);
	if (cli_flags_error_message != NULL) {
		printf("%s\n", cli_flags_error_message);
		return (1);
	}

	cli_context	*list = NULL;

	list = arglib(argc - 1, &argv[1]);
	printf("list created : %p\n", list);
	print_userdata(list);

	(void)argc;
	return 0;
}

/* 
Command examples :

grep -f patterns
grep -fpatterns
grep --file patterns
grep --file patterns --file=p2 -fp3
grep --filepatterns	<< KO
*/

/* 

Each detected flag triggers it's handler
each handler creates a new node

*/
