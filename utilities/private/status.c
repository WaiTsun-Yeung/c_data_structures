#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void cds_print_error_message(
    const errno_t error_code, 
    const char* const filename, const int line_number, 
    const char* const function_name,
    const char* const context_description
){
    #ifdef _WIN32
        const size_t error_message_length = 128;
    #else
        const size_t error_message_length = strerrorlen_s(error_code) + 1;
    #endif
    char* const error_message = (char*)malloc(error_message_length);
    const errno_t get_error_message_error = strerror_s(
        error_message, error_message_length, error_code
    );
    if (get_error_message_error) 
        (void)printf_s(
            "%s:%d:%s, %s error\n", 
            filename, line_number, function_name, context_description
        );
    else (void)printf_s(
        "%s:%d:%s, %s error: %s\n", 
        filename, line_number, function_name, context_description, error_message
    );
    free(error_message);
}