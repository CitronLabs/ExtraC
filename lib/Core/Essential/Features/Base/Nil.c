#include "../../../pkg.h"

import(std)

#define module std, Nil

#undef nilmethod

len_t nilmethod(){
	ERR(ERR_NULLPTR, "nil method called"); return 0;} 

construct(std_Nil,,,

	.Print 	 = generic nilmethod,
	.Scan 	 = generic nilmethod,
	.Write   = generic nilmethod,
	.Read 	 = generic nilmethod,
	.Copy 	 = generic nilmethod,
	.Create  = generic nilmethod,
	.Destroy = generic nilmethod,
	.Iter 	 = generic nilmethod,
	.Size 	 = generic nilmethod,
	.Hash 	 = generic nilmethod,
	.Set 	 = generic nilmethod
);

const void* __nil_obj = 
	&(std_Nil){.__type = &std_Nil_TypeData};


