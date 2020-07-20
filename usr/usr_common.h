#ifndef __USR_COMMON_H
#define __USR_COMMON_H

#define __I		volatile
#define __O		volatile
#define __IO	volatile

typedef signed char			int8_t;
typedef signed short		int16_t;
typedef signed int			int32_t;
typedef signed long long	int64_t;

typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long long	uint64_t;


typedef signed char			s8;
typedef signed short		s16;
typedef signed int			s32;
typedef signed long long	s64;

typedef unsigned char		u8;
typedef unsigned short		u16;
typedef unsigned int		u32;
typedef unsigned long long	u64;

typedef enum {disable = 0, enable = !disable} FunctionalState;

typedef enum
{
    div1 = 0,
    div2 = 1,
    div3 = 2,
    div4 = 3,
    div5 = 4,
    div6 = 5,
    div7 = 6,
    div8 = 7,
} emGEN_DIV;


#endif



