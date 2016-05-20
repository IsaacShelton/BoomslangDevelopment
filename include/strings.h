
#ifndef STRINGS_H_INCLUDED
#define STRINGS_H_INCLUDED

#include <string>

bool string_contains(std::string, std::string);
std::string string_get_until(std::string, std::string);
std::string string_delete_until(std::string, std::string);
std::string string_get_until_or(std::string, std::string);
std::string string_delete_until_or(std::string, std::string);
std::string string_delete_amount(std::string, int);
unsigned int string_count(std::string, std::string);
std::string string_replace(std::string, const std::string, const std::string);
std::string string_replace_all(std::string, const std::string, const std::string);
std::string string_kill_whitespace(std::string);
std::string string_kill_all_whitespace(std::string);
std::string string_kill_newline(std::string);
std::string resource(std::string);
std::string delete_backslash(std::string a);
std::string string_upper(std::string a);
bool is_identifier(std::string what);
bool is_identifier(std::string what, std::string extras);
bool is_indent(std::string what);
std::string string_get_until_last(std::string, std::string);
std::string filename_name(std::string);
std::string filename_path(std::string);
std::string filename_change_ext(std::string, std::string);

#endif // STRINGS_H_INCLUDED
