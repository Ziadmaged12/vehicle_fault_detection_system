/*
 * STD_types.h
 *
 *  Created on: Sep 17, 2025
 *      Author: ziad
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

typedef unsigned char u8;
typedef unsigned short int  u16;
typedef signed short int  s16;
typedef unsigned long int  u32;
typedef signed long int  s32;
typedef unsigned long long int  u64;
typedef signed long long int  s64;
typedef float f32;
typedef double f64;
typedef long double f128;
typedef enum{ LOW=(0u) , HIGH=(1u) } LOGICAL_VALUE;
typedef enum{ F=(0u) , T=(1u) } bool;
#define NULL_PTR ((void*)0)
#define NULL ((void*)0)

#endif /* STD_TYPES_H_ */
