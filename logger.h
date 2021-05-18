#ifndef UTF8_DECODER_LOGGER_H
#define UTF8_DECODER_LOGGER_H

#define INFO_COLOR ("\x1b[32m")
#define WARNING_COLOR ("\x1b[31m")
#define RESET_COLOR ("\x1b[0m")

#define INFO 0
#define WARNING 1

#include <stdio.h>

#define Log(type, format...)\
{\
    if (type == INFO)\
        printf("%s%s::%s: ", INFO_COLOR, "Info", __func__);\
    else\
        printf("%s%s::%s: ", WARNING_COLOR, "Warning", __func__);\
    printf(format);\
    printf(RESET_COLOR);\
}

#endif