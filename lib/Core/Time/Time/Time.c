#include "../../pkg.h"

import(std)


use(std,
	Time
)


errvt methodimpl(std_Time, Difference, Time* time_cmp, Time* result){
	nonull(self || time_cmp || result, return err);

	result->seconds = this.seconds < time_cmp->seconds ?
		time_cmp->seconds - this.seconds :
		this.seconds - time_cmp->seconds;
	
	result->nanosec = this.nanosec < time_cmp->nanosec ?
		time_cmp->nanosec - this.nanosec :
		this.nanosec - time_cmp->nanosec;

return OK;
}

bool methodimpl(Time, Compare, Time* time_cmp, Time* tolerence){
	
	Time* differ = push(Time);

	iferr(std.Time.Difference(self, time_cmp, differ)){
		ERR(ERR_FAIL, "failed to compare times");
		return false;
	}

	if(tolerence == null || tolerence == nil)
		return   differ->seconds == 0 &&
			 differ->nanosec == 0;
	else
		return   differ->seconds > tolerence->seconds &&
			 differ->nanosec > tolerence->nanosec;
	
		
return false;
}


#define NS_PER_US 1000
#define US_PER_MS 1000
#define MS_PER_S  1000
#define NS_PER_S  1000000000

#define S_PER_M  60
#define S_PER_H  3600
#define S_PER_D  86400

#define DAYS_PER_GREGORIAN_YEAR 365.2425
#define S_PER_Y (DAYS_PER_GREGORIAN_YEAR * S_PER_D)
#define S_PER_MO (S_PER_Y / 12)

static double simple_fmod(double dividend, double divisor) {
    long long quotient_integer = (long long)(dividend / divisor);

    double remainder = dividend - (quotient_integer * divisor);

return remainder;
}

struct TimeConversion vmethodimpl(std_Time, Convert_sec_to_minutes, len_t sec){
return (struct TimeConversion){
	.result = sec /  S_PER_M,
	.remainder = sec % S_PER_M
	};
}
struct TimeConversion vmethodimpl(std_Time, Convert_sec_to_hours,   len_t sec){
return (struct TimeConversion){
	.result = sec /  S_PER_H,
	.remainder = sec % S_PER_H
	};
}
struct TimeConversion vmethodimpl(std_Time, Convert_sec_to_days,    len_t sec){
return (struct TimeConversion){
	.result = sec /  S_PER_D,
	.remainder = sec % S_PER_D
	};
}
struct TimeConversion vmethodimpl(std_Time, Convert_sec_to_months,  len_t sec){
return (struct TimeConversion){
	.result = sec /  S_PER_MO,
	.remainder = simple_fmod(sec, S_PER_MO)
	};
}
struct TimeConversion vmethodimpl(std_Time, Convert_sec_to_years,   len_t sec){
return (struct TimeConversion){
	.result = sec /  S_PER_Y,
	.remainder = simple_fmod(sec, S_PER_MO)
	};
}

len_t vmethodimpl(std_Time, Convert_sec_from_hours,   len_t min)  { return min   * S_PER_M;  }
len_t vmethodimpl(std_Time, Convert_sec_from_days,    len_t days) { return days  * S_PER_M;  }
len_t vmethodimpl(std_Time, Convert_sec_from_months,  len_t mon)  { return mon   * S_PER_MO; }
len_t vmethodimpl(std_Time, Convert_sec_from_years,   len_t years){ return years * S_PER_Y;  }

struct TimeConversion vmethodimpl(std_Time, Convert_nano_to_micro,   len_t nanosec){
return (struct TimeConversion){
	.result = nanosec /  NS_PER_US,
	.remainder = nanosec % NS_PER_US
	};
}
struct TimeConversion vmethodimpl(std_Time, Convert_nano_to_milli,   len_t nanosec){
return (struct TimeConversion){
	.result = nanosec /  (NS_PER_US * US_PER_MS),
	.remainder = nanosec % (NS_PER_US * US_PER_MS)
	};
}
struct TimeConversion vmethodimpl(std_Time, Convert_nano_to_second,  len_t nanosec){
return (struct TimeConversion){
	.result = nanosec /  NS_PER_S,
	.remainder = nanosec % NS_PER_S
	};
}

len_t vmethodimpl(std_Time, Convert_nano_from_micro,   len_t microsec){ return microsec * NS_PER_US; }
len_t vmethodimpl(std_Time, Convert_nano_from_milli,   len_t millisec){ return millisec * (NS_PER_US * US_PER_MS); }
len_t vmethodimpl(std_Time, Convert_nano_from_second,  len_t sec){ return sec * NS_PER_S; }


PRINT(std_Time){
	if(f.type)
	    return printTo(out, 
		    $F(push(std_Date, self), .type = f.type)
	    );
	else
	    return printTo(out,
		"(Time){" 
	      	    ".seconds = ", $(this.seconds), ", ",
	      	    ".nanosec = ", $(this.nanosec), 
	      	" }"
	    );
}

SET(std_Time){
	if(!value) {
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

	ERR(ERR_INVALID, "cannot scan time with unspecified time format");
return 0;
}

construct(std_Time,
FMT(.type = std_Date_Format_Null),
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






