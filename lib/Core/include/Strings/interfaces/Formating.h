#pragma once

#define __XC_STRINGS__
#include "../pkg.h"
#include "../types.h"

Decl(String)
Decl(StringBuilder)

Interface(Formatter,
	#define $(expr)   NULL, &(asObject(expr)->__methods->Formatter), generic asObject(expr)

	#define endprint NULL, NULL
	#define endscan  NULL, NULL

	u64 imethod(Scan,, FormatID* format, inst(String) in);
	u64 imethod(Print,, FormatID* format, inst(StringBuilder) out);
)

VTable(FormatUtils,
	
	#define FMT ((pntr)1)
	#define F(format_type, format, ...) 				\
		FMT, FORMAT_##format_type, format_type##_##format,	\
		__VA_ARGS__, 						\
		FMT,FORMAT_##format_type, format_type##_##format

	u64 vmethod(FormatVArgs, inst(StringBuilder) out, va_list args);
	u64 vmethod(ScanVArgs, inst(String) in, va_list args);
)
