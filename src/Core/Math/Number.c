#include <Core/pkg.c>


from(std,
	use(Number),
    	use(Stream),
    	use(List)
);

#define HIDE_USE
#include "BigInteger/operations.c"
#include "BigInteger/format.c"
#include "BigInteger/parse.c"
#include "BigFloat/operations.c"
#include "BigFloat/format.c"
#include "BigFloat/parse.c"

#define module std, Number


#define apriv (a->__private)
#define bpriv (b->__private)


// ensuring both are either int or float
#define ensureNumberCompatibility() 					\
	if(apriv.floating != bpriv.floating){  				\
	  if(!apriv.floating){						\
	    tempList = copy(apriv.digits, push_alloc(List));		\
	    a = makeTempNum(tempList, apriv.precision);			\
	    std.Number.Cast.toFloat(a);					\
	  } else {							\
	    tempList = copy(bpriv.digits, push_alloc(List));		\
	    b = makeTempNum(tempList, bpriv.precision);			\
	    std.Number.Cast.toFloat(b);					\
	  }								\
	}


Number* moduleMethod(std_Number, Add, Number* other){
	nonull(self, other){ return nil; }
	
	Number* result = new(Number);

	if(result != nil)
	    iferr(std.Number.AddInto(result, self, other)){
		del(result); return nil;
	    }

return result;
}
errvt moduleMethod(std_Number, AddInto, Number* a, Number* b){
	nonull(a, b){ return err; }

	std.Number.setZero(self);

	// Handle cases where one of the operands is zero
	if (isZero(a) || isZero(b)) {
		List* nonZero_operand = isZero(a) ? bpriv.digits : apriv.digits;

		std.List.Pop(priv.digits, 1);
		std.List.Insert(priv.digits, 0, std.List.Size(nonZero_operand), std.List.GetPointer(nonZero_operand, 0));
		
		if(apriv.floating || bpriv.floating)
			priv.exponent = apriv.floating ? apriv.exponent : bpriv.exponent;
		
		Internal.clearLeadingZeros(self);
		return OK;
	}

	List* tempList = nil;

	ensureNumberCompatibility()


	if(apriv.floating)
		std.Number.BigFloat.Add(a, b, self);
	else
		std.Number.BigInt.Add(a, b, self);

	if(tempList) 
	    { del(tempList); }

	check(){ return err->errorcode;	}

return OK;
}

Number* moduleMethod(std_Number, Subtract, Number* other){
	nonull(self, other){ return nil; }
	
	Number* result = new(Number);

	if(result != nil)
	    iferr(std.Number.SubtractInto(result, self, other)){
		del(result); return nil;
	    }

return result;

return result;
}
errvt moduleMethod(std_Number, SubtractInto, Number* a, Number* b){
	nonull(a, b){ return err; }
	
	std.Number.setZero(self);

	// Handle cases where one of the operands is zero
	if (isZero(b) || isZero(a)) {
		List* zero_operand = apriv.digits;
		if(isZero(a)){
			zero_operand = bpriv.digits;
			priv.sign = bpriv.sign * -1;
		}
		
		std.List.Pop(priv.digits, 1);
		std.List.Insert(priv.digits, 0, elements(zero_operand), std.List.GetPointer(zero_operand, 0));
		
		Internal.clearLeadingZeros(self);
		return OK;
	}

	List* tempList = nil;
	
	ensureNumberCompatibility()

	    if(apriv.floating)
	    	std.Number.BigFloat.Subtract(a, b, self);
	    else
	    	std.Number.BigInt.Subtract(a, b, self);

	    if(tempList) 
		{ del(tempList); }

	    check(){
		return err->errorcode;
	    }

return OK;

}

Number* moduleMethod(std_Number, Multiply, Number* other){
	nonull(self, other){ return nil; }
	
	Number* result = new(Number);

	if(result != nil)
	    iferr(std.Number.MultiplyInto(result, self, other)){
		del(result); return nil;
	    }

return result;
}
errvt moduleMethod(std_Number, MultiplyInto, Number* a, Number* b){
	nonull(a, b){ return err; }
	
	std.Number.setZero(self);
	
	if (isZero(a) || isZero(b)) {
		return OK;
	}
	
	// The maximum size of the product can be the sum of sizes of operands.
	// Ensure result array has enough space.
	if (std.List.Size(apriv.digits) + std.List.Size(bpriv.digits) > apriv.precision) {
		return ERR(ERR.DATA.OUTOFRANGE, "number overflows");
	}
	std.List.Reserve(priv.digits, RESERVE_EXACT,
		elements(priv.digits) + elements(bpriv.digits));


	List* tempList = nil;

	ensureNumberCompatibility()

	    if(apriv.floating)
	    	std.Number.BigFloat.Multiply(a, b, self);
	    else
	    	std.Number.BigInt.Multiply(a, b, self);

	    if(tempList) 
		{ del(tempList); }

    	    check(){
    	    	return err->errorcode;
    	    }

return OK;



}

Number* moduleMethod(std_Number, Divide,   Number* other, Number* remainder){
	nonull(self, other, remainder){ return nil; }
	
	Number* result = new(Number);

	if(result != nil)
	    iferr(std.Number.DivideInto(result, self, other, remainder)){
		del(result); return nil;
	    }

return result;

return result;
}
errvt moduleMethod(std_Number, DivideInto,   Number* a, Number* b, Number* remainder){
	nonull(a, b, remainder){ return err; }

	if (isZero(b)) {
		return ERR(ERR.INVALID, "cannot divide by 0");
	}else if (isZero(a)) {
		return OK;
	}
	
	std.Number.setZero(remainder); // Remainder is zero
	std.Number.setZero(self);
	
	switch(std.Number.Compare(a, b)) {
	// If abs(dividend) < abs(divisor), result is 0, remainder is dividend.
	case std_Number_Equality_LESSER:
		std.List.Append(
			rmpriv.digits, 
			std.List.GetPointer(apriv.digits, 0), 
			std.List.Size(apriv.digits)
		);
	break;
	// If abs(dividend) == abs(divisor), result is 1 (with appropriate sign), remainder is 0.
	case std_Number_Equality_EQUALS:
		*(u32*)index(priv.digits, 0) = 1;
		priv.sign = (apriv.sign == bpriv.sign) ? 1 : -1;
	break;
	default:{
		List* tempList = nil;

		ensureNumberCompatibility()

		    if(apriv.floating)
		    	std.Number.BigFloat.Divide(a, b, remainder, self);
		    else
		    	std.Number.BigInt.Divide(a, b, remainder, self);

		    if(tempList) 
			{ del(tempList); }

		    check(){
		    	return err->errorcode;
		    }
	}
	}
	
return OK;


}
std_Equality moduleMethod(std_Number, Compare, Number* other) {
	nonull(other, self){ return std.Number.Equality.INVALID; }
	
	// Handle zero cases first
	if (isZero(self) && isZero(other)) 
		return  std.Number.Equality.EQUALS; 			 // Both are zero
	if (isZero(self))  
		return (opriv.sign == 1) ?
			std.Number.Equality.LESSER : std.Number.Equality.GREATER; // 0 < positive, 0 > negative
	if (isZero(other)) 
		return (priv.sign == 1) ? 
			std.Number.Equality.GREATER : std.Number.Equality.LESSER;  // positive > 0, negative < 0
	
	
	// Different signs: positive is always greater than negative
	if (priv.sign == 1 && opriv.sign == -1) return std.Number.Equality.GREATER;
	if (priv.sign == -1 && opriv.sign == 1) return -std.Number.Equality.LESSER;
	
	// Same signs: compare absolute values
	std_Equality cmp_abs = Internal.absoluteCompare(self, other);
	if (priv.sign == 1) {
	    return cmp_abs; // Both positive: direct comparison of absolute values
	} else {
	    // Both negative: inverse comparison of absolute values e.g., -5 > -10, but abs(-5) < abs(-10)
	    return -cmp_abs;
	}
}

bool   moduleMethod(std_Number, isFloat){
	nonull(self){ return -1; } 

return priv.floating;
}
errvt  moduleMethod(std_Number, zeroOut){
	nonull(self){ return err; } 

	std.Number.setZero(self); 

return OK;
}

PRINT(std_Number){
	if(priv.floating)
		return Internal.FloatPrint(self, out);
	else
	    switch(format->base){
	    case 1:
		return Internal.IntPrintBin(self, out); break;
	    case 6:
		return Internal.IntPrintHex(self, out); break;
	    case 10:
	    default:
		return Internal.IntPrintDeci(self, out); break;
	    }
}
SCAN(std_Number){
	return format->floating ? 
		Internal.FloatScan(self, format, in) : 
		Internal.IntScan(self, format, in)
	;
}

COPY(std_Number){

	std_Number* dest = where;

	memcpy(where, self, sizeof(std_Number));

	copy(priv.digits, dest->__private.digits);

return where;
}

SET(std_Number){
	nonull(self, value){ return err; }
	
	Stream* temp_stream = push(Stream, 
			std.Stream.Preset.staticBuffer(
			    value, 
			    strsize((strc8)value, 10280)
			)
		)
	;

	ops(Number_Type).Scan(self, generic &Number_Type->format, temp_stream);
	
	pop(temp_stream);
return OK;
}

DESTROY(std_Number){
	del(priv.digits);
return OK;
}

SIZE(std_Number){

return size(priv.digits) + sizeof(std_Number);
}


construct(std_Number,
FMT(),
DEF(),
	
){
	priv.digits = newList(u32, 10);

	std.List.Append(priv.digits, &(u32){0}, 1);

	if(arg.initVal)
		set(self, arg.initVal);

return self; 
}
