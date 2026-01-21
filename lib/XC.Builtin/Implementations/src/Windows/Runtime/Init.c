#include <XC.pkg.c>
#include <XC.Builtin/Assembly.pkg.h>

#define module env, Windows, Runtime

alias(std.String.Utils.Str, str);
alias(std.String.Encoding,  encoding);

/* --- SECTION 1: PE/COFF CONTRACT SYMBOLS --- */
int _fltused = 0x9875;
u64 _tls_index = 0;

extern byte __tls_start__;
extern byte __tls_end__;

[[gnu::section(".rdata$T")]]
const IMAGE_TLS_DIRECTORY64 _tls_used = {
    (ULONGLONG)&__tls_start__, (ULONGLONG)&__tls_end__,
    (ULONGLONG)&_tls_index,    (ULONGLONG)0, 0, 0
};

/* --- SECTION 2: BRACKETED CONSTRUCTORS --- */
[[gnu::section(".CRT$XCA")]] void (*__xc_init_start[0])();
[[gnu::section(".CRT$XCZ")]] void (*__xc_init_end[0])();

static void moduleFn(runConstructors)() {
    void (**ptr)() = __xc_init_start;
    void (**end)() = __xc_init_end;

    while (ptr < end) {
        if (ptr[0]) ptr[0]();
        pntr_shift(ptr, sizeof(*ptr));
    }

}

/* --- SECTION 3: ENTRY POINT --- */

void moduleFn(init)(){
	int argCount;
	
	iferr(env.Windows.Runtime.Device.init())
		printlnErr("Failed to initialize runtime device system:", $(std.Error.Get()));
	

	errvt exitcode = 
		__MAIN_APP.start(args());
	
	if(exitcode != OK)
		printlnErr("Process Exited with error:", $(std.Error.Get()));
	else 
		printlnErr("Process Exited OK");
	

	ExitProcess(exitcode);
}


static void moduleFn(start_Stage2)() {
    u16 fpu_cw = 0x037F;
    
    asm(
        cpu_fpu_setup(narg(0))
        input_ref(fpu_cw)
    );

    mod(runConstructors)();
    mod(init)();
    
    ExitProcess(-1);
}

asmFn _start() {
    asm(
        xor(rbp, rbp, rbp)            
        mov(rsp, r1)                  
        and(r1, lit(-16), r1)
        sub(lit(40), r1, rsp)   
        call(env_Windows_Runtime_start_Stage2)  
        halt()                        
    );
}


