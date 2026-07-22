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
	printf("userdata created\n");

	if (current_ctx != NULL)
		current_ctx->next = new;
	return (new);
}

cli_context *flag_verbose_handle(cli_context *ptr1, char *clean_arg, char **argv, bool is_short_flag)
{
	cli_context	*list = ptr1;
	printf("verbose : %s\n", argv[0]);

	(void)list;
	(void)argv;
	(void)clean_arg;
	(void)is_short_flag;
	return (NULL);
}

cli_context *flag_help_handle(cli_context *ctx, char *clean_arg, char **argv, bool is_short_flag)
{
	cli_context	*list = ctx;
	printf("help : %s\n", argv[0]);
	(void)list;
	(void)argv;
	(void)clean_arg;
	(void)is_short_flag;
	return (NULL);
}

cli_context *create_str_ctx_node(char *str)
{
	cli_context *new = new_context(NULL);
	if (!new)
		return (NULL);

	((cli_userdata*)new->userdata)->str = strdup(str);
	if (((cli_userdata*)new->userdata)->str == NULL)
		return (NULL);

	new->flag_stop = true;

	return (new);
}

cli_context *flag_file_option_handle(cli_context *ctx, char *clean_arg, char **argv, bool is_short_flag)
{
	printf("clean arg : %s\n", clean_arg);
	char		*value = NULL;

	ctx = new_context(ctx);	// check leaks userdata malloc fail
	if (ctx == NULL)
		return (NULL);

	printf("\t\t\tuserdata check : %p\n", ctx->userdata);

	((cli_userdata *)ctx->next)->flag_infile = true;

	if (is_short_flag == false) {					// --file=file
		clean_arg = strchr(clean_arg, '=');						// --file=file --file file --file=
		if (clean_arg == NULL) {
			return (ctx);					// --file
		}

		if (strlen(&clean_arg[1]) == 0) {
			return (NULL);					// handle error --file= ...
		}
	}

	if (is_short_flag == true && strlen(clean_arg) == 1)
		return (ctx);

	else if (is_short_flag == true && strlen(clean_arg) > 1) {	// -ffile
		ctx->next = create_str_ctx_node(&clean_arg[1]);
		if (ctx->next == NULL)
			return (NULL);					// check leaks in the original new in case of failure

		printf("filename single char '%s'\n", ((cli_userdata *)ctx->next)->str);

		ctx = ctx->next;
	}

	printf("file option handle new check %p\n", new);

	(void)clean_arg;
	(void)value;
	(void)argv;
	return (new);
}

// cli_context	*flag_generic_str_handle(cli_context *ctx, char *clean_arg, char **argv)
// {
// 	cli_context	*list = ctx;
// 	cli_context	*new = calloc(1, (sizeof(cli_context)));
// 	if (new == NULL)
// 		return (NULL);

// 	new->filename = strdup(argv[0]);
// 	if (new->filename == NULL)
// 		return (NULL);

// 	new->flag_outfile = true;

// 	list->next = new;

// 	(void)clean_arg;
// 	(void)argv;
// 	return (new);
// }

const cli_flag_handler flags[] = {
	{{ 'v', "verbose", NO_FLAG_ARG, "Defines the verbosity of the program."}, flag_verbose_handle},
	{{ 'h', "help", NO_FLAG_ARG, "Shows this help page and exits."}, flag_help_handle},
	{{ 'f', "file", MANDATORY_FLAG_ARG, "Sets the file used as input"}, flag_file_option_handle},
	{{ 0, NULL, 0, NULL}, NULL}
};

void	print_userdata(cli_context *list)
{
	printf("list check : %p\n", list);
	printf("list->userdata check : %p\n", list->userdata);

	do
	{
		cli_userdata	*userdata = list->userdata;
		printf("help %d\n", userdata->flag_help);
		printf("infile %d\n", userdata->flag_infile);
		printf("inline infile %d\n", userdata->flag_inline_infile);
		printf("outfile %d\n", userdata->flag_outfile);
		printf("%s\n", userdata->filename);
		printf("%s\n", userdata->str);
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

	cli_context	*list = calloc(1, (sizeof(cli_context)));
	if (!list)
		return ((void)dprintf(2, "malloc error list main"), 1);

	printf("ctx check : %p\n", list);

	list = arglib(argc - 1, &argv[1]);
	printf("list created\n");
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
