#include "../../pkg.h"
#include "./Formats.c"

import(std)
import(XC)

bool methodimpl(std_Date, isValid){
	return 
	    this.month   < 12 &&
	    this.day     < 32 &&
	    this.hour    < 24 &&
	    this.minute  < 60 &&
	    this.second  < 60 
	;

}

std_Time* methodimpl(std_Date, toTime){

	len_t seconds = 
		convert->sec.from.years    (this.year) .result  +
		convert->sec.from.months   (this.month).result +
		convert->sec.from.days     (this.day)  .result   +
		convert->sec.from.hours    (this.hour) .result  +
		convert->sec.from.minutes  (this.hour) .result  +
		this.second
	;

return new(std_Time, .seconds = seconds, .nanosec = this.nanosec);
}
const char* methodimpl(std_Date, getDayName){
	XC_Locale_Data* locale = 
		XC.Dev.Register.access( 
		     XC.Dev.Register.stdHandle
			(XC.Dev.Register.ID.Locale)
		 );

 	static int monthCode[] = {6, 2, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

    	int yearCode = (this.year % 100) + (this.year % 100) / 4;

    	// Adjust year code for the century
    	yearCode = (yearCode + (this.year / 100) / 4 + 5 * (this.year / 100)) % 7;

    	// Calculate the day of the week and return the value as an integer
return locale->Time.day_names[(this.day + monthCode[this.month - 1] + yearCode) % 7];
}


const char* methodimpl(std_Date, getMonthName){
	XC_Locale_Data* locale = 
		XC.Dev.Register.access( 
		     XC.Dev.Register.stdHandle
			(XC.Dev.Register.ID.Locale)
		 );

	if(this.month > 12) { 
		ERR(ERR_INVALID, "invalid date input");
		return null;
	}

return locale->Time.month_names[this.month];
}

SET(std_Date){
	if(create(std_Date, self, value) == nil){
	    return ERR(ERR_FAIL, "failed to set date using time");
	}
return OK;
}

COPY(std_Date){

	if(!memcpy(where, self, sizeof(std_Date))){
		ERR(ERR_FAIL, "failed to copy date");
		return null;
	}

return where;
}

SIZE(std_Date){ return sizeof(std_Date); }

DESTROY(std_Date){ return OK; }


construct(std_Date,
FMT(.type = std_Date_Format_RFC),
DEF(.time = &nilobj(std_Time)),
	.Create  = std_Date_Op_Create,
	.Destroy = std_Date_Op_Destroy,
	.Print   = std_Date_Op_Print,
	.Scan 	 = std_Date_Op_Scan,
	.Copy 	 = std_Date_Op_Copy,
	.Size    = std_Date_Op_Size,
	.Set     = std_Date_Op_Set,
	.Iter  	 = nilmethod,
	.Hash    = nilmethod,
	.Read	 = nilmethod,
	.Write   = nilmethod
){
	this.nanosec = arg.time->nanosec;

	var conv = convert->sec.to.years(arg.time->seconds);
	
	conv = (this.year   = conv.result, convert->sec.to.months(conv.remainder));
	conv = (this.month  = conv.result, convert->sec.to.days(conv.remainder));
	conv = (this.day    = conv.result, convert->sec.to.hours(conv.remainder));
	conv = (this.hour   = conv.result, convert->sec.to.minutes(conv.remainder));
	this.minute = conv.result;
	this.second = conv.remainder;

	if(!std.Date.isValid(self)){
		ERR(ERR_INITFAIL, "failed to properly set up date");
		return nil;
	}

return self;
}


