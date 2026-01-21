#include <XC.pkg.c>

#define module std, Stream

from(std,
	Array_Stack    as Stack,
	Array_List     as List,
    	Stream_Options as Options,
    	use(Stream),
    	use(Process)
);

typedef struct {
	std_StreamDecoder 	activeDecoder;
} StreamData;

typedef const struct std_Stream_Process_Proc SP_Result;
extern SP_Result Stream_Proc_Fail, Stream_Proc_OK;

#define STREAM_TYPE_REAL 1
#define STREAM_TYPE_MEM  2
#define STREAM_TYPE_BUFF 3


Process* moduleFn(getProcess)(){
	thread_local static struct {
		Process* process;
		StreamData data;
	} Context = {};
	
	if(!Context.process){
		Context.process = new(Process, T(Stream), &Context.data);

		if(!Context.process){
			ERR(ERR.INIT, "Failed to initialize stream process context");
	  		return nil;
		}
	}

return Context.process;
}


Stream* fetchStdStream(int id, Stream** stream){
	if(!*stream){
	    *stream = new(Stream,
		std.Stream.Preset.fromHandle(
		    core.Device.Stream.stdHandle(id)
		)
	    );

	    if(*stream == nil){
		ERR(ERR.INIT, "failed to initalize stream");
		return nil;
	    }
	}

return *stream;
}

Stream* std_Stream_stdOut(){ static Stream* stream = nil; return fetchStdStream(core.Device.Stream.ID.Out, &stream); }
Stream* std_Stream_stdIn(){  static Stream* stream = nil; return fetchStdStream(core.Device.Stream.ID.In,  &stream); }
Stream* std_Stream_stdErr(){ static Stream* stream = nil; return fetchStdStream(core.Device.Stream.ID.Err, &stream); }


const Options std_Stream_Preset_staticBuffer(void* start, len_t len){
	return (Options){
		.setMemoryAddr = start,
		.setMemorySize = len
	};
}

const Options std_Stream_Preset_fromHandle(streamHandle handle){
	return (Options){
		.handle = handle
	};
}

streamHandle moduleMethod(std_Stream, getHandle){
	nonull(self){ return nil; }

return priv.stream.handle;
}

errvt moduleMethod(std_Stream, Flush){
	nonull(self){ return err; }

return priv.stream.handle ? core.Device.Stream.Modify.flush(priv.stream.handle) : OK;
}	
void* moduleMethod(std_Stream, ToPointer){
	nonull(self){ return nil; }

	switch(priv.flags.streamType){
	case STREAM_TYPE_REAL:{
		streamInfo info = core.Device.Stream.Modify.info(priv.stream.handle);

		iferr(!info.valid){
			ERR(ERR.FAIL, "failed to get stream size");
			return nil;
		}

		priv.pointer = malloc(info.size);
		
		if(!priv.pointer){
			ERR(ERR.FAIL, "failed to allocate stream pointer buff");
			return nil;
		}

		if(!core.Device.Stream.Modify.readFrom(priv.stream.handle, priv.pointer, info.size)){
			ERR(ERR.FAIL, "failed to read data into stream pointer buff");
			free(priv.pointer);
			return nil;
	  	}

	break;}
	case STREAM_TYPE_MEM: {
		priv.pointer = malloc(size(priv.stream.mem.data) - priv.stream.mem.pos);

		if(!priv.pointer){
			ERR(ERR.FAIL, "failed to allocate stream pointer buff");
			return nil;
		}
	break;}
	case STREAM_TYPE_BUFF: {
		priv.pointer = priv.stream.buff.data;
	break;}
	default:{
		ERR(ERR.INVALID, "invalid stream type");
	  	return nil;
	}
	}

return priv.pointer;
}
void* moduleMethod(std_Stream, GetPointer){ return priv.pointer ? priv.pointer : std.Stream.ToPointer(self); }


SP_Result moduleFn(Process_start)(std_Stream* strm){
	Process* ctx = mod(getProcess)();

	nonull(strm)
		return Stream_Proc_Fail;
	
return Stream_Proc_OK;
}
SP_Result moduleFn(Process_doEncode)(std_StreamEncoder encoder, void* data){
	Process* ctx = mod(getProcess)();

	nonull(encoder, data){ return Stream_Proc_Fail; }

	iferr(encoder(std.Process.result(ctx), data)){
		ERR(ERR.FAIL, "failed to encode data into stream");
		return Stream_Proc_Fail;
	}

return Stream_Proc_OK;
}

SP_Result moduleFn(Process_doDecode)(std_StreamDecoder decoder){
	Process* ctx = mod(getProcess)();

	nonull(decoder){ return Stream_Proc_Fail; }

	((StreamData*)ctx->extraData)->activeDecoder = decoder;

return Stream_Proc_OK;
}

SP_Result moduleFn(Process_advance)(len_t num){
	Process* ctx = mod(getProcess)();
	
	Stream* self = std.Process.result(ctx);
	
	switch(priv.flags.streamType){
	case STREAM_TYPE_REAL:{
		streamInfo info = core.Device.Stream.Modify.info(priv.stream.handle);
		
		if(!info.valid){
			ERR(ERR.FAIL, "failed to get stream cursor position to advance");
			return Stream_Proc_Fail;
		}

		iferr(core.Device.Stream.Modify.shift(priv.stream.handle, num, info.currentPos)){
			ERR(ERR.FAIL, "failed to shift stream cursor position to advance");
			return Stream_Proc_Fail;
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
		ERR(ERR.INVALID, "invalid stream type");
	  	return Stream_Proc_Fail;
	}
	}

return Stream_Proc_OK;
}
SP_Result moduleFn(Process_rewind)(len_t num){
	Process* ctx = mod(getProcess)();

	Stream* self = std.Process.result(ctx);

	switch(priv.flags.streamType){
	case STREAM_TYPE_REAL:{
		streamInfo info = core.Device.Stream.Modify.info(priv.stream.handle);
		
		if(!info.valid){
			ERR(ERR.FAIL, "failed to get stream cursor position to advance");
			return Stream_Proc_Fail;
		}

		iferr(core.Device.Stream.Modify.shift(priv.stream.handle, -num, info.currentPos)){
			ERR(ERR.FAIL, "failed to shift stream cursor position to advance");
			return Stream_Proc_Fail;
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
		ERR(ERR.INVALID, "invalid stream type");
	  	return Stream_Proc_Fail;
	}
	}
	

return Stream_Proc_OK;
}


SP_Result moduleFn(Process_readData)(void* buff, len_t len){
	Process* ctx = mod(getProcess)();

	Stream* self = std.Process.result(ctx);

	switch(priv.flags.streamType){
	case STREAM_TYPE_REAL:{
		if(!core.Device.Stream.Modify.readFrom(priv.stream.handle, buff, len * priv.frameSize)){
			ERR(ERR.FAIL, "failed to read from stream");
			return Stream_Proc_Fail;
		}
	break;}
	case STREAM_TYPE_MEM:{
		if(!memcpy(
		    buff,
		    pntr_shiftcpy(priv.stream.mem.data, len * priv.frameSize),
	  	    len * priv.frameSize
		)){
			ERR(ERR.FAIL, "failed to read from stream");
			return Stream_Proc_Fail;
		}
	break;}
	case STREAM_TYPE_BUFF:{
	    	if(len > priv.stream.buff.size - priv.stream.buff.pos) 
		    len = priv.stream.buff.pos;

		memcpy(buff, priv.stream.buff.data, priv.frameSize * len);

		priv.stream.buff.pos += len;
	break;}
	default:{
		ERR(ERR.INVALID, "invalid stream type");
	  	return Stream_Proc_Fail;
	}
	}
	
return Stream_Proc_OK;
}
SP_Result moduleFn(Process_writeData)(void* buff, len_t len){
	Process* ctx = mod(getProcess)();

	Stream* self = std.Process.result(ctx);

	switch(priv.flags.streamType){
	case STREAM_TYPE_REAL:{
		if(!core.Device.Stream.Modify.writeTo(priv.stream.handle, buff, len * priv.frameSize)){
			ERR(ERR.FAIL, "failed to write from stream");
			return Stream_Proc_Fail;
		}
	break;}
	case STREAM_TYPE_MEM:{
		if(!memcpy(
		    buff,
		    pntr_shiftcpy(priv.stream.mem.data, len * priv.frameSize),
	  	    len * priv.frameSize
		)){
			ERR(ERR.FAIL, "failed to write from stream");
			return Stream_Proc_Fail;
		}
	break;}
	case STREAM_TYPE_BUFF:{
	    	if(len > priv.stream.buff.size - priv.stream.buff.pos) 
		    len = priv.stream.buff.pos;

		memcpy(priv.stream.buff.data, buff, priv.frameSize * len);

		priv.stream.buff.pos += len;
	break;}
	default:{
		ERR(ERR.INVALID, "invalid stream type");
	  	return Stream_Proc_Fail;
	}
	}
	
return Stream_Proc_OK;

}

std_Stream* moduleFn(Process_result)(){ return std.Process.result(mod(getProcess)()); }

noFail moduleFn(Process_end)(){ std.Process.end(mod(getProcess)()); }

noFail moduleFn(Process_fail)(){
	std.Process.end(mod(getProcess)());
	ERR(ERR.FAIL, "stream process failed");
}

SP_Result moduleFn(Process_each)(){ return Stream_Proc_OK; }

noFail moduleFn(Process_doRun)(){}

pntr moduleFn(Process_next)(pntr* buff){
	Process* ctx = mod(getProcess)();

	if(std.Stream.Process.readData(buff, 1)
	   .readData == generic std.Process.noOp){
		ERR(ERR.FAIL, "failed to get next item in stream");
		return nil;
	}

return *buff;
}



SP_Result 
	Stream_Proc_Fail = {
	        .start		= generic std.Process.noOp,
		.doEncode	= generic std.Process.noOp,
		.doDecode	= generic std.Process.noOp,
		.each		= generic std.Process.noOp,
		.advance	= generic std.Process.noOp,
		.rewind		= generic std.Process.noOp,
		.readData	= generic std.Process.noOp,
		.writeData	= generic std.Process.noOp,
		.result		= generic std.Process.noOp,
	    	.end		= generic std.Process.noOp,
		.fail		= generic std.Process.noOp,
		.doRun		= generic std.Process.noOp,
	    	.next		= generic std.Process.noOp,
	},
	Stream_Proc_OK   = {
	        .start		= mod(Process_start),
		.doEncode	= mod(Process_doEncode),
		.doDecode	= mod(Process_doDecode),
		.each		= mod(Process_each),
		.advance	= mod(Process_advance),
		.rewind		= mod(Process_rewind),
		.readData	= mod(Process_readData),
		.writeData	= mod(Process_writeData),
		.result		= mod(Process_result),
	    	.end		= mod(Process_end),
		.fail		= mod(Process_fail),
		.doRun		= mod(Process_doRun),
	    	.next		= mod(Process_next),
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
		.ops = *(Options*)value
	) == nil){
		return ERR(ERR.FAIL, "failed to set stream");	
	}

	switch(store_temp.__private.flags.streamType){
	case STREAM_TYPE_REAL:{ core.Device.Stream.close(store_temp.__private.stream.handle); break;} 
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
		ERR(ERR.FAIL, "cannot iterate through a write only stream");
	  	return nil;
	}

	if(priv.frameSize * index > size(self)){
		ERR(ERR.FAIL, "index out of range");
	  	return nil;
	}

return pntr_shiftcpy(
	    std.Stream.GetPointer(self),
	    priv.frameSize * index
	);
}

DESTROY(std_Stream){

	switch(priv.flags.streamType){
	case STREAM_TYPE_REAL:{
		core.Device.Stream.close(priv.stream.handle);
	break;}
	case STREAM_TYPE_MEM:{
		del(priv.stream.mem.data);
	break;}
	}

return OK;
}

SIZE(std_Stream){
	nonull(self){ return 0; }

	switch(priv.flags.streamType){
	case STREAM_TYPE_REAL:{
		streamInfo info = core.Device.Stream.Modify.info(priv.stream.handle);

		return elements ? info.size / priv.frameSize : info.size;
	break;}
	case STREAM_TYPE_MEM:{
		return elements ? 
			len(priv.stream.mem.data) : 
			size(priv.stream.mem.data);
	break;}
	case STREAM_TYPE_BUFF:{
		return elements ?
			priv.stream.buff.size :
			priv.stream.buff.size * priv.frameSize
		;
	break;}
	default: {
		ERR(ERR.INVALID, "invalid stream type");
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
	.Hash	 = nil,
	.Print	 = nil,
	.Set	 = nil
){ 
	if(
	    (arg.ops.flags.invalid) ||
	    (arg.ops.flags.readOnly && args->ops.flags.writeOnly)
	) {
		ERR(ERR.INIT, "invalid stream options");
		return nil;
	}

	if(arg.ops.handle){
		priv.flags.streamType = STREAM_TYPE_REAL;
		priv.stream.handle    = arg.ops.handle;
	} elif(arg.ops.setMemoryAddr) {

		if(!arg.ops.setMemorySize){
		    ERR(ERR.INIT, "setMemorySize cannot be 0, if setMemoryAddr is set");
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
		ERR(ERR.INIT, "failed to initalize internal mem stream");
		return nil;
	    }
	}

return self;
}

#undef module
#define module std, Stream_Options


COPY(Options){ 
	if(!memcpy(where, self, sizeof(Options))){
		ERR(ERR.FAIL, "failed to copy stream options");
		return nil;
	}

return where;}

SET(Options){  
	if(!memcpy(self, value, sizeof(Options))) 
		return ERR(ERR.FAIL, "failed to set stream options");
return OK;}

SIZE(Options){ return sizeof(Options); }
DESTROY(Options){ return OK; }
PRINT(Options){

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


