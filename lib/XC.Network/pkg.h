#pragma once
#include <XC.Core/pkg.h>
#include <XC.Alloc/pkg.h>

#define package std

Interface(Network, 
	values(Protocol, word,
		TCP,
		UDP,
		RAW
	)

	values(Domain, word,
		IPV4,
		IPV6,
		LOCAL
	)
)


#undef package
