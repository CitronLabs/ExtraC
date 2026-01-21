#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>

#include <XC/pkg.c>

#undef read
#undef write

#define module XC, Common, Posix, IO

int moduleFn(open)(std_FSPath path, int flags){
	
	int fd = -1;

	if(getbitflag(flags, O_WRONLY) && getbitflag(flags, O_RDONLY)){
		flags ^= O_WRONLY;
		flags ^= O_RDONLY;
		flags |= O_RDWR;
	}

	if((fd = open(path, flags)) == -1){
		switch (errno) {
		
		}
	}

return fd;
}

errvt moduleFn(search)(std_FSPath path, std_FSEntry* entry){
	nonull(path, entry){ return err; }

	struct stat statbuf;
	if(stat(path, &statbuf) == -1){
		switch (errno) {
		
		}
	}

	if(entry == nil) return OK;

	entry->type.is.dir = S_ISDIR(statbuf.st_mode);
	
	entry->time_created = statbuf.st_ctime;
	entry->time_modified = statbuf.st_mtime;

	u32 nameoffset = 0, pathlen = strnlen((char*)path, 255);
	
	loop(i, pathlen){
	   if(path[pathlen - i] == '/'){
		nameoffset = pathlen - i + 1;
		break;
	   }
	}
	loop(i, pathlen - nameoffset)
		entry->name[i] = path[nameoffset + i];
return OK;
}

errvt moduleFn(delete)(std_FSPath path){
	nonull(path){ return err; }

	if(-1 == remove(path)){
		switch (errno) {
		
		}	
	}
return OK;
}

i64 moduleFn(read)(int handle, void* buff, u64 size){
	nonull(buff){ return err; }
	u64 bytesread = 0;
	if(-1 == (bytesread = read(handle, buff, size))){
		switch (errno) {
		
		}	
	}
return bytesread;
}





i64 moduleFn(write)(int handle, void* buff, u64 size){
	nonull(buff){ return err; }
	u64 byteswritten = 0;
	if(-1 == (byteswritten = write(handle, buff, size))){
		switch (errno) {
		
		}	
	}
return byteswritten;
}
errvt moduleFn(chdir)(std_FSPath path){
	nonull(path){ return err; }
	if(-1 == chdir(path)){
		switch (errno) {
		
		}	
	}
return OK;
}

errvt moduleFn(close)(int handle){
	if(handle == STDIN_FILENO || handle == STDOUT_FILENO || handle == STDERR_FILENO) 
		return ERR(ERR.INVALID, "cannot close the stdout, stdin, or stderr");
	if((close(handle)) == -1){
		switch (errno) {
		
		}
	}
return OK;
}
