#include "../pkg.h"
#include "../../Error/pkg.h"

void* UTF8Decoder(inst(Stream) strm);

void test(){

	

	char buff[10];

	try(){
	    Stream.Process.start(new(Stream))
		.doDecode(UTF8Decoder, rune){
			
	
			
	
		}
		then.wait(10){
		    
		}
		then.read(buff, 10).run(1){
			
	
		}
	    then.end();
		
	    inst(Stream) strm = Stream.Process.result();
	} 
	catch {
		return;
	}
}
