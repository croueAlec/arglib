#include "main.h"
#include "arglib.h"
#include "arglib_defines.h"

const cli_flag_config flag_config = { true, false, false, false };

cli_context	*new_context(void)
{
	cli_context	*ctx = calloc(1, sizeof(cli_context));
	if (ctx == NULL)
		return (NULL);

	ctx->userdata = calloc(1, sizeof(cli_userdata));
	if (ctx->userdata == NULL)
		return (NULL);

	return (ctx);
}

cli_context *flag_verbose_handle(cli_context *ptr1, char *clean_arg, char **argv)
{
	cli_context	*list = ptr1;
	printf("verbose : %s\n", argv[0]);

	(void)list;
	(void)argv;
	(void)clean_arg;
	return (NULL);
}

cli_context *flag_help_handle(cli_context *ctx, char *clean_arg, char **argv)
{
	cli_context	*list = ctx;
	printf("help : %s\n", argv[0]);
	(void)list;
	(void)argv;
	(void)clean_arg;
	return (NULL);
}

cli_context *flag_file_option_handle(cli_context *ctx, char *clean_arg, char **argv)
{
	printf("clean arg : %s\n", clean_arg);
	char		*value = NULL;
	cli_context	*new = new_context();
	if (new == NULL)
		return (NULL);

	ctx->next = new;

	if (ctx->is_short_flag == false) {
		char	*separator = strchr(clean_arg, '=');
		if (separator != NULL) {
			*separator = '\0';
		}

		if (separator != NULL) {
			new->next = new_context();
			if (new->next == NULL)
				return (NULL);				// check leaks in the original new in case of failure

			((cli_userdata *)new->next)->filename = strdup(&separator[1]);
			if (((cli_userdata *)new->next)->filename == NULL)
				return (NULL);
			((cli_userdata *)new->next)->flag_inline_infile = true;
			printf("filename string %s\n", ((cli_userdata *)new->next)->filename);

			new = new->next;
		}
	} else if (ctx->is_short_flag == true && strlen(clean_arg) > 1) {
		new->next = new_context();
		if (new->next == NULL)
			return (NULL);					// check leaks in the original new in case of failure

		((cli_userdata *)new->next)->filename = strdup(&clean_arg[1]);		// check the index that gets sent as a clean arg in the single argument if clean arg starts with the single or from the beginning
		if (((cli_userdata *)new->next)->filename == NULL)
			return (NULL);
		((cli_userdata *)new->next)->flag_inline_infile = true;
		printf("filename single char %s\n", ((cli_userdata *)new->next)->filename);

		new = new->next;
	}

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
	cli_userdata	*userdata = list->userdata;
	printf("%p\n", userdata);
	
	while (list->next != NULL)
	{
		printf("help %d\n", userdata->flag_help);
		printf("infile %d\n", userdata->flag_infile);
		printf("inline infile %d\n", userdata->flag_inline_infile);
		printf("outfile %d\n", userdata->flag_outfile);
		printf("%s\n", userdata->filename);
		printf("%s\n", userdata->str);
		printf("\n");
		list = list->next;
		userdata = list->userdata;
	}
	
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

	arglib(argc - 1, &argv[1], list);
	printf("list created\n");
	print_userdata(list);

	(void)argc;
	return 0;
}
