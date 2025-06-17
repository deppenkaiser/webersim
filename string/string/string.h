#pragma once

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define STRING_MAXLEN 4096
#define STRING_ERROR -1

typedef char string_t[STRING_MAXLEN];

int32_t string_copy(char* destination, size_t destinantion_size_bytes, const char* source);
int32_t string_cat(char* destination, size_t destinantion_size_bytes, const char* source);
const char* string_dirname_from_filepath(char* filepath);
bool string_directory_exists(const char* directory);
void string_directory_create(const char* directory);
bool string_filepath_exist(char* filepath);
const char* string_append_directory_to_path_and_create(const char* path, const char* directory);
int32_t string_calculate_substring_index(const char* string, const char* sub_string);

// ANSI-Farbcodes
#define COLOR_RED     "\x1b[31m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RESET   "\x1b[0m"

// Formatierung
#define TEXT_BOLD    "\x1b[1m"
#define TEXT_UNDER   "\x1b[4m"

// Hintergrundfarben
#define BG_RED      "\x1b[41m"
#define BG_GREEN    "\x1b[42m"

