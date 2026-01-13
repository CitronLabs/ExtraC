#pragma once
#define __XC_ENV_LIB_C__
#include "../pkg.h"
#include "stddef.h" 


#ifndef __XC_LIBC_HIDETYPES__

// Structure containing monetary and numeric formatting information
struct lconv {
    char *decimal_point;      // Decimal point for non-monetary formatting
    char *thousands_sep;      // Separator for groups of digits (non-monetary)
    char *grouping;           // String indicating size of digit groups (non-monetary)
    char *int_curr_symbol;    // International currency symbol
    char *currency_symbol;    // Local currency symbol
    char *mon_decimal_point;  // Decimal point for monetary formatting
    char *mon_thousands_sep;  // Separator for groups of digits (monetary)
    char *mon_grouping;       // String indicating size of digit groups (monetary)
    char *positive_sign;      // Sign for non-negative monetary quantity
    char *negative_sign;      // Sign for negative monetary quantity
    char int_frac_digits;     // Number of fractional digits (international)
    char frac_digits;         // Number of fractional digits (local)
    char p_cs_precedes;       // 1 if currency_symbol precedes value (positive)
    char p_sep_by_space;      // 1 if space separates currency_symbol and value (positive)
    char n_cs_precedes;       // 1 if currency_symbol precedes value (negative)
    char n_sep_by_space;      // 1 if space separates currency_symbol and value (negative)
    char p_sign_posn;         // Position of positive_sign
    char n_sign_posn;         // Position of negative_sign
    // C99 additions:
    char int_p_cs_precedes;   // International equivalent of p_cs_precedes
    char int_n_cs_precedes;   // International equivalent of n_cs_precedes
    char int_p_sep_by_space;  // International equivalent of p_sep_by_space
    char int_n_sep_by_space;  // International equivalent of n_sep_by_space
    char int_p_sign_posn;     // International equivalent of p_sign_posn
    char int_n_sign_posn;     // International equivalent of n_sign_posn
};

#endif

Interface(C_LOCALE,
	char* fn(setlocale)(int category, const char *locale); 

	struct lconv* fn(localeconv)();
)

#ifndef __XC_LIBC_HIDEMACROS__

#define LC_ALL      1 // Affects all categories below
#define LC_COLLATE  2 // Affects strcoll and strxfrm (string comparison)
#define LC_CTYPE    3 // Affects character handling functions (ctype.h, wctype.h)
#define LC_MONETARY 4 // Affects monetary formatting (lconv)
#define LC_NUMERIC  5 // Affects numeric formatting (lconv, printf/scanf)
#define LC_TIME     6 // Affects strftime (date and time formatting)

#define setlocale(category, _locale) C.locale.setlocale(category, _locale)
#define localeconv()		     C.locale.localeconv()

#endif
