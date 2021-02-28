// The MIT License (MIT)

// Copyright (c) 2021 Pierre Pharel Angoro Abiaga

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef UTF8_DECODER
#define UTF8_DECODER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

// str bytes beginning
#define LATIN_BEGIN ("110")
#define BASIC_MUL_LANG_BEGIN ("1110")
#define OTHERS_PLANESU_BEGIN ("111100") // must be 0xf0 (0b11110xxx), but '0' add for 32bit align
#define SCY_CHR_BEGIN ("10")

#define STR_END ('\0')
#define utf8_bad_char 0
#define utf8_good_char 1

enum UTF8Type
{
    utf8_USASCII_t,
    utf8_Latin_t,
    utf8_BasicMultiLang_t,
    utf8_Others_t,
    utf8_OutRange_t
};
typedef enum UTF8Type UTF8Type_t;

static UTF8Type_t utf8type(const char *hex_str);
static void hex_to_bytes_str(const char *hex_str, char *bytes_str);
static void bytes_to_utf8chr_str(UTF8Type_t type, char *utf8_chr_str);
static void str_to_bit_decoded(const char *utf8_chr_str, char *utf8_str);
static void utf8decode(const char *src, char *dst);

/* ghost functions */
static void utf8d_copy(const short begin, char* const src, char *dst)
{
    const char *s = (src + begin);
    short i = 0;

    for(; *s != STR_END; ++ i, ++ s)
        dst[i] = *s;

    dst[i] = STR_END;
    src[0] = STR_END;
}

static void utf8d_append(char *dst, const char *str)
{
    short i = 0;

    for(char *s = dst; *s != STR_END; ++ s, ++ i);

    for(const char *s = str; *s != STR_END; ++ s, ++i)
        dst[i] = *s;

    dst[i] = STR_END;
}

static UTF8Type_t utf8type(const char *hex_str)
{
    UTF8Type_t type;
    int32_t codepoint = 0;
    short shift = 0;
    const char *s = hex_str;

    while(*s != STR_END)
    {
        switch(*s)
        {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': codepoint = ((codepoint << shift) | (*s - '0'));
                    break;

            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case 'E':
            case 'F': codepoint = ((codepoint << shift) | (*s - 'A' + 10));
                    break;

            case 'a':
            case 'b':
            case 'c':
            case 'd':
            case 'e':
            case 'f': codepoint = ((codepoint << shift) | (*s - 'a' + 10));
                    break;
        }

        shift = 4;
        ++ s;
    }

    if(codepoint <= 0x007f)
        type = utf8_USASCII_t;
    else if(codepoint > 0x007f && codepoint <= 0x07ff)
        type = utf8_Latin_t;
    else if(codepoint > 0x07ff && codepoint <= 0xffff)
        type = utf8_BasicMultiLang_t;
    else if(codepoint > 0xffff && codepoint <= 0x10ffff)
        type = utf8_Others_t;
    else
        type = utf8_OutRange_t;

    return type;
}

static void hex_to_bytes_str(const char *hex_str, char *bytes_str)
{
    const char *s = hex_str;

    while(*s != STR_END)
    {
        switch(*s)
        {
            case '0': utf8d_append(bytes_str, "0000");
                break;
            case '1': utf8d_append(bytes_str, "0001");
                break;
            case '2': utf8d_append(bytes_str, "0010");
                break;
            case '3': utf8d_append(bytes_str, "0011");
                break;
            case '4': utf8d_append(bytes_str, "0100");
                break;
            case '5': utf8d_append(bytes_str, "0101");
                break;
            case '6': utf8d_append(bytes_str, "0110");
                break;
            case '7': utf8d_append(bytes_str, "0111");
                break;
            case '8': utf8d_append(bytes_str, "1000");
                break;
            case '9': utf8d_append(bytes_str, "1001");
                break;
            case 'a':
            case 'A': utf8d_append(bytes_str, "1010");
                break;
            case 'b':
            case 'B': utf8d_append(bytes_str, "1011");
                break;
            case 'c':
            case 'C': utf8d_append(bytes_str, "1100");
                break;
            case 'd':
            case 'D': utf8d_append(bytes_str, "1101");
                break;
            case 'e':
            case 'E': utf8d_append(bytes_str, "1110");
                break;
            case 'f':
            case 'F': utf8d_append(bytes_str, "1111");
                break;
        }

        ++ s;
    }
}

static void bytes_to_utf8chr_str(UTF8Type_t type, char *utf8_chr_str)
{
    char bytes_str[33];

    switch(type)
    {
        case utf8_Latin_t:
        {
            utf8d_copy(5, utf8_chr_str, bytes_str);
            utf8d_append(utf8_chr_str, LATIN_BEGIN);
            for(short i = 0; bytes_str[i] != STR_END; ++ i)
                if(i < 5) // first char
                {
                    utf8_chr_str[i + 3] = bytes_str[i];
                    utf8_chr_str[i + 4] = STR_END;
                }
                else if(i == 5) // transition
                {
                    utf8d_append(utf8_chr_str, SCY_CHR_BEGIN);
                    utf8_chr_str[i + 5] = bytes_str[i];
                    utf8_chr_str[i + 6] = STR_END;
                }
                else // last char
                {
                    utf8_chr_str[i + 5] = bytes_str[i];
                    utf8_chr_str[i + 6] = STR_END;
                }

            break;
        }

        case utf8_BasicMultiLang_t:
        {
            utf8d_copy(0, utf8_chr_str, bytes_str);
            utf8d_append(utf8_chr_str, BASIC_MUL_LANG_BEGIN);
            for(short i = 0; bytes_str[i] != STR_END; ++ i)
                if(i < 4) // first char
                {
                    utf8_chr_str[i + 4] = bytes_str[i];
                    utf8_chr_str[i + 5] = STR_END;
                }
                else if(i == 4) // transition
                {
                    utf8d_append(utf8_chr_str, SCY_CHR_BEGIN);
                    utf8_chr_str[i + 6] = bytes_str[i];
                    utf8_chr_str[i + 7] = STR_END;
                }
                else if(i > 4 && i < 10) // second char
                {
                    utf8_chr_str[i + 6] = bytes_str[i];
                    utf8_chr_str[i + 7] = STR_END;
                }
                else if(i == 10) // transition
                {
                    utf8d_append(utf8_chr_str, SCY_CHR_BEGIN);
                    utf8_chr_str[i + 8] = bytes_str[i];
                    utf8_chr_str[i + 9] = STR_END;
                }
                else // last char
                {
                    utf8_chr_str[i + 8] = bytes_str[i];
                    utf8_chr_str[i + 9] = STR_END;
                }

            break;
        }

        case utf8_Others_t:
        {
            utf8d_copy(0, utf8_chr_str, bytes_str);
            utf8d_append(utf8_chr_str, OTHERS_PLANESU_BEGIN);
            for(short i = 0; bytes_str[i] != STR_END; ++ i)
                if(i < 2) // first char
                {
                    utf8_chr_str[i + 6] = bytes_str[i];
                    utf8_chr_str[i + 7] = STR_END;
                }
                else if(i == 2) // transition
                {
                    utf8d_append(utf8_chr_str, SCY_CHR_BEGIN);
                    utf8_chr_str[i + 8] = bytes_str[i];
                    utf8_chr_str[i + 9] = STR_END;
                }
                else if(i > 2 && i < 8)
                {
                    utf8_chr_str[i + 8] = bytes_str[i];
                    utf8_chr_str[i + 9] = STR_END;
                }
                else if(i == 8) // transition
                {
                    utf8d_append(utf8_chr_str, SCY_CHR_BEGIN);
                    utf8_chr_str[i + 10] = bytes_str[i];
                    utf8_chr_str[i + 11] = STR_END;
                }
                else if(i > 8 && i < 14)
                {
                    utf8_chr_str[i + 10] = bytes_str[i];
                    utf8_chr_str[i + 11] = STR_END;
                }
                else if(i == 14) // transition
                {
                    utf8d_append(utf8_chr_str, SCY_CHR_BEGIN);
                    utf8_chr_str[i + 12] = bytes_str[i];
                    utf8_chr_str[i + 13] = STR_END;
                }
                else // last char
                {
                    utf8_chr_str[i + 12] = bytes_str[i];
                    utf8_chr_str[i + 13] = STR_END;
                }

            break;
        }
    }
}

static void str_to_bit_decoded(const char *utf8_chr_str, char *utf8_str)
{
    short i = 0;
    short j = 0;

    while(utf8_chr_str[i] != STR_END)
    {
        utf8_str[j] <<= 1;
        utf8_str[j] |= utf8_chr_str[i] - '0';

        if(((i + 1) % 8) == 0)
            ++ j;

        ++ i;
    }

    utf8_str[j + 1] = STR_END;
}

static void utf8decode(const char *hex_str, char *utf8_str)
{
    const UTF8Type_t type = utf8type(hex_str);
    char utf8_chr_str[33] = {0};

    switch(type)
    {
        case utf8_USASCII_t:
            utf8_str[0] = utf8type(hex_str);
            utf8_str[1] = STR_END;
            break;
        case utf8_Latin_t:
        case utf8_BasicMultiLang_t:
        case utf8_Others_t:
            hex_to_bytes_str(hex_str, utf8_chr_str);
            bytes_to_utf8chr_str(type, utf8_chr_str);
            str_to_bit_decoded(utf8_chr_str, utf8_str);
            break;
        case utf8_OutRange_t:
            utf8_str[0] = STR_END;
            break;
    }
}

/* Bonus function */
static short utf8valid(const char *str);

static short utf8valid(const char *str)
{
    const char *s = str;

    while('\0' != *s)
    {
        if(0xf0 == (0xf8 & *s))
        {
            // ensure each of the 3 following bytes in this 4-byte
            // utf8 codepoint began with 0b10xxxxxx
            if((0x80 != (0xc0 & s[1])) || (0x80 != (0xc0 & s[2])) || (0x80 != (0xc0 & s[3])))
                return utf8_bad_char;

            // ensure that our utf8 codepoint ended after 4 bytes
            if(0x80 == (0xc0 & s[4]))
                return utf8_bad_char;

            // ensure that the top 5 bits of this 4-byte utf8
            // codepoint were not 0, as then we could have used
            // one of the smaller encodings
            if((0 == (0x07 & s[0])) && (0 == (0x30 & s[1])))
                return utf8_bad_char;

            // 4-byte utf8 code point (began with 0b11110xxx)
            s += 4;
        }
        else if(0xe0 == (0xf0 & *s))
        {
            // ensure each of the 2 following bytes in this 3-byte
            // utf8 codepoint began with 0b10xxxxxx
            if((0x80 != (0xc0 & s[1])) || (0x80 != (0xc0 & s[2])))
                return utf8_bad_char;

            // ensure that our utf8 codepoint ended after 3 bytes
            if(0x80 == (0xc0 & s[3]))
                return utf8_bad_char;

            // ensure that the top 5 bits of this 3-byte utf8
            // codepoint were not 0, as then we could have used
            // one of the smaller encodings
            if((0 == (0x0f & s[0])) && (0 == (0x20 & s[1])))
                return utf8_bad_char;

            // 3-byte utf8 code point (began with 0b1110xxxx)
            s += 3;
        }
        else if(0xc0 == (0xe0 & *s))
        {
            // ensure the 1 following byte in this 2-byte
            // utf8 codepoint began with 0b10xxxxxx
            if(0x80 != (0xc0 & s[1]))
                return utf8_bad_char;

            // ensure that our utf8 codepoint ended after 2 bytes
            if (0x80 == (0xc0 & s[2]))
                return utf8_bad_char;

            // ensure that the top 4 bits of this 2-byte utf8
            // codepoint were not 0, as then we could have used
            // one of the smaller encodings
            if(0 == (0x1e & s[0]))
                return utf8_bad_char;

            // 2-byte utf8 code point (began with 0b110xxxxx)
            s += 2;
        }
        else if(0x00 == (0x80 & *s))
        {
            // 1-byte ascii (began with 0b0xxxxxxx)
            s += 1;
        }
        else
        {
            // we have an invalid 0b1xxxxxxx utf8 code point entry
            return utf8_bad_char;
        }
    }

    return utf8_good_char;
}

#undef STR_END
#undef LATIN_BEGIN
#undef BASIC_MUL_LANG_BEGIN
#undef OTHERS_PLANESU_BEGIN
#undef SCY_CHR_BEGIN

#ifdef __cplusplus
} // extern "C"
#endif

#endif
