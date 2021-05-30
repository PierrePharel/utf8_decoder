# 📚 utf8_decoder

A simple one header solution to decode hex utf8 string in C and C++.

## Usage

Just `#include "utf8_decoder.h"` in your code!

The current supported platforms are Linux, macOS and Windows.

The current supported compilers are gcc, clang, MSVC.

## API function docs

```c
char hexchr_to_hex(const char hex_chr);
```

Return a char decimal value greater than or equal to zero and less than zero if char is out of range

Convert hexadecimal char beetwen '0' - 'F' ('a'-'z' & 'A'-'Z') in decimal value

```c
Utf8Type utf8type(const char* hex_str, int32_t* cdp);
```

Return the Utf8Type

But you can get codepoint (decimal value) of hexadecimal string if you pass an int32_t pointer instead of NULL

Convert hexadecimal string in a decimal value, and determinate what is the range of utf8 string

```c
void utf8decode(const char* hex_str, char* dest);
```

Convert hexadecimal utf8 string into usable utf8 string and store it in dest

dest will be empty if hexadecimal string is invalid or out of range

NB: min size of dest must be 2 and max 5 with null char

```c
bool utf8valid(const char* str);
```

Return a bool (true, if valid and false otherwise)

Check if str is a valid utf8 string

```c
int32_t utf8codepoint(const char* str);
```
Return codepoint greater than or equal to zero and less than zero if char is out of range or if string is NULL

Convert utf8 string in codepoint (decimal value)

```c
void utf8chr(const int32_t codepoint, char* dest);
```
Convert codepoint into usable utf8 string and store it in dest

dest will be empty if hexadecimal string is invalid or out of range

NB: min size of dest must be 2 and max 5 with null char

## Example

```c
#include <stdio.h>
#include "utf8_decoder.h"

int main()
{
    char dst1[5];
    char dst2[5];
    int codepoint;

    // one byte
    utf8decode("0024", dst1);
    printf("decoded result : %s\n", dst1); // decoded : $
    codepoint = utf8codepoint(dst1);
    printf("decoded codepoint : %d\n", codepoint); // codepoint : 36
    utf8chr(codepoint, dst2);
    printf("chr codepoint : %s\n", dst2); // decoded : $

    // two byte
    utf8decode("00A2", dst1);
    printf("decoded result : %s\n", dst1); // decoded : ¢
    codepoint = utf8codepoint(dst1);
    printf("decoded codepoint : %d\n", codepoint); // codepoint : 162
    utf8chr(codepoint, dst2);
    printf("chr codepoint : %s\n", dst2); // decoded : ¢

    // three byte
    utf8decode("D55C", dst1);
    printf("decoded result : %s\n", dst1); // decoded : 한
    codepoint = utf8codepoint(dst1);
    printf("decoded codepoint : %d\n", codepoint); // codepoint : 54620
    utf8chr(codepoint, dst2);
    printf("chr codepoint : %s\n", dst2); // decoded : 한

    // four byte
    utf8decode("1F47A", dst1);
    printf("decoded result : %s\n", dst1); // decoded : 👺
    codepoint = utf8codepoint(dst1);
    printf("decoded codepoint : %d\n", codepoint); // codepoint : 128122
    utf8chr(codepoint, dst2);
    printf("chr codepoint : %s\n", dst2); // decoded : 👺

    return 0;

}
```

NB : the *utf8valid* function is based on the function of the same name in the [utf8.h](https://github.com/sheredom/utf8.h) repository.