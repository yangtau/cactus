#ifndef _CACTUS_LIBS_MACROS_H_
#define _CACTUS_LIBS_MACROS_H_

/* in_range: return lo<=a<=hi */
#define in_range(a, lo, hi)               \
    ({                                    \
        typeof(a) __a  = (a);             \
        typeof(a) __hi = (typeof(a))(hi); \
        typeof(a) __lo = (typeof(a))(lo); \
        __a >= __lo &&__a <= __hi;        \
    })


#endif // _CACTUS_LIBS_MACROS_H_
