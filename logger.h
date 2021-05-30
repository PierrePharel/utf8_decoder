#ifndef UTF8_DECODER_LOGGER_H
#define UTF8_DECODER_LOGGER_H

#define INFO_COLOR ("\x1b[32m")
#define WARNING_COLOR ("\x1b[31m")
#define RESET_COLOR ("\x1b[0m")

#define INFO 0
#define WARNING 1

#include <stdio.h>

#define Log(type, ...)\
{\
    if (type == INFO)\
        printf("%s%s: ", INFO_COLOR, __func__);\
    else\
        printf("%s%s: ", WARNING_COLOR, __func__);\
    printf(__VA_ARGS__);\
    printf("\n");\
    printf(RESET_COLOR);\
}

#endif