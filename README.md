# Arthur G. Libert aka Arglib; the CLI argument library
made by acroue

Arglib debugging library is a tool meant to improve the displaying of errors, information and output of your C program.

The arglib library is a CLI argument tokenizer for your C program. It is meant to be as flexible and customizable as possible.

## How it works

The library builds a linked list of tokens from the CLI arguments. Tokens can be **flag options** or **strings**.

### Configuration
The user creates a **Flag Prototype Array** using the following structures
```c
typedef struct cli_flag_metadata
{
	char			short_flag;
	const char		*long_flag;
	arg_requirement	required_arg;
	const char		*desc;
}	cli_flag;

typedef struct cli_flag_handler
{
	cli_flag				metadata;
	flag_handler_function	handler;
}	cli_flag_handler;
```

A **Flag Prototype** can contain a `short` or `long` **flag option**, a **requirement specifier**, and a **description**. Each **flag** requires a corresponding **handler function** pointer. This is the function that will be called when a **flag** is found.

A valid **Flags Prototype Array** should look like this
```c
const cli_flag_handler flags[] = {
	{{ 'v', "verbose", NO_FLAG_ARG, "Defines the verbosity of the program."}, flag_verbose_handle},
	{{ 'h', "help", NO_FLAG_ARG, "Shows this help page and exits."}, flag_help_handle},
	{{ 'f', "file", MANDATORY_FLAG_ARG, "Sets the file used as input"}, flag_file_option_handle},
	{{ '\0', "", STR_FLAG_ARG, ""}, flag_str_option_handle},
	{{ 0, NULL, 0, NULL}, NULL}
};
```
An empty value will be interpreted as a default `string option handler`

Except for the last index, no **Flag Prototype** should contain `NULL` values. In such a case, the library will throw an error.
A missing `NULL` **prototype terminator** or **flag_str_option_handle** will result in undefined behavior.

The library can be configured using this structure
```c
typedef struct cli_flag_config
{
	bool	allow_double_dash_separator;
	bool	positional_argument_order_sensitive;
}	cli_flag_config;
```

`allow_double_dash_separator` enables the usage of the `--` flag that indicates following arguments should be considered as strings. See the [Double Dash separator example](#the----argument-separator) for more details.

When `positional_argument_order_sensitive` is enabled, the first non **flag option** argument encountered will be interpreted as a **double dash separator**. Following arguments will be interpreted as strings. See the [Positional Argument Order example](#positional-argument-order) for more details.

A valid **Flag Config structure** should look like this
```c
const cli_flag_config flag_config = { true, false };
```

The **Flags Prototype Array** and **Flag Config Structure** must be placed at the bottom of the user's [arglib_handlers.c](./srcs/main/arglib_handlers.c) file.

## Using arglib

To implement a custom flag you must add a new **Flag Prototype** to the **Flag Prototype Array** and pass your **Flag Handler Function** as parameter. Your **cli_userdata** structure must reflect the behavior intended by your flag.

Here are two simple use cases from `man rm`
```c
// arglib_handlers.c
const cli_flag_handler flags[] = {
	{{ 'f', "force", NO_FLAG_ARG, "ignore nonexistent files and arguments, never prompt"}, flag_force_handle},
	{{ '', "no-preserve-root", NO_FLAG_ARG, "do not treat '/' specially"}, flag_no_preserve_root_handle},
	[...]
	{{ '\0', "", STR_FLAG_ARG, ""}, flag_str_option_handle},
	{{ 0, NULL, 0, NULL}, NULL}
};

// main.h
typedef struct cli_userdata
{
	bool	flag_force;
	bool	flag_no_preserve_root;
	[...]
	bool	flag_cut_str;
	bool	flag_str;

	char	*str;
} cli_userdata;
```

These **handlers** would look like this
```c
// arglib_handlers.c
static cli_context *flag_force_handle(cli_context *ctx, char *clean_arg, char **argv, bool parameter)
{
	ctx = new_context(ctx);
	if (ctx == NULL)
		return (free_arglib_node(ctx), NULL);

	((cli_userdata*)ctx->userdata)->flag_force = true;

	(void)clean_arg;
	(void)argv;
	(void)parameter;
	return (ctx);
}

static cli_context *flag_no_preserve_root_handle(cli_context *ctx, char *clean_arg, char **argv, bool parameter)
{
	ctx = new_context(ctx);
	if (ctx == NULL)
		return (free_arglib_node(ctx), NULL);

	((cli_userdata*)ctx->userdata)->flag_no_preserve_root = true;

	(void)clean_arg;
	(void)argv;
	(void)parameter;
	return (ctx);
}
```

So if our program call were to look like this
```sh
program -f --no-preserve-root
```

Then the linked list would have `flag_force` and `flag_no_preserve_root` as true.

Most handlers are simple and only toggle their respective boolean but some can be more complex.

The **File Flag Handler** implemented [here](./srcs/main/arglib_handlers.c) manages file inputs.

```sh
# These examples are all valid
program -f <file>
program --file <file>

program -f<file>
program --file=<file>
```

The **File Flag Handler** will create a `File Flag Node` containing nothing except the `flag_infile` boolean set to true. In the first two examples, a `Str Flag Node` containing the `<file>` string will be created on the next iteration. The last two examples have the **File Flag Handler** create these `Str Flag Node` itself.

These 4 examples would give the same linked list

|node|->|node|
|-|-|-|
|`file (flag_infile == true)`||`str == <file>`|

In case of multiple `<file>` arguments like this
```sh
# in this example Positional Argument Order is disabled
program -f file1 file2 file3 --file file4
```
The linked list would look like this
|node|->|node|->|node|->|node|->|node|->|node|
|-|-|-|-|-|-|-|-|-|-|-|
|`file`||`str == file1`||`str == file2`||`str == file3`||`file`||`str == file4`|

As these infiles are simply treated as `String Nodes`, it is up to the user to interpret them as wished in their program.

Depending on needs, the program might interpret only the first `Str Flag Node` following a `File Flag Node` to be a file, in this case only `file1` and `file4` would be considered files.

## Memory management

The arglib does not allocate nor free anything. It is up to the user, when building their **handlers** and **userdata** to manage their allocations, error handling and freeing everything in the end.
A **handler** returning `NULL` on error is the expected behavior.

Memory management function examples can be found in the [arglib_handlers.c](./srcs/main/arglib_handlers.c) file.

## Examples

### The '--' argument separator

```sh
touch -- -rf # This creates a file named `-rf`
rm -- -rf # This deletes a file named `-rf`
```

### Positional argument order

```sh
# Disabled
example -a file1 -r file2	# Only file1 and file2 will be interpreted as non flag arguments

# Enabled
example -a file1 -r file2	# file1 '-r' and file2 will be interpreted as non flag arguments
```
