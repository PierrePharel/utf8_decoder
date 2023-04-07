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

#include <stdbool.h>

#if defined(_MSC_VER) && (_MSC_VER < 1920)
    typedef __int32 utf8_int32_t;
    #pragma warning (disable: 4309)
#else
    #include <stdint.h>
    typedef int32_t utf8_int32_t;
#endif

typedef uint8_t utf8chr_t;

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
extern utf8chr_t hexchr_to_hex(const utf8chr_t hex_chr);

// Return a the Utf8Type defined above
// But you can get codepoint (decimal value) of hexadecimal string if you pass an int32_t pointer instead of NULL
// Convert hexadecimal string in a decimal value, and determinate what is the range of utf8 string
extern Utf8Type utf8type(const utf8chr_t* hex_str, int32_t* cdp);

// Convert hexadecimal utf8 string into usable utf8 string and store it in dest
// dest will be empty if hexadecimal string is invalid or out of range
// NB: min size of dest must be 2 and max 5 with null char
extern void utf8decode(const utf8chr_t* hex_str, utf8chr_t* dest);

// Return a bool (true, if valid and false otherwise)
// Check if str is a valid utf8 string
extern bool utf8valid(const utf8chr_t* str);

// Return codepoint greater than or equal to zero and less than zero if char is out of range or if string is NULL
// Convert utf8 string in codepoint (decimal value)
extern int32_t utf8codepoint(const char* str);

// Convert codepoint into usable utf8 string and store it in dest
// dest will be empty if hexadecimal string is invalid or out of range
// NB: min size of dest must be 2 and max 5 with null char
extern void utf8chr(const int32_t codepoint, utf8chr_t* dest);

#endif