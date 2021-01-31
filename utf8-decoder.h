#ifndef UTF8_DECODER
#define UTF8_DECODER

#define _0 0b0000'0000'0000'0000'0000'0000'0000'0000
#define _1 0b0000'0000'0000'0000'0000'0000'0000'0001
#define _2 0b0000'0000'0000'0000'0000'0000'0000'0010
#define _3 0b0000'0000'0000'0000'0000'0000'0000'0011
#define _4 0b0000'0000'0000'0000'0000'0000'0000'0100
#define _5 0b0000'0000'0000'0000'0000'0000'0000'0101
#define _6 0b0000'0000'0000'0000'0000'0000'0000'0110
#define _7 0b0000'0000'0000'0000'0000'0000'0000'0111
#define _8 0b0000'0000'0000'0000'0000'0000'0000'1000
#define _9 0b0000'0000'0000'0000'0000'0000'0000'1001
#define _A 0b0000'0000'0000'0000'0000'0000'0000'1010
#define _B 0b0000'0000'0000'0000'0000'0000'0000'1011
#define _C 0b0000'0000'0000'0000'0000'0000'0000'1100
#define _D 0b0000'0000'0000'0000'0000'0000'0000'1101
#define _E 0b0000'0000'0000'0000'0000'0000'0000'1110
#define _F 0b0000'0000'0000'0000'0000'0000'0000'1111

// first char
#define LATIN 0xc0
#define BASIC_MULTI_LANG 0xe0
#define OTHERS 0xf0

#define END ('\0')

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


enum UTF8Type
{
    USASCII,
    Latin,
    BasicMultiLang,
    Others,
    Unknown
};
typedef enum UTF8Type UTF8Type_t;

UTF8Type_t utf8type(const char *str);
void utf8layout(const char *src, char *dst, UTF8Type_t type);
void bytes_to_str(const char *src, char *buf);
void chr_to_str(char *buf);
void utf8decode(const char *src, char *dst);

/* ghost functions */
void copy(short begin, const char *src, char *dst)
{
    const char *s = (src + begin);
    short i = 0;

    for(; *s != END; ++ i, ++ s)
        dst[i] = *s;

    dst[i] = END;
}

void append(char *buf, const char *str)
{
    short i = 0;

    for(char *s = buf; *s != END; ++ s, ++ i);

    for(const char *s = str; *s != END; ++ s, ++i)
        buf[i] = *s;

    buf[i] = END;
}

UTF8Type_t utf8type(const char *str)
{
    UTF8Type_t type;
    int32_t codepoint = 0;
    short shift = 0;
    const char *s = str;

    while(*s != END)
    {
        switch(*s)
        {
            case '0': codepoint = ((codepoint << shift) | _0);
                    break;
            case '1': codepoint = ((codepoint << shift) | _1);
                    break;
            case '2': codepoint = ((codepoint << shift) | _2);
                    break;
            case '3': codepoint = ((codepoint << shift) | _3);
                    break;
            case '4': codepoint = ((codepoint << shift) | _4);
                    break;
            case '5': codepoint = ((codepoint << shift) | _5);
                    break;
            case '6': codepoint = ((codepoint << shift) | _6);
                    break;
            case '7': codepoint = ((codepoint << shift) | _7);
                    break;
            case '8': codepoint = ((codepoint << shift) | _8);
                    break;
            case '9': codepoint = ((codepoint << shift) | _9);
                    break;
            case 'A': codepoint = ((codepoint << shift) | _A);
                    break;
            case 'B': codepoint = ((codepoint << shift) | _B);
                    break;
            case 'C': codepoint = ((codepoint << shift) | _C);
                    break;
            case 'D': codepoint = ((codepoint << shift) | _D);
                    break;
            case 'E': codepoint = ((codepoint << shift) | _E);
                    break;
            case 'F': codepoint = ((codepoint << shift) | _F);
                    break;
        }

        shift = 4;
        ++ s;
    }

    if(codepoint <= 0x007f)
        type = USASCII;
    else if(codepoint > 0x007f && codepoint <= 0x07ff)
        type = Latin;
    else if(codepoint > 0x07ff && codepoint <= 0xffff)
        type = BasicMultiLang;
    else if(codepoint > 0xffff && codepoint <= 0x10ffff)
        type = Others;
    else
        type = Unknown;

    return type;
}

void bytes_to_str(const char *src, char *buf)
{
    for(short i = 0; i < strlen(src); ++ i)
        switch(src[i])
        {
            case '0': strcat(buf, "0000");
                break;
            case '1': strcat(buf, "0001");
                break;
            case '2': strcat(buf, "0010");
                break;
            case '3': strcat(buf, "0011");
                break;
            case '4': strcat(buf, "0100");
                break;
            case '5': strcat(buf, "0101");
                break;
            case '6': strcat(buf, "0110");
                break;
            case '7': strcat(buf, "0111");
                break;
            case '8': strcat(buf, "1000");
                break;
            case '9': strcat(buf, "1001");
                break;
            case 'A': strcat(buf, "1010");
                break;
            case 'B': strcat(buf, "1011");
                break;
            case 'C': strcat(buf, "1100");
                break;
            case 'D': strcat(buf, "1101");
                break;
            case 'E': strcat(buf, "1110");
                break;
            case 'F': strcat(buf, "1111");
                break;
        }
}

void to_type(char *buf, UTF8Type_t type)
{
    
}

void utf8layout(const char *src, char *dst, UTF8Type_t type)
{
    char buf[32] = {0};

    bytes_to_str(src, buf);
    switch(type)
    {
        case Latin:

            break;
    }
}

void utf8decode(const char *src, char *dst)
{
    const UTF8Type_t type = utf8type(src);

    
}

#endif