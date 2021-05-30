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

#ifndef UTF8_DECODER_H
#define UTF8_DECODER_H

#if defined (_MSC_VER)
    typedef __int32 int32_t;
    #pragma warning (disable: 4309)
#else
    #include <stdint.h>
#endif

#include <stdbool.h>

#if defined (UTF8_DECODER_LOG)
    #include "logger.h"
#endif

#define LATIN_EXTRA_BEGIN 0xc0 // 0b110xxxxx
#define BASIC_MULTILINGUAL_BEGIN 0xe0 // 0b1110xxxx
#define OTHERS_PLANES_UNICODE_BEGIN 0xf0 // 0b11110xxxx
#define SECONDARY_CHAR_BEGIN 0x80 // 0b10xxxxxx

#define END ('\0')
#define UTF8_BAD_CHAR false
#define UTF8_GOOD_CHAR true

typedef enum
{
    US_ASCII,
    LatinExtra,
    BasicMultiLingual,
    OthersPlanesUnicode,
    OutRange
} Utf8Type;

// Return a char decimal value greater than or equal to zero and less than zero if char is out of range
// Convert hexadecimal char beetwen '0'-'F'('a'-'z' & 'A'-'Z') in decimal value
static char hexchr_to_hex(const char hex_chr);

// Return a the Utf8Type defined above
// But you can get codepoint (decimal value) of hexadecimal string if you pass an int32_t pointer instead of NULL
// Convert hexadecimal string in a decimal value, and determinate what is the range of utf8 string
static Utf8Type utf8type(const char* hex_str, int32_t* cdp);

// Convert hexadecimal utf8 string into usable utf8 string and store it in dest
// dest will be empty if hexadecimal string is invalid or out of range
// NB: min size of dest must be 2 and max 5 with null char
static void utf8decode(const char* hex_str, char* dest);

// Return a bool (true, if valid and false otherwise)
// Check if str is a valid utf8 string
static bool utf8valid(const char* str);

// Return codepoint greater than or equal to zero and less than zero if char is out of range or if string is NULL
// Convert utf8 string in codepoint (decimal value)
static int32_t utf8codepoint(const char* str);

// Convert codepoint into usable utf8 string and store it in dest
// dest will be empty if hexadecimal string is invalid or out of range
// NB: min size of dest must be 2 and max 5 with null char
static void utf8chr(const int32_t codepoint, char* dest);


static Utf8Type utf8type(const char* hex_str, int32_t* cdp)
{
    int32_t codepoint = 0;
    short shift = 0;

    for (const char* s = hex_str; *s != END; ++ s)
    {
        codepoint = ((codepoint << shift) | hexchr_to_hex(*s));
        shift = 4;
    }

    if (cdp != NULL)
        *cdp = codepoint;

    if (codepoint >= 0x0000 && codepoint <= 0x007f)
        return US_ASCII;
    else if (codepoint > 0x007f && codepoint <= 0x07ff)
        return LatinExtra;
    else if (codepoint > 0x07ff && codepoint <= 0xffff)
        return BasicMultiLingual;
    else if (codepoint > 0xffff && codepoint <= 0x10ffff)
        return OthersPlanesUnicode;

    return OutRange;
}

static char hexchr_to_hex(const char hex_chr)
{
    if ('0' <= hex_chr && hex_chr <= '9')
        return hex_chr - 48;
    if ('A' <= hex_chr && hex_chr <= 'F')
        return hex_chr - 55;
    if ('a' <= hex_chr && hex_chr <= 'f')
        return hex_chr - 87;
    return -1;
}

static void utf8decode(const char* hex_str, char* dest)
{
    int32_t cdp = 0;
    Utf8Type type = utf8type(hex_str, &cdp);
    // those are common to all the utf8 types
    char c1 = hexchr_to_hex(hex_str[1]);
    char c2 = hexchr_to_hex(hex_str[2]);
    char c3 = hexchr_to_hex(hex_str[3]);

    switch (type)
    {
        case US_ASCII:
        {
            int32_t c0 = 0;
            utf8type(hex_str, &c0);
            dest[0] = c0;
            dest[1] = END;

#if defined (UTF8_DECODER_LOG)
            Log(INFO, "%X", dest[0]);
#endif
            break;
        }

        case LatinExtra:
        {
            // first char
            dest[0] = LATIN_EXTRA_BEGIN;
            dest[0] |= ((c1 & 0x7) << 2);
            dest[0] |= ((c2 & 0xc) >> 2);

            // second char
            dest[1] = SECONDARY_CHAR_BEGIN;
            dest[1] |= ((c2 & 0x3) << 4);
            dest[1] |= c3;

            // end char
            dest[2] = END;

#if defined (UTF8_DECODER_LOG)
            Log(INFO, "%X %X", dest[0], dest[1]);
#endif
            break;
        }

        case BasicMultiLingual:
        {
            char c0 = hexchr_to_hex(hex_str[0]);

            // first char
            dest[0] = BASIC_MULTILINGUAL_BEGIN;
            dest[0] |= c0;

            // second char
            dest[1] = SECONDARY_CHAR_BEGIN;
            dest[1] |= (c1 << 2);
            dest[1] |= ((c2 & 0xc) >> 2);

            // third char
            dest[2] = SECONDARY_CHAR_BEGIN;
            dest[2] |= ((c2 & 0x3) << 4);
            dest[2] |= c3;

            // end char
            dest[3] = END;

#if defined (UTF8_DECODER_LOG)
            Log(INFO, "%X %X %X", dest[0], dest[1], dest[2]);
#endif
            break;
        }

        case OthersPlanesUnicode:
        {
            char c0 = hexchr_to_hex(hex_str[0]);
            char c4 = hexchr_to_hex(hex_str[4]);

            if (cdp <= 0xfffff)
            {
                // first char
                dest[0] = OTHERS_PLANES_UNICODE_BEGIN;
                dest[0] |= ((c0 & 0xc) >> 2);

                // second char
                dest[1] = SECONDARY_CHAR_BEGIN;
                dest[1] |= ((c0 & 0x3) << 4);
                dest[1] |= c1;

                // third char
                dest[2] = SECONDARY_CHAR_BEGIN;
                dest[2] |= (c2 << 2);
                dest[2] |= ((c3 & 0xc) >> 2);

                // fourth char
                dest[3] = SECONDARY_CHAR_BEGIN;
                dest[3] |= ((c3 & 0x3) << 4);
                dest[3] |= c4;

                // end char
                dest[4] = END;

#if defined (UTF8_DECODER_LOG)
                Log(INFO, "%X %X %X %X", dest[0], dest[1], dest[2],  dest[3]);
#endif
            }
            else
            {
                char c5 = hexchr_to_hex(hex_str[5]);

                // first char
                dest[0] = OTHERS_PLANES_UNICODE_BEGIN;
                dest[0] |= ((c0 & 0x1) << 2);
                dest[0] |= ((c1 & 0xc) >> 2);

                // second char
                dest[1] = SECONDARY_CHAR_BEGIN;
                dest[1] |= ((c1 & 0x3) << 4);
                dest[1] |= ((c1 & 0xc) >> 2);
                dest[1] |= c2;

                // third char
                dest[2] = SECONDARY_CHAR_BEGIN;
                dest[2] |= (c3 << 2);
                dest[2] |= ((c4 & 0xc) >> 2);

                // fourth char
                dest[3] = SECONDARY_CHAR_BEGIN;
                dest[3] |= ((c4 & 0x3) << 4);
                dest[3] |= c5;

                // end char
                dest[4] = END;

#if defined (UTF8_DECODER_LOG)
                Log(INFO, "%X %X %X %X", dest[0], dest[1], dest[2],  dest[3]);
#endif
            }

            break;
        }

        case OutRange:
            *dest = END;
#if defined (UTF8_DECODER_LOG)
            Log(WARNING, "String is empty, we are out of utf8 range !");
#endif
            break;
    }


}

static bool utf8valid(const char *str)
{
    const char* s = str;

    if (str == NULL)
    {
#if defined (UTF8_DECODER_LOG)
        Log(WARNING, "Null string");
#endif
        return UTF8_BAD_CHAR;
    }

    while (*s != END)
    {
        if (0xf0 == (0xf8 & *s))
        {
            // ensure each of the 3 following bytes in this 4 byte
            // utf8 codepoint began with 0b10xxxxxx
            if ((0x80 != (0xc0 & s[1])) || (0x80 != (0xc0 & s[2])) || (0x80 != (0xc0 & s[3])))
                return UTF8_BAD_CHAR;

            // ensure that our utf8 codepoint ended after 4 byte
            if (0x80 == (0xc0 & s[4]))
                return UTF8_BAD_CHAR;

            // ensure that the top 5 bits of this 4 byte utf8
            // codepoint were not 0, as then we could have used
            // one of the smaller encodings
            if ((0 == (0x07 & s[0])) && (0 == (0x30 & s[1])))
                return UTF8_BAD_CHAR;

            // 4-byte utf8 code point (began with 0b11110xxx)
            s += 4;
        }
        else if (0xe0 == (0xf0 & *s))
        {
            // ensure each of the 2 following bytes in this 3 byte
            // utf8 codepoint began with 0b10xxxxxx
            if ((0x80 != (0xc0 & s[1])) || (0x80 != (0xc0 & s[2])))
                return UTF8_BAD_CHAR;

            // ensure that our utf8 codepoint ended after 3 bytes
            if (0x80 == (0xc0 & s[3]))
                return UTF8_BAD_CHAR;

            // ensure that the top 5 bits of this 3 byte utf8
            // codepoint were not 0, as then we could have used
            // one of the smaller encodings
            if((0 == (0x0f & s[0])) && (0 == (0x20 & s[1])))
                return UTF8_BAD_CHAR;

            // 3-byte utf8 code point (began with 0b1110xxxx)
            s += 3;
        }
        else if (0xc0 == (0xe0 & *s))
        {
            // ensure the 1 following byte in this 2 byte
            // utf8 codepoint began with 0b10xxxxxx
            if (0x80 != (0xc0 & s[1]))
                return UTF8_BAD_CHAR;

            // ensure that our utf8 codepoint ended after 2 bytes
            if (0x80 == (0xc0 & s[2]))
                return UTF8_BAD_CHAR;

            // ensure that the top 4 bits of this 2 byte utf8
            // codepoint were not 0, as then we could have used
            // one of the smaller encodings
            if (0 == (0x1e & s[0]))
                return UTF8_BAD_CHAR;

            // 2-byte utf8 code point (began with 0b110xxxxx)
            s += 2;
        }
        else if (0x00 == (0x80 & *s))
        {
            // 1-byte ascii (began with 0b0xxxxxxx)
            s += 1;
        }
        else
        {
            // we have an invalid 0b1xxxxxxx utf8 code point entry
            return UTF8_BAD_CHAR;
        }
    }

    return UTF8_GOOD_CHAR;
}

static int32_t utf8codepoint(const char* str)
{
    int32_t codepoint = 0;
    const char* s = str;

    if (utf8valid(str))
    {
        if (str == NULL)
        {
#if defined (UTF8_DECODER_LOG)
            Log(WARNING, "Null string");
#endif
            return -1;
        }

        while (*s != END)
        {
            if (0xf0 == (0xf8 & *s))
            {
                // four byte
                codepoint = ((0x07 & s[0]) << 18) | ((0x3f & s[1]) << 12) | ((0x3f & s[2]) << 6) | (0x3f & s[3]);
                s += 4;
            }
            else if (0xe0 == (0xf0 & *s))
            {
                // three byte
                codepoint = ((0x0f & s[0]) << 12) | ((0x3f & s[1]) << 6) | (0x3f & s[2]);
                s += 3;
            }
            else if (0xc0 == (0xe0 & *s))
            {
                // two byte
                codepoint = ((0x1f & s[0]) << 6) | (0x3f & s[1]);
                s += 2;
            }
            else if (0x00 == (0x80 & *s))
            {
                // one byte
                codepoint = s[0];
                ++ s;
            }
            else
            {
#if defined (UTF8_DECODER_LOG)
                Log(WARNING, "Invalid codepoint");
#endif
                return -1;
            }
        }
    }

    return codepoint;
}

static void utf8chr(const int32_t codepoint, char* dest)
{
    if (codepoint >= 0x0000 && codepoint <= 0x007f)
    {
        dest[0] = codepoint;
        dest[1] = END;
#if defined (UTF8_DECODER_LOG)
        Log(INFO, "%X", dest[0]);
#endif
    }
    else if (codepoint > 0x007f && codepoint <= 0x07ff)
    {
        dest[0] = LATIN_EXTRA_BEGIN;
        dest[1] = SECONDARY_CHAR_BEGIN;

        if (codepoint > 0xff)
            dest[0] |= (codepoint >> 6);
        dest[0] |= ((codepoint & 0xc0) >> 6);
        dest[1] |= (codepoint & 0x3f);

        dest[2] = END;
#if defined (UTF8_DECODER_LOG)
        Log(INFO, "%X %X", dest[0], dest[1]);
#endif
    }
    else if (codepoint > 0x07ff && codepoint <= 0xffff)
    {
        dest[0] = BASIC_MULTILINGUAL_BEGIN;
        dest[1] = SECONDARY_CHAR_BEGIN;
        dest[2] = SECONDARY_CHAR_BEGIN;

        if (codepoint > 0xfff)
            dest[0] |= ((codepoint & 0xf000) >> 12);
        dest[1] |= ((codepoint & 0xf00) >> 6);
        dest[1] |= ((codepoint & 0xf0) >> 6);
        dest[2] |= (codepoint & 0x30);
        dest[2] |= (codepoint & 0xf);

        dest[3] = END;
#if defined (UTF8_DECODER_LOG)
        Log(INFO, "%X %X %X", dest[0], dest[1], dest[2]);
#endif
    }
    else if (codepoint > 0xffff && codepoint <= 0x10ffff)
    {
        dest[0] = OTHERS_PLANES_UNICODE_BEGIN;
        dest[1] = SECONDARY_CHAR_BEGIN;
        dest[2] = SECONDARY_CHAR_BEGIN;
        dest[3] = SECONDARY_CHAR_BEGIN;

        if (codepoint > 0xfffff)
            dest[0] |= ((codepoint & 0x100000) >> 18);
        dest[0] |= ((codepoint & 0xc0000) >> 18);
        dest[1] |= ((codepoint & 0x30000) >> 12);
        dest[1] |= ((codepoint & 0xf000) >> 12);
        dest[2] |= ((codepoint & 0xf00) >> 6);
        dest[2] |= ((codepoint & 0xc0) >> 6);
        dest[3] |= (codepoint & 0x30);
        dest[3] |= (codepoint & 0xf);

        dest[4] = END;
#if defined (UTF8_DECODER_LOG)
        Log(INFO, "%X %X %X %X", dest[0], dest[1], dest[2], dest[3]);
#endif
    }
    else
    {
        *dest = END;
#if defined (UTF8_DECODER_LOG)
        Log(WARNING, "String is empty, we are out of utf8 range !");
#endif
    }
}

#undef LATIN_EXTRA_BEGIN
#undef BASIC_MULTILINGUAL_BEGIN
#undef OTHERS_PLANES_UNICODE_BEGIN
#undef SECONDARY_CHAR_BEGIN
#undef END
#undef UTF8_BAD_CHAR
#undef UTF8_GOOD_CHAR

#endif