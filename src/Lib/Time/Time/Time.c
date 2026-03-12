#include <XC.Core/pkg.c>
#include "conversion_macros.h"
#define module std, Time

from(std,
	use(Time)
)

errvt moduleMethod(std_Time, Difference, std_Time* time_cmp, std_Time* result){
	nonull(self, time_cmp, result){ return err; }


	result->seconds = this.seconds < time_cmp->seconds ?
		time_cmp->seconds - this.seconds :
		this.seconds - time_cmp->seconds;
	
	result->nanosec = this.nanosec < time_cmp->nanosec ?
		time_cmp->nanosec - this.nanosec :
		this.nanosec - time_cmp->nanosec;

return OK;
}

bool moduleMethod(std_Time, Compare, std_Time* time_cmp, std_Time* tolerence){
	nonull(self, time_cmp, tolerence){ return false; } 

	std_Time* differ = push(std_Time);

	iferr(std.Time.Difference(self, time_cmp, differ)){
		ERR(ERR.FAIL, "failed to compare times");
		return false;
	}

	if(tolerence == nil)
		return   differ->seconds == 0 &&
			 differ->nanosec == 0;
	else
		return   differ->seconds > tolerence->seconds &&
			 differ->nanosec > tolerence->nanosec;
	
		
return false;
}

PRINT(std_Time){
	if(f.type)
	    return printTo(out, 
		    $F(push(std_Date, self), .type = f.type)
	    );
	else
	    return printTo(out,
		"(std_Time){" 
	      	    ".seconds = ", $(this.seconds), ", ",
	      	    ".nanosec = ", $(this.nanosec), 
	      	" }"
	    );
}

SET(std_Time){
	nonull(self, value){ return err; }
	if(value == nil) {
		this.seconds = 0;
		this.nanosec = 0;
	} else {
		this.seconds = (len_t)*(float*)value;
		this.nanosec = ((len_t)*(float*)value - this.seconds) * NS_PER_S;
	}
return OK;
}

SCAN(std_Time){
	if(f.type)
	    return scanFrom(in, 
		    $F(push(std_Date, self), .type = f.type)
	    );

	ERR(ERR.INVALID, "cannot scan time with unspecified time format");
return 0;
}

construct(std_Time,
FMT(),
DEF(),
	.Print  = std_Time_Op_Print,
	.Create = std_Time_Op_Create,
	.Set    = std_Time_Op_Set,
	.Scan   = std_Time_Op_Scan,

){ 
  this.seconds = arg.seconds;
  this.nanosec = arg.nanosec;
return self;
}

