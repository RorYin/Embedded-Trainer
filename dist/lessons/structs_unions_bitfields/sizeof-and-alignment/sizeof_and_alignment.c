#include <stdio.h>
#include <stdint.h>

/*
 * TASK 1: Struct Padding & Memory Alignment Calculations
 *
 * Compilers pad struct members to align them to natural memory boundaries.
 */

typedef struct { char a; int b; char c; } S1;
typedef struct { int b; char a; char c; } S2;
typedef struct { short x; int y; char z; } S3;
typedef struct { char a; char b; short c; int d; } S4;

/* Return the sizeof(S1) */
uint32_t get_s1_size(void)
{
    /* TODO: Return sizeof(S1) */
    return 0;
}

/* Return the sizeof(S2) */
uint32_t get_s2_size(void)
{
    /* TODO: Return sizeof(S2) */
    return 0;
}

/* Return the sizeof(S3) */
uint32_t get_s3_size(void)
{
    /* TODO: Return sizeof(S3) */
    return 0;
}

/* Return the sizeof(S4) */
uint32_t get_s4_size(void)
{
    /* TODO: Return sizeof(S4) */
    return 0;
}
