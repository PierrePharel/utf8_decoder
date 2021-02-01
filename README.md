# 📚 utf8-decoder

A simple one header solution to decode hex utf8 string in C and C++.

## Usage

```c
#include "utf8-decoder.h"


int main(void)
{
    char utf8_str[5] = {0}; // five is max size of utf8 string with null-terminated char

    utf8decode("1F47A", utf8_str);
    printf("decoded : %s\n", utf8_str);
}
```