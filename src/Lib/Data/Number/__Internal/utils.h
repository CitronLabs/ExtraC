
#define BIGINT_BASE (1ULL << 32)
#define isZero(num) (num->__private.sign == 0)

#define makeTempNum(list, _precision) (&(std_Number){		\
	.__type = generic &std_Number_Type,			\
	.__private = { 						\
	.sign = 1, .floating = false, .precision = _precision,	\
	.exponent = -1, .digits = list }			\
})
#define opriv (other->__private)
#define rpriv (result->__private)

#define apriv (a->__private)
#define bpriv (b->__private)
