#pragma once
#include "../types.h"
#include "XC.Core/Essential/utils.h"

typedef struct XC_Locale_Data {

	const char * Identifier;

	struct {
	    const char
	    	* day_names[7],
	    	* short_day_names[7],
	    
	    	* month_names[13],
	    	* short_month_names[13]
	    ;

	    byte first_day_in_week;
	} Time;
	struct {
	    const char 
	   	* currency_symbol,
	    	* int_curr_symbol,
    	    	* positive_sign,            
    	    	* negative_sign,            
    	    	* mon_grouping;          
    	    int 
		frac_digits,                
    	    	int_frac_digits,            
    	     	mon_decimal_point,        
    	     	mon_thousands_sep,        

    	    	pos_cs_precedes,              
    	    	pos_sep_by_space,             
    	    	pos_sign_posn,                

    	    	neg_cs_precedes,              
    	    	neg_sep_by_space,             
    	    	neg_sign_posn;                
	} Currency;
	struct {
	// System of Measurement
	    word measurement_system;           // The primary system for length, weight, and volume
	// Temperature System
	    word temperature_system;
	} Measure;
	
} XC_Locale_Data;


Interface(__XC_SYS_LOCALE,
	values(Temp, word,
		Celsius,
		Fahrenheit
	)
	values(Measure, word,
		Metric,
		Imperial
	)

)
