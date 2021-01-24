#ifndef UTF8_DECODER
#define UTF8_DECODER

#define _0_32b 0b0000'0000'0000'0000'0000'0000'0000'0000
#define _1_32b 0b0000'0000'0000'0000'0000'0000'0000'0001
#define _2_32b 0b0000'0000'0000'0000'0000'0000'0000'0010
#define _3_32b 0b0000'0000'0000'0000'0000'0000'0000'0011
#define _4_32b 0b0000'0000'0000'0000'0000'0000'0000'0100
#define _5_32b 0b0000'0000'0000'0000'0000'0000'0000'0101
#define _6_32b 0b0000'0000'0000'0000'0000'0000'0000'0110
#define _7_32b 0b0000'0000'0000'0000'0000'0000'0000'0111
#define _8_32b 0b0000'0000'0000'0000'0000'0000'0000'1000
#define _9_32b 0b0000'0000'0000'0000'0000'0000'0000'1001
#define _A_32b 0b0000'0000'0000'0000'0000'0000'0000'1010
#define _B_32b 0b0000'0000'0000'0000'0000'0000'0000'1011
#define _C_32b 0b0000'0000'0000'0000'0000'0000'0000'1100
#define _D_32b 0b0000'0000'0000'0000'0000'0000'0000'1101
#define _E_32b 0b0000'0000'0000'0000'0000'0000'0000'1110
#define _F_32b 0b0000'0000'0000'0000'0000'0000'0000'1111

#define US_ASCII 0x0
#define LATIN 0xc0
#define BASIC_MULTI_LANG 0xe0
#define OTHERS 0xf0

int32_t utf8type(const void *str);
void utf8construct(const char *src, char *dst);

int32_t utf8type(const void *str)
{
    int32_t type = 0;
    short shift = 0;
    const char *s = (const char*)str;

    while(*s != '\0')
    {
        switch(*s)
        {
            case '0': type = ((type << shift) | _0_32b);
                    break;
            case '1': type = ((type << shift) | _1_32b);
                    break;
            case '2': type = ((type << shift) | _2_32b);
                    break;
            case '3': type = ((type << shift) | _3_32b);
                    break;
            case '4': type = ((type << shift) | _4_32b);
                    break;
            case '5': type = ((type << shift) | _5_32b);
                    break;
            case '6': type = ((type << shift) | _6_32b);
                    break;
            case '7': type = ((type << shift) | _7_32b);
                    break;
            case '8': type = ((type << shift) | _8_32b);
                    break;
            case '9': type = ((type << shift) | _9_32b);
                    break;
            case 'A': type = ((type << shift) | _A_32b);
                    break;
            case 'B': type = ((type << shift) | _B_32b);
                    break;
            case 'C': type = ((type << shift) | _C_32b);
                    break;
            case 'D': type = ((type << shift) | _D_32b);
                    break;
            case 'E': type = ((type << shift) | _E_32b);
                    break;
            case 'F': type = ((type << shift) | _F_32b);
                    break;
        }

        shift = 4;
        ++ s;
    }

    return type;
}

void utf8construct(const char *src, char *dst)
{
    const int32_t type = utf8type(src);

    if(type <= 0x007f) // US ASCII
    {
        printf("US ASCII\n");
        dst[0] = type;
        dst[1] = '\0';
    }
    else if(type > 0x007f && type <= 0x07ff) // Latin
    {
        printf("LATIN\n");
    }
    else if(type > 0x07ff && type <= 0xffff) // Basic multi languages (Chinese, Japanese,...)
    {
        printf("BML\n");
    }
    else if(type > 0xffff && type <= 0x10ffff) // Others
    {
        printf("OTHERS\n");
    }
    else // Unknown
    {
        printf("Unknown\n");
        dst = NULL;
    }
}

#endif