/* Registers */
#define gpr_a "%rax"
#define gpr_b "%rbx"
#define gpr_c "%rcx"
#define gpr_d "%rdx"

#define r0  "%rax"
#define r1  "%rdi"
#define r2  "%rsi"
#define r3  "%rdx"
#define r4  "%rcx"
#define r5  "%r8"
#define r6  "%r9"
#define r7  "%r10"
#define r8  "%rbx"
#define r9  "%r12"
#define r10 "%r13"
#define r11 "%r14"
#define r12 "%r15"
#define r13 "%r11"
#define rbp "%rbp"
#define rsp "%rsp"
#define rip "%rip"

/* Memory & Addressing */
#define mem_dref(base)      "(" base ")"
#define mem_off(base, off)  #off "(" base ")"
#define mem_idx(b, i, s)    "(" b ", " i ", " #s ")"
#define ref(name)           "$" #name
#define lit(val) 	    "$" #val

/* Data Movement */
#define mov(src, dst)       "\tmovq " src ", " dst "\n"
#define mov_imm(imm, dst)   "\tmovq $" #imm ", " dst "\n"
#define load(addr, dst)     "\tmovq " addr ", " dst "\n"
#define store(src, addr)    "\tmovq " src ", " addr "\n"
#define lea(addr, dst)      "\tleaq " addr ", " dst "\n"

/* Arithmetic (3-Operand Destructive) */
#define add(s1, s2, d)      "\tmovq " s1 ", " d "\n\taddq " s2 ", " d "\n"
#define sub(s1, s2, d)      "\tmovq " s1 ", " d "\n\tsubq " s2 ", " d "\n"
#define mul(s1, s2, d)      "\tmovq " s1 ", " d "\n\timulq " s2 ", " d "\n"
#define div(s1, s2, d)      "\tmovq " s1 ", %%rax\n\txorq %%rdx, %%rdx\n\tdivq " s2 "\n\tmovq %%rax, " d "\n"
#define mds(s1, s2, d)      "\tmovq " s1 ", %%rax\n\txorq %%rdx, %%rdx\n\tdivq " s2 "\n\tmovq %%rdx, " d "\n"
#define and(s1, s2, d)      "\tmovq " s1 ", " d "\n\tandq " s2 ", " d "\n"
#define or(s1, s2, d)       "\tmovq " s1 ", " d "\n\torq " s2 ", " d "\n"
#define xor(s1, s2, d)      "\tmovq " s1 ", " d "\n\txorq " s2 ", " d "\n"
#define not(s, d)           "\tmovq " s ", " d "\n\tnotq " d "\n"
#define shl(s, amt, d)      "\tmovq " s ", " d "\n\tshlq $" #amt ", " d "\n"
#define shr(s, amt, d)      "\tmovq " s ", " d "\n\tshrq $" #amt ", " d "\n"
#define sar(s, amt, d)      "\tmovq " s ", " d "\n\tsarq $" #amt ", " d "\n"

/* Control Flow */
#define lbl(name)           #name ":\n"
#define jmp(label)          "\tjmp " #label "\n"
#define call(label)         "\tcall " #label "\n"
#define ret()               "\tret\n"
#define cmp(r1, r2)         "\tcmpq " r1 ", " r2 "\n"
#define je(label)           "\tje " #label "\n"
#define jne(label)          "\tjne " #label "\n"
#define jl(label)           "\tjl " #label "\n"
#define jg(label)           "\tjg " #label "\n"
#define bz(reg, label)      "\ttestq " reg ", " reg "\n\tjz " #label "\n"
#define bnz(reg, label)     "\ttestq " reg ", " reg "\n\tjnz " #label "\n"

/* Stack */
#define stk_push(reg)       "\tpushq " reg "\n"
#define stk_pop(reg)        "\tpopq " reg "\n"
#define stk_push_pair(a, b) "\tpushq " a "\n\tpushq " b "\n"
#define stk_pop_pair(a, b)  "\tpopq " b "\n\tpopq " a "\n"

/* System */
#define syscall()           "\tsyscall\n"
#define pause()             "\tpause\n"
#define halt()              "\thlt\n"
#define nop()               "\tnop\n"
#define fence()             "\tmfence\n"
#define atomic_add(p, v)    "\tlock addq " v ", " p "\n"
#define cmpxchg(p, e, n)    "\tlock cmpxchgq " n ", " p "\n"
#define cpu_fpu_setup(addr) "\tfldcw " mem_dref(addr) "\n"

