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
#define UTF8_DECODER_LOG
#include <utf8_decoder.h>
#include "test.h"


int main()
{
    char dst1[5];
    char dst2[5];
    int codepoint;

    // one byte
    utf8decode("0024", dst1);
    printf("decoded result : %s\n", dst1);
    codepoint = utf8codepoint(dst1);
    printf("decoded codepoint : %d\n", codepoint);
    utf8chr(codepoint, dst2);
    printf("chr codepoint : %s\n", dst2);

    // two byte
    utf8decode("00A2", dst1);
    printf("decoded result : %s\n", dst1);
    codepoint = utf8codepoint(dst1);
    printf("decoded codepoint : %d\n", codepoint);
    utf8chr(codepoint, dst2);
    printf("chr codepoint : %s\n", dst2);

    // three byte
    utf8decode("D55C", dst1);
    printf("decoded result : %s\n", dst1);
    codepoint = utf8codepoint(dst1);
    printf("decoded codepoint : %d\n", codepoint);
    utf8chr(codepoint, dst2);
    printf("chr codepoint : %s\n", dst2);

    // four byte
    utf8decode("1F47A", dst1);
    printf("decoded result : %s\n", dst1);
    codepoint = utf8codepoint(dst1);
    printf("decoded codepoint : %d\n", codepoint);
    utf8chr(codepoint, dst2);
    printf("chr codepoint : %s\n", dst2);

    return 0;
}