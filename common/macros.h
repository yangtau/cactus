#ifndef _LLL_COMMON_MACRO_H_
#define _LLL_COMMON_MACRO_H_

/* IN: return lo<=a<=hi */
#define IN(a, lo, hi)                     \
    ({                                    \
        typeof(a) __a  = (a);             \
        typeof(a) __hi = (typeof(a))(hi); \
        typeof(a) __lo = (typeof(a))(lo); \
        __a >= __lo &&__a <= __hi;        \
    })

#define MAX(a, b)                                         \
    (typeof(a) __a = (a); typeof(a) __b = (typeof(a))(b); \
     __a > __b ? __a : __b;)

#define MIN(a, b)                                         \
    (typeof(a) __a = (a); typeof(a) __b = (typeof(a))(b); \
     __a < __b ? __a : __b;)

#endif // _LLL_COMMON_MACRO_H_
