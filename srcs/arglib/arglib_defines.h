#ifndef ARGLIB_DEFINES_H
#define ARGLIB_DEFINES_H

#include "arglib.h"

const char				*is_cli_flag_handler_valid(const cli_flag_handler *flags);
bool					set_double_dash_separator();
size_t					get_flag_list_length();
cli_context				*get_first_ctx_node(cli_context *ctx);
flag_handler_function	get_str_flag_handler(size_t flag_list_length);

#endif
