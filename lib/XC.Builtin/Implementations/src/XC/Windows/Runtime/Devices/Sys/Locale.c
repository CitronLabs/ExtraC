#include "../Device.h"

alias(std.String.Encoding, Str)

static char* moduleFn(GetLocaleStr)(LPCWSTR localeName, LCTYPE lcType) {
    	int wideLen   = GetLocaleInfoEx(localeName, lcType, NULL, 0);
    	len_t ansiLen = 0;

    	if (wideLen <= 0){ 
    	    ERR(ERR.FAIL, "failed to get locale info");
    	    return nil;
    	}

    	strc16 wideBuff = malloc(wideLen * sizeof(c16));
    	strc8  ansiBuff = malloc(wideLen * sizeof(rune));


    	GetLocaleInfoEx(localeName, lcType, generic wideBuff, wideLen);

    	if(!Str.UTF16.toUtf8(wideBuff, wideLen, ansiBuff, wideLen)){
    	    ERR(ERR.FAIL, "failed to get locale info");

    	    free(wideBuff);
    	    free(ansiBuff);
    	    return nil;
    	}

return ansiBuff;
}

static int moduleFn(GetLocaleInt)(LPCWSTR localeName, LCTYPE lcType) {
    	int value = 0;
    	GetLocaleInfoEx(localeName, lcType | LOCALE_RETURN_NUMBER, (LPWSTR)&value, sizeof(value) / sizeof(WCHAR));
return value;
}


static errvt moduleFn(SetLocale)(void* localeName, XC_Locale_Data* localeData){

	// 1. Identifier
    	localeData->Identifier = mod(GetLocaleStr)(localeName, LOCALE_SNAME);

    	// 2. Time Data
    	loop(i, lenof(localeData->Time.day_names)) {
    	    localeData->Time.day_names[i]       = mod(GetLocaleStr)(localeName, LOCALE_SDAYNAME1 + i);
    	    localeData->Time.short_day_names[i] = mod(GetLocaleStr)(localeName, LOCALE_SABBREVDAYNAME1 + i);
    	}

    	// Month 13 is used in some calendars (e.g., Hebrew Lunar), otherwise returns empty
    	loop(i, lenof(localeData->Time.month_names)) {
    	    localeData->Time.month_names[i]       = mod(GetLocaleStr)(localeName, LOCALE_SMONTHNAME1 + i);
    	    localeData->Time.short_month_names[i] = mod(GetLocaleStr)(localeName, LOCALE_SABBREVMONTHNAME1 + i);
    	}

    	localeData->Time.first_day_in_week = (BYTE)mod(GetLocaleInt)(localeName, LOCALE_IFIRSTDAYOFWEEK);

    	// 3. Currency Data (Strings)
    	const char* dec 	= mod(GetLocaleStr)(localeName, LOCALE_SMONDECIMALSEP);
    	const char* sep 	= mod(GetLocaleStr)(localeName, LOCALE_SMONTHOUSANDSEP);

	localeData->Currency = (typeof(localeData->Currency)){
    	.currency_symbol   	= mod(GetLocaleStr)(localeName, LOCALE_SCURRENCY),
    	.int_curr_symbol   	= mod(GetLocaleStr)(localeName, LOCALE_SINTLSYMBOL),
    	.positive_sign     	= mod(GetLocaleStr)(localeName, LOCALE_SPOSITIVESIGN),
    	.negative_sign     	= mod(GetLocaleStr)(localeName, LOCALE_SNEGATIVESIGN),
    	.mon_grouping      	= mod(GetLocaleStr)(localeName, LOCALE_SMONGROUPING),

    	.frac_digits       	= mod(GetLocaleInt)(localeName, LOCALE_ICURRDIGITS),
    	.int_frac_digits   	= mod(GetLocaleInt)(localeName, LOCALE_IINTLCURRDIGITS),
    	
    	.mon_decimal_point 	= (dec) ? (int)dec[0] : 0,
    	
    	.mon_thousands_sep 	= (sep) ? (int)sep[0] : 0,

    	.pos_cs_precedes   	= mod(GetLocaleInt)(localeName, LOCALE_IPOSSYMPRECEDES),
    	.pos_sep_by_space  	= mod(GetLocaleInt)(localeName, LOCALE_IPOSSEPBYSPACE),
    	.pos_sign_posn     	= mod(GetLocaleInt)(localeName, LOCALE_IPOSSIGNPOSN),
    	
    	.neg_cs_precedes   	= mod(GetLocaleInt)(localeName, LOCALE_INEGSYMPRECEDES),
    	.neg_sep_by_space  	= mod(GetLocaleInt)(localeName, LOCALE_INEGSEPBYSPACE),
    	.neg_sign_posn     	= mod(GetLocaleInt)(localeName, LOCALE_INEGSIGNPOSN)

	};

    	localeData->Measure.measurement_system = (WORD)mod(GetLocaleInt)(localeName, LOCALE_IMEASURE);
    	
    	// In Windows, if measurement is 0 (Metric) temperature is usually Celsius (0)
    	// If measurement is 1 (US) temperature is Fahrenheit (1).
    	localeData->Measure.temperature_system = (localeData->Measure.measurement_system == 0) ? 0 : 1;

return OK;
}

errvt moduleFn(Sys_Locale_open)(registerHandle handle, bool create){
	var LocaleResource = Dev.Resource.getOne(
		WinRTDev.getManager(),
		WinRTDev.getSys(),
		(pntrval)handle
	);

	if(LocaleResource == nil){
		return ERR(ERR.INVALID, "Invalid handle");
	}

	LocaleResource->data = (XC_Locale_Data*)malloc(sizeof(XC_Locale_Data) + sizeof(u8));

	if(LocaleResource->data == nil)
		return ERR(ERR.FAIL, "failed to alloc locale register");
	
	
	iferr(mod(SetLocale)(LOCALE_NAME_SYSTEM_DEFAULT, LocaleResource->data))
		return ERR(ERR.FAIL, "failed to set locale");
	
	*(u8*)(LocaleResource->data + sizeof(XC_Locale_Data)) = false;

return OK;
}

errvt moduleFn(Sys_Locale_close)(registerHandle handle){
	var LocaleResource = Dev.Resource.getOne(
		WinRTDev.getManager(),
		WinRTDev.getSys(),
		(pntrval)handle
	);

	if(LocaleResource == nil){
		return ERR(ERR.INVALID, "Invalid handle");
	}

	free(LocaleResource->data);
return OK;
}

errvt moduleFn(Sys_Locale_watch)(registerHandle handle){
// Locale is being "watched" for changes with the extra byte at the end
return OK;
}
errvt moduleFn(Sys_Locale_isModified)(registerHandle handle){
	var LocaleResource = Dev.Resource.getOne(
		WinRTDev.getManager(),
		WinRTDev.getSys(),
		(pntrval)handle
	);

	if(LocaleResource == nil){
		return ERR(ERR.INVALID, "Invalid handle");
	}

return *(u8*)(LocaleResource->data + sizeof(XC_Locale_Data));
}

len_t moduleFn(Sys_Locale_readFrom)(registerHandle handle, const void* buffer, len_t size){
	if(size != sizeof(XC_Locale_Data) || size != 0){
		ERR(ERR.INVALID, "Invalid size, can only be either 0 or sizeof(XC_Locale_Data)");
		return 0;
	}

	var LocaleResource = Dev.Resource.getOne(
		WinRTDev.getManager(),
		WinRTDev.getSys(),
		(pntrval)handle
	);

	if(LocaleResource == nil){
		ERR(ERR.INVALID, "Invalid handle");
		return 0;
	}

	memcpy(LocaleResource->data, generic buffer, sizeof(XC_Locale_Data));
	
return sizeof(XC_Locale_Data);
}
len_t moduleFn(Sys_Locale_writeTo) (registerHandle handle, const void* buffer, len_t size){
	var LocaleResource = Dev.Resource.getOne(
		WinRTDev.getManager(),
		WinRTDev.getSys(),
		(pntrval)handle
	);

	if(LocaleResource == nil){
		ERR(ERR.INVALID, "Invalid handle");
		return 0;
	}

	memcpy(generic buffer, LocaleResource->data, sizeof(XC_Locale_Data));

return sizeof(XC_Locale_Data);
}
len_t moduleFn(Sys_Locale_setTo)   (registerHandle handle, const void* buffer, len_t size){
	var LocaleResource = Dev.Resource.getOne(
		WinRTDev.getManager(),
		WinRTDev.getSys(),
		(pntrval)handle
	);

	if(LocaleResource == nil){
		ERR(ERR.INVALID, "Invalid handle");
		return 0;
	}

	memcpy(LocaleResource->data, generic buffer, sizeof(XC_Locale_Data));

return sizeof(XC_Locale_Data);
}

registerInfo moduleFn(Sys_Locale_info)(registerHandle handle){
return (registerInfo){
	.name		= "Locale",
	.path 		= "Locale",
	.type 		= nil,
	.time_created 	= 0,
	.time_modified  = 0,
	.size 		= sizeof(XC_Locale_Data),
	.currentPos 	= 0,
};
}
