#include "./time.h"

private(Watch,
	inst(Time) time_limit;
	struct timespec curr_time;
	struct timespec start_time;
	struct timespec wait_time;
	bool active;
);


inst(Time) methodimpl(Watch, GetLimit){
	nonull(self, return null);
	return priv.time_limit;
}

errvt methodimpl(Watch, Restart){
	
	nonull(self, return err);
	
	if(TIME_UTC != timespec_get(&priv.start_time, TIME_UTC)) {
		return ERR(ERR_INVALID, "could not restart time");
	}

return OK;
}
errvt methodimpl(Watch, Start){

	nonull(self, return err);

	if(TIME_UTC != timespec_get(&priv.start_time, TIME_UTC)) 
		return ERR(ERR_INVALID, "could not get current time");	
	
	priv.active = true;

	if(priv.start_time.tv_nsec != 0){
		priv.wait_time.tv_nsec += priv->curr_time.tv_nsec;
		priv.wait_time.tv_sec += priv->curr_time.tv_sec;
	}

return OK;
}
inst(Time) methodimpl(Watch, Check){
	nonull(self, return null);

	if(TIME_UTC != timespec_get(&priv.curr_time, TIME_UTC)){
		ERR(ERR_INVALID, "could not get current time");
		return null;
	}

	Time_Instance
		start_time = {0},
		curr_time = {0},
		wait_time = {0};

	Time.FromCTime(&start_time, priv.start_time);
	Time.FromCTime(&curr_time, priv.curr_time);
	Time.FromCTime(&wait_time, priv.wait_time);


return Time.Difference(
	    Time.Difference(
		&start_time,
		&curr_time
	    ),  &wait_time
	);
}
inst(Time) methodimpl(Watch, Stop){
	nonull(self, return null);

	
	struct timespec result = {0};

	priv.active = false;

return  Watch.Check(self);
}

construct(Watch,
	.Check = Watch_Check,
	.Start = Watch_Start,
	.Stop = Watch_Stop,
){
	priv.time_limit = arg.limit

return self;
}
