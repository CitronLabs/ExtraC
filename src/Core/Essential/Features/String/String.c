#include <XC.Core/pkg.c>
#define module std, String

alias(std.Stream.Process, then);

bool moduleMethod(std_String, Compare, std_String* cmp_string){
	
	if(len(self) != len(cmp_string)	|| 
	   priv.len_bytes != privof(cmp_string).len_bytes
	) 
		return false;

return strncmp(self, cmp_string, this.len);
}

errvt moduleMethod(std_String, Copy, std_String* to){
	return to == copy(self, to) ? OK : ERR(ERR.FAIL, "failed to copy");
}

errvt moduleMethod(std_String, Cat,  Array(std_String*) strings){

	std_Stream* temp_stream =
		push(std_Stream,
			.ops.init = {this.data, priv.len_bytes}
	);

	foreach(&strings, std_String*, string){
	    
	    if(!(*string)->len) continue;

	    if(!printTo(temp_stream, $(*string))){
	    	pop(temp_stream);
	    	return ERR(ERR.FAIL, "failed to concatinate an input string");
	    }
	}

	if(!scanFrom(temp_stream, $(self))){
	    pop(temp_stream);
	    return ERR(ERR.FAIL, "failed to scan concatinated string");
	}

	pop(temp_stream);

return OK;
}


std_String* moduleMethod(std_String, View, len_t from, len_t to){
	
	if(to < from){
		ERR(ERR.INVALID, "to cannot be less than when making a string view");
		return nil;
	}
		
	if(to > this.len) to = this.len;

	if(!priv.views)
		priv.views = newArrayList(std_String, 5);

	len_t index = len(priv.views);
	
	write(priv.views, &(std_String){0});

	
return create(std_String, index(priv.views, index),
	.data    = pntr_shiftcpy(this.data, from),
	.max_len = to - from,
	.view 	 = true
);
}
errvt moduleMethod(std_String, ViewShift, len_t up, len_t down){
	
	if(!priv.IsView)
		return ERR(ERR.INVALID, "only string views are able to be shifted");
	
	if(down + up > this.len) 
		return ERR(ERR.INVALID, "string view shift down and up collision detected");

	

	len_t bytes_to_shift = 0;

	if(down){
		c8 
		   * new_end = index(self, this.len - down),
		   * old_end = pntr_shiftcpy(this.data, priv.len_bytes);
		;

		bytes_to_shift += pntr_asVal(old_end) - pntr_asVal(new_end);
	}

	if(up){
		len_t up_shift = strsize(this.data, up * sizeof(rune));
	
		pntr_shift(this.data, up_shift);

		bytes_to_shift += up_shift;
	}

	priv.len_bytes -= bytes_to_shift;

return OK;
}	
bool moduleMethod(std_String, IsView){ return priv.IsView; }

errvt moduleMethod(std_String, StreamTo, std_Stream* stream){
	std.String.Encoding.UTF8.Encoder(stream, self);
return OK;
}

DESTROY(std_String){
	nonull(self->data){ return err; }
	
	if(!priv.IsView)
		free(self->data);

	if(priv.views) del(priv.views);

return OK;
};


PRINT(std_String){
	try(){
	    std.Stream.Process
		.start(out)
		.doEncode(std.String.Encoding.UTF8.Encoder, self)
		.end();
	} catch { return 0; }

return priv.len_bytes;
}

SCAN(std_String){
	nonull(self){ return 0; }

	rune c = 0;

	len_t 
	  prev_pos 	= std.Stream.GetCursorPos(in), 
	  string_size 	= 0, 
	  string_len 	= 0
	;

	std.Stream.Process
	    .start(in)

	    .doDecode(std.String.Encoding.UTF8.Decoder, c){ string_len++; } 

	    then.rewind((string_size = std.Stream.GetCursorPos(in) - prev_pos))

	    .run(({
		if(this.data && !priv.IsView){ 
			destroy(self); 
		}

		this.data 	= malloc(string_size);
		this.len 	= string_len;
		priv.len_bytes 	= string_size;
		priv.IsView 	= false;
		priv.views 	= nil;

		run_continue;
	    }))
	    .end();
	;

return string_size;
}

ITER(std_String){
	
	c8* result = this.data;

	loop(i, index){	std.String.Encoding.UTF8.decode(&result, nil); }

return result;
}

SIZE(std_String){ return elements ? this.len : priv.len_bytes; }

COPY(std_String){

	std_String* dest = where;
	
	memcpy(dest, self, sizeof(std_String));

	priv.views = nil;
	dest->data = malloc(priv.len_bytes); 

	if(!dest->data){
		ERR(ERR.FAIL, "failed to allocate new string");
		return nil;
	}

	memcpy(dest->data, this.data, priv.len_bytes);

return where;

}

HASH(std_String){ return hash_bytes(self->data, priv.len_bytes); }

construct(std_String,
FMT(),
DEF(),
		  
){
	void* end = nil;

	this.len = std.String.Utils.Str.length(V(arg.data), arg.max_len, &end);

	priv.len_bytes = (pntr_asVal(end) - pntr_asVal(arg.data));

	if(arg.view){
		this.data = arg.data;

		priv.IsView = true;
	} else {
		this.data = malloc(priv.len_bytes);

		memcpy(this.data, arg.data, priv.len_bytes);
	  	
		this.data[priv.len_bytes] = '\0';
	}
		
return self;
}



