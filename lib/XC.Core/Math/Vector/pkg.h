#pragma once

#define __XC_MATH__
#include "../pkg.h"
#include "../types.h"

#define package std

Class(Vector,
INIT(std_Math_VecData initData),
FMT(),
){
	bool 		method(Vector, isFloat);
	u64 		method(Vector, DataSize);
	std_Vector* 	method(Vector, DotProduct, 	std_Vector* other);
	errvt 		method(Vector, DotProductInto,  std_Vector* a, std_Vector* b);
};

#define vec(first, ...) (std_Vector*) 				\
	push(std_Vector, (std_VecData){true, 			\
	_Generic((first),					\
	std_Vector*: std.Vector.isFloat((std_Vector*)		\
		((i64*)(&(typeof(first)){first}))),		\
	std_Number*: std.Number.isFloat((std_Number*)		\
		((i64*)(&(typeof(first)){first}))),		\
	float: true,						\
	double: true,						\
	default: false						\
	),							\
	_Generic((first),					\
	std_Vector*: VECDATA_VEC,				\
	std_Number*: VECDATA_BIG,				\
	double: VECDATA_LONG,					\
	i64: 	VECDATA_LONG,					\
	float:  VECDATA_REG,					\
	i8:     VECDATA_REG,					\
	i16: 	VECDATA_REG,					\
	i32:  	VECDATA_REG					\
	),							\
	_Generic((first),					\
	std_Vector*: std.Vector.DataSize((std_Vector*)		\
		((i64*)(&(typeof(first)){first}))),		\
	default: sizeof(typeof(first))				\
	),							\
	sizeof((typeof(first)[]){first, __VA_ARGS__})		\
		/sizeof(typeof(first)),				\
	(typeof(first)[]){first, __VA_ARGS__}			\
})


#define VEC(first, ...) (std_Vector*) 				\
	new(std_Vector, (std_VecData){true, 			\
	_Generic((first),					\
	std_Vector*: std.Vector.isFloat((std_Vector*)		\
		((i64*)(&(typeof(first)){first}))),		\
	std_Number*: std.Number.isFloat((std_Number*)		\
		((i64*)(&(typeof(first)){first}))),		\
	float: true,						\
	double: true,						\
	default: false						\
	),							\
	_Generic((first),					\
	std_Vector*: VECDATA_VEC,				\
	std_Number*: VECDATA_BIG,				\
	double: VECDATA_LONG,					\
	i64: 	VECDATA_LONG,					\
	float:  VECDATA_REG,					\
	i8:     VECDATA_REG,					\
	i16: 	VECDATA_REG,					\
	i32:  	VECDATA_REG					\
	),							\
	_Generic((first),					\
	std_Vector*: std.Vector.DataSize((std_Vector*)		\
		((i64*)(&(typeof(first)){first}))),		\
	default: sizeof(typeof(first))				\
	),							\
	sizeof((typeof(first)[]){first, __VA_ARGS__})		\
		/sizeof(typeof(first)),				\
	(typeof(first)[]){first, __VA_ARGS__}			\
})

#define Vector(type, dim) std_Vector*

#undef package
