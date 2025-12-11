#include "../../../pkg.h"

import(std)

#define module std, Nil

#undef nil

len_t nil(){
	ERR(ERR_NULLPTR, "nil method called"); return 0;} 

construct(std_Nil,,,

	.Print 	 = generic nil,
	.Scan 	 = generic nil,
	.Write   = generic nil,
	.Read 	 = generic nil,
	.Copy 	 = generic nil,
	.Create  = generic nil,
	.Destroy = generic nil,
	.Iter 	 = generic nil,
	.Size 	 = generic nil,
	.Hash 	 = generic nil,
	.Set 	 = generic nil
);

const void* __nil_obj = 
	&(std_Nil){.__type = &std_Nil_TypeData};


