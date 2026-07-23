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
A missing `NULL` **prototype terminator** will result in undefined behavior.

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
