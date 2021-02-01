#ifndef UTF8_DECODER
#define UTF8_DECODER

#include <stdlib.h>

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

// str bytes beginning
#define LATIN_BEGIN ("110")
#define BASIC_MUL_LANG_BEGIN ("1110")
#define OTHERS_PLANESU_BEGIN ("111100")
#define SCY_CHR_BEGIN ("10")

#define END ('\0')

enum UTF8Type
{
    USASCII,
    Latin,
    BasicMultiLang,
    Others,
    OutRange
};
typedef enum UTF8Type UTF8Type_t;

UTF8Type_t utf8type(const char *hex_str);
void hex_to_bytes_str(const char *hex_str, char *bytes_str);
void bytes_to_utf8chr_str(UTF8Type_t type, char *utf8_chr_str);
void utf8decode(const char *src, char *dst);
void str_to_bit_decoded(const char *utf8_chr_str, char *utf8_str);

/* ghost functions */
void copy(const short begin, char* const src, char *dst)
{
    const char *s = (src + begin);
    short i = 0;

    for(; *s != END; ++ i, ++ s)
        dst[i] = *s;

    dst[i] = END;
    src[0] = END;
}

void append(char *dst, const char *str)
{
    short i = 0;

    for(char *s = dst; *s != END; ++ s, ++ i);

    for(const char *s = str; *s != END; ++ s, ++i)
        dst[i] = *s;

    dst[i] = END;
}

UTF8Type_t utf8type(const char *hex_str)
{
    UTF8Type_t type;
    int32_t codepoint = 0;
    short shift = 0;
    const char *s = hex_str;

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
        type = OutRange;

    return type;
}

void hex_to_bytes_str(const char *hex_str, char *bytes_str)
{
    const char *s = hex_str;

    while(*s != END)
    {
        switch(*s)
        {
            case '0': append(bytes_str, "0000");
                break;
            case '1': append(bytes_str, "0001");
                break;
            case '2': append(bytes_str, "0010");
                break;
            case '3': append(bytes_str, "0011");
                break;
            case '4': append(bytes_str, "0100");
                break;
            case '5': append(bytes_str, "0101");
                break;
            case '6': append(bytes_str, "0110");
                break;
            case '7': append(bytes_str, "0111");
                break;
            case '8': append(bytes_str, "1000");
                break;
            case '9': append(bytes_str, "1001");
                break;
            case 'A': append(bytes_str, "1010");
                break;
            case 'B': append(bytes_str, "1011");
                break;
            case 'C': append(bytes_str, "1100");
                break;
            case 'D': append(bytes_str, "1101");
                break;
            case 'E': append(bytes_str, "1110");
                break;
            case 'F': append(bytes_str, "1111");
                break;
        }

        ++ s;
    }
}

void bytes_to_utf8chr_str(UTF8Type_t type, char *utf8_chr_str)
{
    char bytes_str[33];

    switch(type)
    {
        case Latin:
        {
            copy(5, utf8_chr_str, bytes_str);
            append(utf8_chr_str, LATIN_BEGIN);
            for(short i = 0; bytes_str[i] != END; ++ i)
                if(i < 5) // first char
                {
                    utf8_chr_str[i + 3] = bytes_str[i];
                    utf8_chr_str[i + 4] = END;
                }
                else if(i == 5) // transition
                {
                    append(utf8_chr_str, SCY_CHR_BEGIN);
                    utf8_chr_str[i + 5] = bytes_str[i];
                    utf8_chr_str[i + 6] = END;
                }
                else // last char
                {
                    utf8_chr_str[i + 5] = bytes_str[i];
                    utf8_chr_str[i + 6] = END;
                }

            break;
        }

        case BasicMultiLang:
        {
            copy(0, utf8_chr_str, bytes_str);
            append(utf8_chr_str, BASIC_MUL_LANG_BEGIN);
            for(short i = 0; bytes_str[i] != END; ++ i)
                if(i < 4) // first char
                {
                    utf8_chr_str[i + 4] = bytes_str[i];
                    utf8_chr_str[i + 5] = END;
                }
                else if(i == 4) // transition
                {
                    append(utf8_chr_str, SCY_CHR_BEGIN);
                    utf8_chr_str[i + 6] = bytes_str[i];
                    utf8_chr_str[i + 7] = END;
                }
                else if(i > 4 && i < 10) // second char
                {
                    utf8_chr_str[i + 6] = bytes_str[i];
                    utf8_chr_str[i + 7] = END;
                }
                else if(i == 10) // transition
                {
                    append(utf8_chr_str, SCY_CHR_BEGIN);
                    utf8_chr_str[i + 8] = bytes_str[i];
                    utf8_chr_str[i + 9] = END;
                }
                else // last char
                {
                    utf8_chr_str[i + 8] = bytes_str[i];
                    utf8_chr_str[i + 9] = END;
                }

            break;
        }

        case Others:
        {
            copy(0, utf8_chr_str, bytes_str);
            append(utf8_chr_str, OTHERS_PLANESU_BEGIN);
            for(short i = 0; bytes_str[i] != END; ++ i)
                if(i < 2) // first char
                {
                    utf8_chr_str[i + 6] = bytes_str[i];
                    utf8_chr_str[i + 7] = END;
                }
                else if(i == 2) // transition
                {
                    append(utf8_chr_str, SCY_CHR_BEGIN);
                    utf8_chr_str[i + 8] = bytes_str[i];
                    utf8_chr_str[i + 9] = END;
                }
                else if(i > 2 && i < 8)
                {
                    utf8_chr_str[i + 8] = bytes_str[i];
                    utf8_chr_str[i + 9] = END;
                }
                else if(i == 8) // transition
                {
                    append(utf8_chr_str, SCY_CHR_BEGIN);
                    utf8_chr_str[i + 10] = bytes_str[i];
                    utf8_chr_str[i + 11] = END;
                }
                else if(i > 8 && i < 14)
                {
                    utf8_chr_str[i + 10] = bytes_str[i];
                    utf8_chr_str[i + 11] = END;
                }
                else if(i == 14) // transition
                {
                    append(utf8_chr_str, SCY_CHR_BEGIN);
                    utf8_chr_str[i + 12] = bytes_str[i];
                    utf8_chr_str[i + 13] = END;
                }
                else // last char
                {
                    utf8_chr_str[i + 12] = bytes_str[i];
                    utf8_chr_str[i + 13] = END;
                }

            break;
        }
    }
}

void str_to_bit_decoded(const char *utf8_chr_str, char *utf8_str)
{
    short i = 0;
    short j = 0;

    while(utf8_chr_str[i] != END)
    {
        switch(utf8_chr_str[i])
        {
            case '0':
                utf8_str[j] <<= 1;
                utf8_str[j] |= 0b0000'0000;
                break;
            case '1':
                utf8_str[j] <<= 1;
                utf8_str[j] |= 0b0000'0001;
                break;
        }

        if(((i + 1) % 8) == 0)
            ++ j;

        ++ i;
    }

    utf8_str[j + 1] = END;
}

void utf8decode(const char *hex_str, char *utf8_str)
{
    const UTF8Type_t type = utf8type(hex_str);
    char utf8_chr_str[33] = {0};

    switch(type)
    {
        case USASCII:
            utf8_str[0] = utf8type(hex_str);
            utf8_str[1] = END;
            break;
        case Latin:
        case BasicMultiLang:
        case Others:
            hex_to_bytes_str(hex_str, utf8_chr_str);
            bytes_to_utf8chr_str(type, utf8_chr_str);
            str_to_bit_decoded(utf8_chr_str, utf8_str);
            break;
        case OutRange:
            utf8_str[0] = END;
            break;
    }
}

#endif