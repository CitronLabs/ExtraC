#pragma once
#include "../utils.h"

#define gpr_a 	__UNDEFINED__
#define gpr_b 	__UNDEFINED__
#define gpr_c 	__UNDEFINED__
#define gpr_d 	__UNDEFINED__

/* --- 2. Virtual Register Map (GPR) --- */
/* Mapping 16 standard GPRs to preserve ABI compatibility across platforms */
#define r0  __UNDEFINED__ /* Return Value / Arg 0 */
#define r1  __UNDEFINED__ /* Arg 1 */
#define r2  __UNDEFINED__ /* Arg 2 */
#define r3  __UNDEFINED__ /* Arg 3 */
#define r4  __UNDEFINED__ /* Arg 4 */
#define r5  __UNDEFINED__ /* Arg 5 */
#define r6  __UNDEFINED__ /* Scratch / Temp */
#define r7  __UNDEFINED__ /* Scratch / Temp */
#define r8  __UNDEFINED__ /* Callee-Saved 0 */
#define r9  __UNDEFINED__ /* Callee-Saved 1 */
#define r10 __UNDEFINED__ /* Callee-Saved 2 */
#define r11 __UNDEFINED__ /* Callee-Saved 3 */
#define r12 __UNDEFINED__ /* Intra-procedure-call scratch */
#define r13 __UNDEFINED__ /* Platform Reserved (Thread Pointer) */
#define rbp __UNDEFINED__ /* Frame Pointer */
#define rsp __UNDEFINED__ /* Stack Pointer */
#define rip __UNDEFINED__ /* Instruction Pointer (PC) */

/* --- 3. Memory & Addressing Engine --- */
/* Abstracting displacement, indexing, and scaling */
#define mem_dref(base)      __UNDEFINED__
#define mem_off(base, off)  __UNDEFINED__
#define mem_idx(b, i, s)    __UNDEFINED__
#define ref_lbl(name)       __UNDEFINED__  /* Reference to a symbol */
#define lit(val) 	    __UNDEFINED__  /* Wraps a literal number for the assembler */

/* --- 4. Core ISA: Data Movement --- */
#define mov(src, dst)       __UNDEFINED__ /* General move */
#define mov_imm(imm, dst)   __UNDEFINED__ /* Load immediate */
#define load(addr, dst)     __UNDEFINED__ /* Load from memory */
#define store(src, addr)    __UNDEFINED__ /* Store to memory */
#define lea(addr, dst)      __UNDEFINED__ /* Load Effective Address */

/* --- 5. Core ISA: Integer Arithmetic (3-Operand) --- */
#define add(s1, s2, d)  __UNDEFINED__    /* d = s1 + s2 */
#define sub(s1, s2, d)  __UNDEFINED__    /* d = s1 - s2 */
#define mul(s1, s2, d)  __UNDEFINED__    /* d = s1 * s2 */
#define div(s1, s2, d)  __UNDEFINED__    /* d = s1 / s2 */
#define mds(s1, s2, d)  __UNDEFINED__    /* d = s1 % s2 */
#define and(s1, s2, d)  __UNDEFINED__    /* d = s1 & s2 */
#define or(s1, s2, d)   __UNDEFINED__    /* d = s1 | s2 */
#define xor(s1, s2, d)  __UNDEFINED__    /* d = s1 ^ s2 */
#define not(s, d)       __UNDEFINED__    /* d = ~s */
#define shl(s, amt, d)  __UNDEFINED__    /* d = s << amt */
#define shr(s, amt, d)  __UNDEFINED__    /* d = s >> amt (Logical) */
#define sar(s, amt, d)  __UNDEFINED__    /* d = s >> amt (Arithmetic) */

/* --- 6. Core ISA: Control Flow --- */
#define lbl(name)	__UNDEFINED__    /* Label */
#define jmp(label)      __UNDEFINED__    /* Unconditional jump */
#define call(label)     __UNDEFINED__    /* Function call */
#define ret()           __UNDEFINED__    /* Return from call */
#define cmp(r1, r2)     __UNDEFINED__    /* Compare r1 and r2 */
#define je(label)       __UNDEFINED__    /* Jump if equal */
#define jne(label)      __UNDEFINED__    /* Jump if not equal */
#define jl(label)       __UNDEFINED__    /* Jump if less (signed) */
#define jg(label)       __UNDEFINED__    /* Jump if greater (signed) */
#define bz(reg, label)  __UNDEFINED__    /* Branch if reg == zero */
#define bnz(reg, label) __UNDEFINED__    /* Branch if reg != zero */

/* --- 7. Core ISA: Stack Operations --- */
/* Note: On 64-bit platforms, these must maintain 16-byte alignment */
#define stk_push(reg)        	__UNDEFINED__   
#define stk_pop(reg)            __UNDEFINED__
#define stk_push_pair(r1, r2)   __UNDEFINED__	/* Optimized for ARM/RISC-V */
#define stk_pop_pair(r1, r2)	__UNDEFINED__


/* --- 9. System & Atomic Operations --- */
#define syscall()               __UNDEFINED__	/* Execute system call */
#define pause()                 __UNDEFINED__	/* Spin-lock hint */
#define halt()                  __UNDEFINED__	/* Halt CPU */
#define nop()                   __UNDEFINED__	/* No operation */
#define fence()                 __UNDEFINED__	/* Memory barrier */
#define atomic_add(ptr, val)    __UNDEFINED__	/* Atomic fetch-and-add */
#define cmpxchg(ptr, exp, new)  __UNDEFINED__	/* Atomic compare-and-swap */
#define cpu_fpu_setup(addr)     __UNDEFINED__   /* Abstracted hardware state initialization */
