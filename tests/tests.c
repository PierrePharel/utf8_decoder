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

#include <stdio.h>
#include <string.h>
#define UTF8_DECODER_LOG
#include <utf8_decoder.h>
#include "test.h"


void hex_format (char* str)
{
    char tmp[5];
    int sz = strlen (str);
    int filler_sz = 4 - sz;
    int i = 0;

    strcpy (tmp, str);

    while (i < filler_sz)
    {
        str[i] = '0';
        i ++;
    }

    for (; i < (sz + filler_sz); ++ i)
        str[i] = tmp[i - filler_sz];

    str[5] = '\0';
}

int main (int argc, char** argv)
{
    char out[5];
    char in[5];

    printf ("#===============================#\n");
    printf ("#             Tests             #\n");
    printf ("#===============================#\n");

    for (int i = 0; i < 0x04ff; i ++)
    {
        sprintf (in, "%X", i);
        hex_format(in);
        printf ("in : %s\n", in);
        utf8decode (in, out);
        printf ("in : %s => out : %s\n", in, out);
    }

    printf ("#===============================#\n");

    return 0;
}