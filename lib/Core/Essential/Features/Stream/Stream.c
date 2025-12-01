#include "../../../pkg.h"

import(std)

import(XC)


use(std,
	Array_Stack,
	Array_List,
    	Local,
    	Stream,
    	Stream_Options
);

typedef struct {
	Stream* 		activeStream;
	ArrayStack(Stream*) 	activeStreamStack;
	std_StreamDecoder 	activeDecoder;
	ArrayStack(len_t)	frameSizeStack;
	ArrayStack(Stream_Proc*)activeProc;
} localStreamContext;

#define STREAM_TYPE_REAL 1
#define STREAM_TYPE_MEM  2
#define STREAM_TYPE_BUFF 3


localStreamContext* fetchLocalStreamCtx(){
	static Local* localStreamCtx = null;
	
	localStreamContext* result = null;

	if(!localStreamCtx){
		localStreamCtx = new(Local, 
		       sizeof(localStreamContext)
		);

		if(!localStreamCtx){
		    ERR(ERR_INITFAIL, 
       			"failed to initalize local Stream context");
		
		    XC.Sys.terminate(XC.Sys.ExitCode.FAILURE, 0);	
		}

	 	result = std.Local.getData(localStreamCtx);

		if(!result){
		    ERR(ERR_INITFAIL, 
       			"failed to get local Stream context");
		
		    XC.Sys.terminate(XC.Sys.ExitCode.FAILURE, 0);	
		}

		result->activeStreamStack = newArrayStack(Stream*, 5);
	} else {
	 	result = std.Local.getData(localStreamCtx);

		if(!result){
		    ERR(ERR_INITFAIL, 
       			"failed to get local Stream context");
		
		    XC.Sys.terminate(XC.Sys.ExitCode.FAILURE, 0);	
		}
	}
return result;
}


Stream* fetchStdStream(int id, Stream** stream){
	if(!*stream){
	    *stream = new(Stream,
		std.Stream.Preset.fromHandle(
		    XC.Dev.Stream.stdHandle(id)
		)
	    );

	    if(!*stream){
		ERR(ERR_INITFAIL, "failed to initalize stdout stream");
		return nil;
	    }
	}

return *stream;
}

Stream* std_Stream_stdOut(){ static Stream* stream = null; return fetchStdStream(XC.Dev.Stream.ID.Out, &stream); }
Stream* std_Stream_stdIn(){  static Stream* stream = null; return fetchStdStream(XC.Dev.Stream.ID.In,  &stream); }
Stream* std_Stream_stdErr(){ static Stream* stream = null; return fetchStdStream(XC.Dev.Stream.ID.Err, &stream); }


const std_Stream_Options std_Stream_Preset_staticBuffer(void* start, len_t len){
	return (std_Stream_Options){
		.setMemoryAddr = start,
		.setMemorySize = len
	};
}

const std_Stream_Options std_Stream_Preset_fromHandle(streamHandle handle){
	return (std_Stream_Options){
		.handle = handle
	};
}

streamHandle methodimpl(std_Stream, getHandle){
	nonull(self, return streamHandle_Invalid);

return priv.stream.handle;
}

errvt methodimpl(std_Stream, Flush){
	nonull(self, return err)

return priv.stream.handle ? XC.Dev.Stream.flush(priv.stream.handle) : OK;
}	
void* methodimpl(std_Stream, ToPointer){
	nonull(self, return null);

	switch(priv.flags.streamType){
	case STREAM_TYPE_REAL:{
		streamInfo info = XC.Dev.Stream.info(priv.stream.handle);

		iferr(!info.valid){
			ERR(ERR_FAIL, "failed to get stream size");
			return null;
		}

		priv.pointer = malloc(info.size);
		
		if(!priv.pointer){
			ERR(ERR_FAIL, "failed to allocate stream pointer buff");
			return null;
		}

		if(!XC.Dev.Stream.readFrom(priv.stream.handle, priv.pointer, info.size)){
			ERR(ERR_FAIL, "failed to read data into stream pointer buff");
			free(priv.pointer);
			return null;
	  	}

	break;}
	case STREAM_TYPE_MEM: {
		priv.pointer = malloc(size(priv.stream.mem.data) - priv.stream.mem.pos);

		if(!priv.pointer){
			ERR(ERR_FAIL, "failed to allocate stream pointer buff");
			return null;
		}
	break;}
	case STREAM_TYPE_BUFF: {
		priv.pointer = priv.stream.buff.data;
	break;}
	default:{
		ERR(ERR_INVALID, "invalid stream type");
	  	return null;
	}
	}

return priv.pointer;
}
void* methodimpl(std_Stream, GetPointer){ return priv.pointer ? priv.pointer : std.Stream.ToPointer(self); }

typedef struct Stream_Proc SP_Result;
extern const struct Stream_Proc Stream_Proc_Fail, Stream_Proc_OK;

static inline const struct Stream_Proc setProc(localStreamContext* ctx, const struct Stream_Proc* to){

	if(ctx->activeProc->items) 
	    *(const struct Stream_Proc**) index(ctx->activeProc, 
		  ctx->activeProc->items) = to;
return *to;
}


SP_Result std_Stream_Process_start(std_Stream* strm){
	localStreamContext* ctx = fetchLocalStreamCtx();

	nonull(strm,
		setProc(ctx,&Stream_Proc_Fail);
		return Stream_Proc_Fail
	);

	
	if(ctx->activeStream)
		write(ctx->activeStreamStack, &ctx->activeStream);	
		
	ctx->activeStream = strm;

return setProc(ctx, &Stream_Proc_OK);
}
SP_Result std_Stream_Process_cont(){
	localStreamContext* ctx = fetchLocalStreamCtx();

	Stream* self = ctx->activeStream;
	
	if(elements(ctx->frameSizeStack)){
		read(ctx->frameSizeStack, &priv.frameSize);
	}

return setProc(ctx, &Stream_Proc_OK);
}
SP_Result std_Stream_Process_doEncode(std_StreamEncoder encoder, void* data){
	localStreamContext* ctx = fetchLocalStreamCtx();

	nonull(encoder || data, return setProc(ctx, &Stream_Proc_Fail));

	iferr(encoder(ctx->activeStream, data)){
		ERR(ERR_FAIL, "failed to encode data into stream");
		return Stream_Proc_Fail;
	}
return setProc(ctx, &Stream_Proc_OK);
}

SP_Result std_Stream_Process_doDecode(std_StreamDecoder decoder){
	localStreamContext* ctx = fetchLocalStreamCtx();

	nonull(decoder, return setProc(ctx,&Stream_Proc_Fail));

	ctx->activeDecoder = decoder;

return setProc(ctx, &Stream_Proc_OK);
}

SP_Result std_Stream_Process_each(len_t frameSize){
	localStreamContext* ctx = fetchLocalStreamCtx();

	Stream* self = ctx->activeStream;
	
	if(!write(ctx->frameSizeStack, &priv.frameSize)){
		ERR(ERR_FAIL, "could not save previous frame size for each stream iteration");
		return setProc(ctx,&Stream_Proc_Fail);
	}

	priv.frameSize = frameSize;
	

return setProc(ctx, &Stream_Proc_OK);
}
SP_Result std_Stream_Process_advance(len_t num){
	localStreamContext* ctx = fetchLocalStreamCtx();
	
	Stream* self = ctx->activeStream;
	
	switch(priv.flags.streamType){
	case STREAM_TYPE_REAL:{
		streamInfo info = XC.Dev.Stream.info(priv.stream.handle);
		
		if(!info.valid){
			ERR(ERR_FAIL, "failed to get stream cursor position to advance");
			return setProc(ctx,&Stream_Proc_Fail);
		}

		iferr(XC.Dev.Stream.shift(priv.stream.handle, num, info.currentPos)){
			ERR(ERR_FAIL, "failed to shift stream cursor position to advance");
			return setProc(ctx,&Stream_Proc_Fail);
		}
	break;}
	case STREAM_TYPE_MEM:{
		if(num > (size(priv.stream.mem.data) / priv.frameSize) - priv.stream.mem.pos)
			priv.stream.mem.pos = size(priv.stream.mem.data) / priv.frameSize;
		else
	  		priv.stream.mem.pos += num;
	break;}
	case STREAM_TYPE_BUFF:{
		if((priv.stream.buff.pos + num) < priv.stream.buff.size) 
			priv.stream.buff.pos += num;
	break;}
	default:{
		ERR(ERR_INVALID, "invalid stream type");
	  	return setProc(ctx, &Stream_Proc_Fail);
	}
	}

return setProc(ctx, &Stream_Proc_OK);
}
SP_Result std_Stream_Process_rewind(len_t num){
	localStreamContext* ctx = fetchLocalStreamCtx();

	Stream* self = ctx->activeStream;

	switch(priv.flags.streamType){
	case STREAM_TYPE_REAL:{
		streamInfo info = XC.Dev.Stream.info(priv.stream.handle);
		
		if(!info.valid){
			ERR(ERR_FAIL, "failed to get stream cursor position to advance");
			return setProc(ctx,&Stream_Proc_Fail);
		}

		iferr(XC.Dev.Stream.shift(priv.stream.handle, -num, info.currentPos)){
			ERR(ERR_FAIL, "failed to shift stream cursor position to advance");
			return setProc(ctx,&Stream_Proc_Fail);
		}
	break;}
	case STREAM_TYPE_MEM:{
		if(num > priv.stream.mem.pos)
			priv.stream.mem.pos = 0;
		else
	  		priv.stream.mem.pos -= num;
	break;}
	case STREAM_TYPE_BUFF:{
		if((priv.stream.buff.pos - num) >= 0) 
			priv.stream.buff.pos -= num;
	break;}
	default:{
		ERR(ERR_INVALID, "invalid stream type");
	  	return setProc(ctx, &Stream_Proc_Fail);
	}
	}
	

return setProc(ctx, &Stream_Proc_OK);
}


SP_Result std_Stream_Process_readData(void* buff, len_t len){
	localStreamContext* ctx = fetchLocalStreamCtx();

	Stream* self = ctx->activeStream;

	switch(priv.flags.streamType){
	case STREAM_TYPE_REAL:{
		if(!XC.Dev.Stream.readFrom(priv.stream.handle, buff, len * priv.frameSize)){
			ERR(ERR_FAIL, "failed to read from stream");
			return setProc(ctx,&Stream_Proc_Fail);
		}
	break;}
	case STREAM_TYPE_MEM:{
		if(!memcpy(
		    buff,
		    pntr_shiftcpy(priv.stream.mem.data, len * priv.frameSize),
	  	    len * priv.frameSize
		)){
			ERR(ERR_FAIL, "failed to read from stream");
			return setProc(ctx,&Stream_Proc_Fail);
		}
	break;}
	case STREAM_TYPE_BUFF:{
	    	if(len > priv.stream.buff.size - priv.stream.buff.pos) 
		    len = priv.stream.buff.pos;

		memcpy(buff, priv.stream.buff.data, priv.frameSize * len);

		priv.stream.buff.pos += len;
	break;}
	default:{
		ERR(ERR_INVALID, "invalid stream type");
	  	return setProc(ctx, &Stream_Proc_Fail);
	}
	}
	
return setProc(ctx, &Stream_Proc_OK);
}
SP_Result std_Stream_Process_writeData(void* buff, len_t len){

	localStreamContext* ctx = fetchLocalStreamCtx();

	Stream* self = ctx->activeStream;

	switch(priv.flags.streamType){
	case STREAM_TYPE_REAL:{
		if(!XC.Dev.Stream.writeTo(priv.stream.handle, buff, len * priv.frameSize)){
			ERR(ERR_FAIL, "failed to write from stream");
			return setProc(ctx,&Stream_Proc_Fail);
		}
	break;}
	case STREAM_TYPE_MEM:{
		if(!memcpy(
		    buff,
		    pntr_shiftcpy(priv.stream.mem.data, len * priv.frameSize),
	  	    len * priv.frameSize
		)){
			ERR(ERR_FAIL, "failed to write from stream");
			return setProc(ctx,&Stream_Proc_Fail);
		}
	break;}
	case STREAM_TYPE_BUFF:{
	    	if(len > priv.stream.buff.size - priv.stream.buff.pos) 
		    len = priv.stream.buff.pos;

		memcpy(priv.stream.buff.data, buff, priv.frameSize * len);

		priv.stream.buff.pos += len;
	break;}
	default:{
		ERR(ERR_INVALID, "invalid stream type");
	  	return setProc(ctx, &Stream_Proc_Fail);
	}
	}
	
return setProc(ctx, &Stream_Proc_OK);

}

std_Stream* std_Stream_Process_current(){
	localStreamContext* ctx = fetchLocalStreamCtx();

return ctx->activeStream;
}

noFail std_Stream_Process_end(){
	localStreamContext* ctx = fetchLocalStreamCtx();

	if(elements(ctx->activeStreamStack) != 0){
		ctx->activeStream = index(
			ctx->activeStreamStack, 
			ctx->activeStreamStack->items
		);

		ctx->activeStreamStack--;
	} else {
		ctx->activeStream = null;
	}
	ctx->activeProc = null;
}
noFail std_Stream_Process_fail(){
	std.Stream.Process.end();
	ERR(ERR_FAIL, "stream process failed");
}
noFail std_Stream_Process_doRun(){}
noFail std_Stream_Process_pause(){}

noFail std_Stream_Process_SkipAll(){}

pntr std_Stream_Process_next(pntr* buff){
	localStreamContext* ctx = fetchLocalStreamCtx();

	if(std.Stream.Process.readData(buff, 1)
	   .readData == generic std_Stream_Process_SkipAll){
		ERR(ERR_FAIL, "failed to get next item in stream");
		return null;
	}

return *buff;
}



const struct Stream_Proc 
	Stream_Proc_Fail = {
	        .start		= generic std_Stream_Process_SkipAll,
		.cont		= generic std_Stream_Process_SkipAll,
		.doEncode	= generic std_Stream_Process_SkipAll,
		.doDecode	= generic std_Stream_Process_SkipAll,
		.each		= generic std_Stream_Process_SkipAll,
		.advance	= generic std_Stream_Process_SkipAll,
		.rewind		= generic std_Stream_Process_SkipAll,
		.readData	= generic std_Stream_Process_SkipAll,
		.writeData	= generic std_Stream_Process_SkipAll,
		.pause		= generic std_Stream_Process_SkipAll,
		.current	= generic std_Stream_Process_SkipAll,
	    	.end		= generic std_Stream_Process_SkipAll,
		.fail		= generic std_Stream_Process_SkipAll,
		.doRun		= generic std_Stream_Process_SkipAll,
	    	.next		= generic std_Stream_Process_SkipAll,
	},
	Stream_Proc_OK   = {
	        .start		= std_Stream_Process_start,
		.cont		= std_Stream_Process_cont,
		.doEncode	= std_Stream_Process_doEncode,
		.doDecode	= std_Stream_Process_doDecode,
		.each		= std_Stream_Process_each,
		.advance	= std_Stream_Process_advance,
		.rewind		= std_Stream_Process_rewind,
		.readData	= std_Stream_Process_readData,
		.writeData	= std_Stream_Process_writeData,
		.pause		= std_Stream_Process_pause,
		.current	= std_Stream_Process_current,
	    	.end		= std_Stream_Process_end,
		.fail		= std_Stream_Process_fail,
		.doRun		= std_Stream_Process_doRun,
	    	.next		= std_Stream_Process_next,
	}
;



WRITE(std_Stream){
    len_t result = 0, written = 0;

    std.Stream.Process.start(self);

    loop(i, size){
	switch(pntr_asVal(data[i])){
	case __EVAL_VALUE :{
	    std_typeData* type = data[++i];

	    if(!type){
	    	i = size; break;
	    }
	    pntr inst = data[++i];
	    written = ops(type).Print(
			inst, 
			data[++i]     ? 
			    data[i] :
			    generic type->format,
			self
	    );

	    if(!written){
	    	i = size; break;
	    }
	    
	    result += written;
	break;}
	case __SKIP_VALUE:{ break; }
	case __END_VALUE: { i = size; break; }
	default:{
	    strc8 str = data[i];
	    	std.Stream.Process.writeData(str, size(str));
	}
	}
    }

    std.Stream.Process.end();

return written;
}

READ(std_Stream){
    len_t result = 0, scanned = 0;

    std.Stream.Process.start(self);

    loop(i, size){
	switch(pntr_asVal(data[i])){
	case __EVAL_VALUE :{
	    std_typeData* type = data[++i];

	    if(!type){
	    	i = size; break;
	    }
	    pntr inst = data[++i];
	    scanned = ops(type).Scan(
			inst, 
			data[++i]     ? 
			    data[i] :
			    generic type->format,
			self
	    );

	    if(!scanned){
	    	i = size; break;
	    }
	    
	    result += scanned;
	break;}
	case __SKIP_VALUE:{ break; }
	case __END_VALUE: { i = size; break; }
	default:{
	    len_t str_len = size(data[i]);
	    strc8 str = data[i], strbuff = malloc(str_len); //optimize this later
	    	
		std.Stream.Process.readData(strbuff, str_len);

	    if(memcmp(str, strbuff, str_len)){
	    	i = size; break;
	    }

	    free(strbuff); // optimize this later
	}
	}
    }

    std.Stream.Process.end();

return scanned;
}


SET(std_Stream){

	std_Stream store_temp = this;

	if(create(std_Stream, self,  
		.ops = *(std_Stream_Options*)value
	) == nil){
		return ERR(ERR_FAIL, "failed to set stream");	
	}

	switch(store_temp.__private.flags.streamType){
	case STREAM_TYPE_REAL:{ XC.Dev.Stream.drop(store_temp.__private.stream.handle); break;} 
	case STREAM_TYPE_MEM: { del(priv.stream.mem.data); break;}
	}
		
return OK;
}
COPY(std_Stream){
	
	memcpy(where, self, sizeof(std_Stream));

	std_Stream* dest = where;

	if(priv.flags.streamType == STREAM_TYPE_MEM){
		privof(dest).stream.mem.data =	copy(
			priv.stream.mem.data,
			new_alloc(std_Array_List)
		);
	}

return where;
}

ITER(std_Stream){
	if(priv.flags.writeOnly){ 
		ERR(ERR_FAIL, "cannot iterate through a write only stream");
	  	return null;
	}

	if(priv.frameSize * index > size(self)){
		ERR(ERR_FAIL, "index out of range");
	  	return null;
	}

return pntr_shiftcpy(
	    std.Stream.GetPointer(self),
	    priv.frameSize * index
	);
}

DESTROY(std_Stream){

	switch(priv.flags.streamType){
	case STREAM_TYPE_REAL:{
		XC.Dev.Stream.drop(priv.stream.handle);
	break;}
	case STREAM_TYPE_MEM:{
		del(priv.stream.mem.data);
	break;}
	}

return OK;
}

SIZE(std_Stream){
	if(!self) return sizeof(std_Stream);

	switch(priv.flags.streamType){
	case STREAM_TYPE_REAL:{
		streamInfo info = XC.Dev.Stream.info(priv.stream.handle);

		return elements ? info.size / priv.frameSize : info.size;
	break;}
	case STREAM_TYPE_MEM:{
		return elements ? 
			elements(priv.stream.mem.data) : 
			size(priv.stream.mem.data);
	break;}
	case STREAM_TYPE_BUFF:{
		return elements ?
			priv.stream.buff.size :
			priv.stream.buff.size * priv.frameSize
		;
	break;}
	default: {
		ERR(ERR_INVALID, "invalid stream type");
		return 0;
	}
	}
}

construct(std_Stream,
FMT(),
DEF(),
	.Create  = std_Stream_Op_Create,
	.Destroy = std_Stream_Op_Destroy,
	.Size	 = std_Stream_Op_Size,
	.Iter	 = std_Stream_Op_Iter,
	.Copy	 = std_Stream_Op_Copy,
	.Write   = std_Stream_Op_Write,
	.Read	 = std_Stream_Op_Read,
	.Hash	 = nilmethod,
	.Print	 = nilmethod,
	.Set	 = nilmethod
){ 
	if(
	    (arg.ops.flags.invalid) ||
	    (arg.ops.flags.readOnly && args->ops.flags.writeOnly)
	) {
		ERR(ERR_INITFAIL, "invalid stream options");
		return nil;
	}

	if(arg.ops.handle){
		priv.flags.streamType = STREAM_TYPE_REAL;
		priv.stream.handle    = arg.ops.handle;
	} elif(arg.ops.setMemoryAddr) {

		if(!arg.ops.setMemorySize){
		    ERR(ERR_INITFAIL, "setMemorySize cannot be 0, if setMemoryAddr is set");
		    return nil;
		}

		priv.flags.streamType = STREAM_TYPE_BUFF;

		priv.stream.buff.data = arg.ops.setMemoryAddr;
		priv.stream.buff.size = arg.ops.setMemorySize;

	} else {
	    priv.flags.streamType = STREAM_TYPE_MEM;
	    priv.stream.mem.data = newArrayList(u8, 
					 arg.ops.init.len ? 
					 arg.ops.init.len : 50);

	    if(priv.stream.mem.data == nil){
		ERR(ERR_INITFAIL, "failed to initalize internal mem stream");
		return nil;
	    }
	}

return self;
}

COPY(std_Stream_Options){ 
	if(!memcpy(where, self, sizeof(std_Stream_Options))){
		ERR(ERR_FAIL, "failed to copy stream options");
		return nil;
	}

return where;}

SET(std_Stream_Options){  
	if(!memcpy(self, value, sizeof(std_Stream_Options))) 
		return ERR(ERR_FAIL, "failed to set stream options");
return OK;}

SIZE(std_Stream_Options){ return sizeof(std_Stream_Options); }
DESTROY(std_Stream_Options){ return OK; }
PRINT(std_Stream_Options){

	return write(out,
		"(Stream_Options){ "
	      	".flags = { ", 
	      		this.flags.readOnly   ? "readOnly "   : fmt_skip,
	      		this.flags.writeOnly  ? "writeOnly "  : fmt_skip,
	      		this.flags.invalid    ? "invalid "    : fmt_skip,
	      	"}, ",
	      	".handle = ",        $use(pntr_Type, this.handle),    ", ",
	      	".initData = ",      $(this.init.data), 	", ",
	      	".initLen = ",       $(this.init.len), 	", ",
	      	" }",
	fmt_end);
}


construct(std_Stream_Options,
FMT(),
DEF(),
	.Create  = std_Stream_Options_Op_Create,
	.Destroy = std_Stream_Options_Op_Destroy,
	.Copy    = std_Stream_Options_Op_Copy,
	.Set     = std_Stream_Options_Op_Set,
	.Size    = std_Stream_Options_Op_Size,
	.Print   = std_Stream_Options_Op_Print,
){ passover }


