#include <XC.Core/pkg.c>
#include "conversion_macros.h"
#define module std, Time, Convert

from(std,
    use(Time)
)


static double simple_fmod(double dividend, double divisor) {
    long long quotient_integer = (long long)(dividend / divisor);

    double remainder = dividend - (quotient_integer * divisor);

return remainder;
}


struct TimeConversion moduleFn(sec_to_minutes)(len_t sec){
return (struct TimeConversion){
	.result = sec /  S_PER_M,
	.remainder = sec % S_PER_M
	};
}
struct TimeConversion moduleFn(sec_to_hours)(len_t sec){
return (struct TimeConversion){
	.result = sec /  S_PER_H,
	.remainder = sec % S_PER_H
	};
}
struct TimeConversion moduleFn(sec_to_days)(len_t sec){
return (struct TimeConversion){
	.result = sec /  S_PER_D,
	.remainder = sec % S_PER_D
	};
}
struct TimeConversion moduleFn(sec_to_months)(len_t sec){
return (struct TimeConversion){
	.result = sec /  S_PER_MO,
	.remainder = simple_fmod(sec, S_PER_MO)
	};
}
struct TimeConversion moduleFn(sec_to_years)(len_t sec){
return (struct TimeConversion){
	.result = sec /  S_PER_Y,
	.remainder = simple_fmod(sec, S_PER_MO)
	};
}

len_t moduleFn(sec_from_hours)(len_t min)  { return min   * S_PER_M;  }
len_t moduleFn(sec_from_days)(len_t days) { return days  * S_PER_M;  }
len_t moduleFn(sec_from_months)(len_t mon)  { return mon   * S_PER_MO; }
len_t moduleFn(sec_from_years)(len_t years){ return years * S_PER_Y;  }

struct TimeConversion moduleFn(nano_to_micro)(len_t nanosec){
return (struct TimeConversion){
	.result = nanosec /  NS_PER_US,
	.remainder = nanosec % NS_PER_US
	};
}
struct TimeConversion moduleFn(nano_to_milli)(len_t nanosec){
return (struct TimeConversion){
	.result = nanosec /  (NS_PER_US * US_PER_MS),
	.remainder = nanosec % (NS_PER_US * US_PER_MS)
	};
}
struct TimeConversion moduleFn(nano_to_second)(len_t nanosec){
return (struct TimeConversion){
	.result = nanosec /  NS_PER_S,
	.remainder = nanosec % NS_PER_S
	};
}

len_t moduleFn(nano_from_micro)(len_t microsec){ return microsec * NS_PER_US; }
len_t moduleFn(nano_from_milli)(len_t millisec){ return millisec * (NS_PER_US * US_PER_MS); }
len_t moduleFn(nano_from_second)(len_t sec){ return sec * NS_PER_S; }

