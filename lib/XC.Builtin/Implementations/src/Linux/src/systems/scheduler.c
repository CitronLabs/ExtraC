#include "__systems.h"

#include <bits/pthreadtypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, strc8 argv[]);
type(LinuxThread,
	pthread_t thread;
);

void moduleFn(LinuxScheduler, sleep, u64 millisec){
	usleep(millisec);
}

threadHandle moduleFn(LinuxScheduler, initThread, void fn(startfn,void*), void* args){
	nonull(startfn, return null);

	LinuxThread* result = new(LinuxThread);

	if(-1 == pthread_create(&result->thread, null, (void*(*)(void*))startfn, args)){
		switch (errno) {
		
		}
	}

return result;
}


void childProcessStart(u32 flags, strc8 exePath, strc8* args){

	if(getbitflag(flags, PROCFLAG_DEBUG)){
		ptrace(PTRACE_TRACEME, 0, null, NULL);
		switch (errno) {
			
		}
	}


	execv(exePath, args);
	switch (errno) {
			
	}

	exit(-1);
}

processHandle parentProcessStart(u32 flags, processHandle child){

	if(getbitflag(flags, PROCFLAG_DEBUG)){
		ptrace(PTRACE_ATTACH, child, null, NULL);	
		switch (errno) {
	
		}
	}
	
	waitpid((pid_t)(u64)child, null, 0);
	switch (errno) {
	
	}

return child;
}

processHandle moduleFn(LinuxScheduler, initProcess, strc8 exePath, strc8 args, u32 flags){
	nonull(exePath, return null);

	List(strc8) list = pushList(strc8, 10);
	u64 start = 0;	
	for(int i = 0;;){
		if(args[i] == '\0') break;
		if(args[i] == ' '){
			List.Append(list, push(strc8, &args[start], i - start), 1);
			i++;
			while(!isalpha(args[i]) && args[i] != '\0') i++;
			if(args[i] == '\0') break;
			start = i;
		}
		i++;	
	}
	List.Append(list, &(void*){0}, 1);
	processHandle result = (processHandle)(u64) fork();
	if(result == (processHandle)(i64)-1) {
		switch (errno) {
		
		}
		return nil;
	}
	if(result == (processHandle)0)
		childProcessStart(flags, exePath, List.GetPointer(list, 0));
	else
		result = parentProcessStart(flags, result); // result gets null'd out if error

	pop(list);

return result;
}

errvt moduleFn(LinuxScheduler, killProcess, processHandle handle){
	nonull(handle){ return err; }

	if(kill(addrasval(handle), SIGTERM) == -1){
		switch (errno) {
		case EPERM:{return ERR(IOERR_PERMS, "invalid permission to kill process");}	
		case ESRCH:{return ERR(ERR_INVALID, "invalid handle to process");}
		}
		return ERR(ERR_FAIL, "failed to kill process");
	}
	if(kill(addrasval(handle), SIGKILL) == -1){
		switch (errno) {
		case EPERM:{return ERR(IOERR_PERMS, "invalid permission to kill process");}	
		case ESRCH:{return ERR(ERR_INVALID, "invalid handle to process");}
		}
		return ERR(ERR_FAIL, "failed to kill process");
	}
return OK; 
}


threadHandle moduleFn(LinuxScheduler, getCurrentThread){
	threadHandle result = (threadHandle)pthread_self();
return result;
}

typedef struct {pthread_mutex_t mut;}lin_mutex;
asClass(lin_mutex){ passover }
mutexHandle moduleFn(LinuxScheduler, initMutex){
	mutexHandle result = new(lin_mutex, PTHREAD_MUTEX_INITIALIZER);
return result;
}
errvt moduleFn(LinuxScheduler, lockMutex, mutexHandle handle){
	if(pthread_mutex_lock(&((lin_mutex*)handle)->mut) == -1){
		switch (errno) {
		case EINVAL : { return ERR(ERR_FAIL, "invalid mutex"); }
		case EAGAIN : { return ERR(ERR_FAIL, "The maximum number of recursive locks for this mutex has been exceeded."); }
		case EDEADLK: { return ERR(ERR_FAIL, "A deadlock condition was detected or the current thread already owns the mutex."); }
		}
		return ERR(ERR_FAIL, "failed to lock mutex");
	}
return OK;
}
errvt moduleFn(LinuxScheduler, unlockMutex, mutexHandle handle){
	if(pthread_mutex_unlock(&((lin_mutex*)handle)->mut) == -1){
		switch (errno) {
		case EPERM  : { return ERR(ERR_FAIL, "The current thread does not own the mutex."); }
		case EAGAIN : { return ERR(ERR_FAIL, "The maximum number of recursive locks for this mutex has been exceeded."); }
		case EINVAL : { return ERR(ERR_FAIL, "invalid mutex"); }
		}
		return ERR(ERR_FAIL, "failed to unlock mutex");
	}
return OK;
}
errvt moduleFn(LinuxScheduler, tryLockMutex, mutexHandle handle){
	if(pthread_mutex_trylock(&((lin_mutex*)handle)->mut) == -1){

		switch (errno) {
		case EBUSY  : { return ERR(THREADERR_MUTEX_LOCKED, "The mutex is already locked"); }
		case EAGAIN : { return ERR(ERR_FAIL, "The maximum number of recursive locks for this mutex has been exceeded."); }
		case EINVAL : { return ERR(ERR_FAIL, "invalid mutex"); }
		}
		return ERR(ERR_FAIL, "failed to lock mutex");
	}
return OK;
}

typedef struct {sem_t sem;}lin_semaphore;
asClass(lin_semaphore){ passover }

semaphoreHandle moduleFn(LinuxScheduler, initSemaphore, len_t num){
	lin_semaphore* result = new(lin_semaphore);
	sem_init(&result->sem, 0, num);
	switch (errno) {
	case EINVAL: { ERR(ERR_INVALID, "value exceeds SEM_VALUE_MAX"); return nil; } 
	}
return result;
}

errvt moduleFn(LinuxScheduler, waitSemaphore, semaphoreHandle handle){
	if(sem_wait(&((lin_semaphore*)handle)->sem) == -1){
		switch (errno) {
		
		}
	}
return OK;
}
errvt moduleFn(LinuxScheduler, postSemaphore, semaphoreHandle handle){
	if(sem_post(&((lin_semaphore*)handle)->sem) == -1){
		switch (errno) {
		
		}
	}
return OK;
}
errvt moduleFn(LinuxScheduler, tryWaitSemaphore, semaphoreHandle handle){
	if(sem_wait(&((lin_semaphore*)handle)->sem) == -1){
		switch (errno) {
		
		}
	}
return OK;
}

errvt moduleFn(LinuxScheduler, handleProcEvents, processHandle process, Queue(OSEvent) evntQueue){}
errvt moduleFn(LinuxScheduler, handleThrdEvents, threadHandle thread, Queue(OSEvent) evntQueue){}
errvt moduleFn(LinuxScheduler, waitThread){}
bool moduleFn(LinuxScheduler,  isProcessRunning){}
u64 moduleFn(LinuxScheduler,   pollEvents){}
errvt moduleFn(LinuxScheduler, initSystem){}
errvt moduleFn(LinuxScheduler, exitSystem){}

const ImplAs(scheduler, LinuxScheduler){
	.pollEvents 	   	= LinuxScheduler_pollEvents,
	.initSystem 	   	= LinuxScheduler_initSystem,
	.exitSystem 	   	= LinuxScheduler_exitSystem,
	.thread = {
		.init	   	= LinuxScheduler_initThread,
		.sleep		= LinuxScheduler_sleep,
		.getCurrent  	= LinuxScheduler_getCurrentThread,
		.wait  	   	= LinuxScheduler_waitThread,
		.handleEvents  	= LinuxScheduler_handleThrdEvents,
	},
	.process = {
		.init	   	= LinuxScheduler_initProcess,
		.isRunning  	= LinuxScheduler_isProcessRunning,
		.kill	   	= LinuxScheduler_killProcess,
		.handleEvents  	= LinuxScheduler_handleProcEvents,
	},
	.ctrl = {
	    .mutex = {
		.init		= LinuxScheduler_initMutex,
		.lock		= LinuxScheduler_lockMutex,
		.unlock		= LinuxScheduler_unlockMutex,
		.tryLock	= LinuxScheduler_tryLockMutex,
	    },
	    .semaphore = {
		.init	   	= LinuxScheduler_initSemaphore,
		.wait		= LinuxScheduler_waitSemaphore,
		.post		= LinuxScheduler_postSemaphore,
		.tryWait	= LinuxScheduler_tryWaitSemaphore,
	    }
	}
};
