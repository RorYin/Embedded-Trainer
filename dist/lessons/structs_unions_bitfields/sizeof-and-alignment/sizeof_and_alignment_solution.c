#include <stdio.h>
#include <stdint.h>

typedef struct { char a; int b; char c; } S1;
typedef struct { int b; char a; char c; } S2;
typedef struct { short x; int y; char z; } S3;
typedef struct { char a; char b; short c; int d; } S4;

uint32_t get_s1_size(void)
{
    return (uint32_t)sizeof(S1);
}

uint32_t get_s2_size(void)
{
    return (uint32_t)sizeof(S2);
}

uint32_t get_s3_size(void)
{
    return (uint32_t)sizeof(S3);
}

uint32_t get_s4_size(void)
{
    return (uint32_t)sizeof(S4);
}
