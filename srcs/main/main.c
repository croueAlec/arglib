#include "main.h"

void print_userdata(cli_context *list)
{
	if (!list)
		return ((void)printf("no list created, exiting\n"));

	do {
		cli_userdata *userdata = list->userdata;
		printf("\tinfile: %d\toutfile: %d\thelp: %d\tverbose: %d\tstr: [%s]\n", userdata->flag_infile, userdata->flag_outfile, userdata->flag_help, userdata->flag_verbose, userdata->str);
		list = list->next;
	} while (list);
}

int main(int argc, char **argv)
{
	const char *cli_flags_error_message = is_cli_flag_handler_valid(flags);
	if (cli_flags_error_message != NULL) {
		printf("%s\n", cli_flags_error_message);
		return (1);
	}

	cli_context *list = NULL;

	list = arglib((size_t)(argc - 1), &argv[1]);
	if (list != (cli_context *)0x1) {
		printf("list created : %p\n", list);
		print_userdata(list);
	} else {
		printf("no arguments, exiting\n");
	}

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
