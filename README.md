# 📚 utf8-decoder

A simple one header solution to decode hex utf8 string in C and C++.

## Usage

```c
#include <stdio.h>
#include "utf8-decoder.h"

int main(void)
{
    char *utf8_str = NULL; // five is max size of utf8 string with null-terminated char

    utf8_str = utf8decode("1F47A");
    printf("decoded : %s\n", utf8_str);  // decoded : 👺

    free(utf8_str); // freeing of **utf8_str** because utfe8decode use dynamic memory allocation
}
```

NB : the *utf8valid* function is based on the function of the same name in the [utf8.h](https://github.com/sheredom/utf8.h) repository.