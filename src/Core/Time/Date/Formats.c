#include <XC.Core/pkg.c>

alias(std.Time.Convert, Convert);

enum(Date_Format,
	std_Date_Format_Null,
	std_Date_Format_US,
	std_Date_Format_Time_12Hour,
	std_Date_Format_Time_24Hour,
	std_Date_Format_EU,
	std_Date_Format_FullName,
	std_Date_Format_ISODate,
	std_Date_Format_ISOTime,
	std_Date_Format_RFC,
)


PRINT(std_Date){
    nonull(self){ return 0; }

    if(!std.Date.isValid(self)){
	ERR(ERR.INVALID, "invalid date");
	return 0;
    }

    len_t result = 0;

    switch(f.type){
    case std_Date_Format_US:{
	result = printTo(out, 
       		$((u8)this.month), "/",
       		$((u8)this.day),   "/",
       		$(this.year)
       );
    break;}
    case std_Date_Format_EU:{
	result = printTo(out, 
       		$((u8)this.day),   "/",
       		$((u8)this.month), "/",
       		$(this.year)
       );
    break;}
    case std_Date_Format_ISODate:{
	result = printTo(out, 
       		$(this.year),      "-",
       		$((u8)this.month), "-",
       		$((u8)this.day)
       );
    break;}
    case std_Date_Format_Time_12Hour:{
	result = printTo(out, 
       		$((u8)
	   	    this.hour % 12 ?
	   		(this.hour % 12) : 12	
	   	),":",
       		$((u8)this.minute)," ",
       		this.hour / 12 == 1 ? "PM" : "AM"
       	);
    break;}
    case std_Date_Format_Time_24Hour:{
	result = printTo(out, 
       		$((u8)this.hour), ":", $((u8)this.minute)
       	);
    break;}
    case std_Date_Format_ISOTime:{
	result = printTo(out, 
       		$F((u8)this.hour,   .precision = 2), ":", 
	 	$F((u8)this.minute, .precision = 2), ":",
	 	$F((u8)this.second, .precision = 2), ".",
	 	$F(Convert.nano_to_milli(this.nanosec).result, .precision = 3), "Z"
       	);
    break;}
    case std_Date_Format_FullName:{
	result = printTo(out,
		generic std.Date.getDayName(self, false), ", ",
	 	generic std.Date.getMonthName(self, false), " ",
	 	$((u8)this.day), ", ", $(this.year)
	);
    break;}
    case std_Date_Format_RFC:{
	result = printTo(out,
		generic std.Date.getDayName(self,   true), ", ",
	 	$((u8)this.day), " ",
	 	generic std.Date.getMonthName(self, true), " ",
	 	$(this.year), " ",
       		$F((u8)this.hour,       .precision = 2),  ":", 
	 	$F((u8)this.minute,     .precision = 2),  ":",
	 	$F((u8)this.second,     .precision = 2),  " +",
	 	$F(this.offset_seconds, .precision = 4)
	);
    break;}
    default:{ 
	ERR(ERR.INVALID, "invalid date format type");
	return 0;
    }
    }

return result;
}

SCAN(std_Date){
    nonull(self){ return 0; }

    if(!std.Date.isValid(self)){
	ERR(ERR.INVALID, "invalid date");
	return 0;
    }

    len_t result = 0;

    switch(format->type){
    case std_Date_Format_US:{
	result = scanFrom(in, 
       		$((u8)this.month), "/",
       		$((u8)this.day),   "/",
       		$(this.year)
       );
    break;}
    case std_Date_Format_EU:{
	result = scanFrom(in, 
       		$((u8)this.day),   "/",
       		$((u8)this.month), "/",
       		$(this.year)
       );
    break;}
    case std_Date_Format_ISODate:{
	result = scanFrom(in, 
       		$(this.year),      "-",
       		$((u8)this.month), "-",
       		$((u8)this.day)
       );
    break;}
    case std_Date_Format_Time_12Hour:{
	result = scanFrom(in, 
       		$((u8)
	   	    this.hour % 12 ?
	   		(this.hour % 12) : 12	
	   	),":",
       		$((u8)this.minute)," ",
       		this.hour / 12 == 1 ? "PM" : "AM"
       	);
    break;}
    case std_Date_Format_Time_24Hour:{
	result = scanFrom(in, 
       		$((u8)this.hour), ":", $((u8)this.minute)
       	);
    break;}
    case std_Date_Format_ISOTime:{
	result = scanFrom(in, 
       		$F((u8)this.hour,   .precision = 2), ":", 
	 	$F((u8)this.minute, .precision = 2), ":",
	 	$F((u8)this.second, .precision = 2), ".",
	 	$F(Convert.nano_to_milli(this.nanosec).result, .precision = 3), "Z"
       	);
    break;}
    case std_Date_Format_FullName:{
	result = scanFrom(in,
		generic std.Date.getDayName(self, false), ", ",
	 	generic std.Date.getMonthName(self, false), " ",
	 	$((u8)this.day), ", ", $(this.year)
	);
    break;}
    case std_Date_Format_RFC:{
	result = scanFrom(in,
		generic std.Date.getDayName(self,   true), ", ",
	 	$((u8)this.day), " ",
	 	generic std.Date.getMonthName(self, true), " ",
	 	$(this.year), " ",
       		$F((u8)this.hour,       .precision = 2),  ":", 
	 	$F((u8)this.minute,     .precision = 2),  ":",
	 	$F((u8)this.second,     .precision = 2),  " +",
	 	$F(this.offset_seconds, .precision = 4)
	);
    break;}
    default:{ 
	ERR(ERR.INVALID, "invalid date format type");
	return 0;
    }
    }

return result;
}
