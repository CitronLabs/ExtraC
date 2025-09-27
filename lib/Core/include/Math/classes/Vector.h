#pragma once

#define __XC_MATH__
#include "../pkg.h"
#include "../types.h"


Class(Vector,
INIT(VecData initData),
FIELD(),

	bool 		method(Vector, isFloat);
	u64 		method(Vector, DataSize);
	inst(Vector) 	method(Vector, DotProduct,, 	 inst(Vector) other);
	errvt 		method(Vector, DotProductInto,,  inst(Vector) a, inst(Vector) b);
)

#define vec(first, ...) push(Vector, (VecData){true, 		\
	_Generic((first),					\
	inst(Vector): Vector.isFloat((inst(Vector))		\
		((i64*)(&(typeof(first)){first}))),		\
	inst(Number): Number.isFloat((inst(Number))		\
		((i64*)(&(typeof(first)){first}))),		\
	float: true,						\
	double: true,						\
	default: false						\
	),							\
	_Generic((first),					\
	inst(Vector): VECDATA_VEC,				\
	inst(Number): VECDATA_BIG,				\
	double: VECDATA_LONG,					\
	i64: 	VECDATA_LONG,					\
	float:  VECDATA_REG,					\
	i8:     VECDATA_REG,					\
	i16: 	VECDATA_REG,					\
	i32:  	VECDATA_REG					\
	),							\
	_Generic((first),					\
	inst(Vector): Vector.DataSize((inst(Vector))		\
		((i64*)(&(typeof(first)){first}))),		\
	default: sizeof(typeof(first))				\
	),							\
	sizeof((typeof(first)[]){first, __VA_ARGS__})		\
		/sizeof(typeof(first)),				\
	(typeof(first)[]){first, __VA_ARGS__}			\
})


#define VEC(first, ...) new(Vector, (VecData){true, 		\
	_Generic((first),					\
	inst(Vector): Vector.isFloat((inst(Vector))		\
		((i64*)(&(typeof(first)){first}))),		\
	inst(Number): Number.isFloat((inst(Number))		\
		((i64*)(&(typeof(first)){first}))),		\
	float: true,						\
	double: true,						\
	default: false						\
	),							\
	_Generic((first),					\
	inst(Vector): VECDATA_VEC,				\
	inst(Number): VECDATA_BIG,				\
	double: VECDATA_LONG,					\
	i64: 	VECDATA_LONG,					\
	float:  VECDATA_REG,					\
	i8:     VECDATA_REG,					\
	i16: 	VECDATA_REG,					\
	i32:  	VECDATA_REG					\
	),							\
	_Generic((first),					\
	inst(Vector): Vector.DataSize((inst(Vector))		\
		((i64*)(&(typeof(first)){first}))),		\
	default: sizeof(typeof(first))				\
	),							\
	sizeof((typeof(first)[]){first, __VA_ARGS__})		\
		/sizeof(typeof(first)),				\
	(typeof(first)[]){first, __VA_ARGS__}			\
})

#define Vector(type, dim) inst(Vector)
