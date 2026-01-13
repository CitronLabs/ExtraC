/* Registers */
#define gpr_a "%eax"
#define gpr_b "%ebx"
#define gpr_c "%ecx"
#define gpr_d "%edx"

#define r0  "%eax"
#define r1  "%ecx"
#define r2  "%edx"
#define r3  "%ebx"
#define rbp "%ebp"
#define rsp "%esp"
#define rip "%eip"

/* Memory & Addressing */
#define mem_dref(base)      "(" base ")"
#define mem_off(base, off)  #off "(" base ")"
#define mem_idx(b, i, s)    "(" b ", " i ", " #s ")"
#define ref(name)           "$" #name
#define lit(val) 	    "$" #val

/* Data Movement */
#define mov(src, dst)       "\tmovl " src ", " dst "\n"
#define mov_imm(imm, dst)   "\tmovl $" #imm ", " dst "\n"
#define load(addr, dst)     "\tmovl " addr ", " dst "\n"
#define store(src, addr)    "\tmovl " src ", " addr "\n"
#define lea(addr, dst)      "\tleal " addr ", " dst "\n"

/* Arithmetic */
#define add(s1, s2, d)      "\tmovl " s1 ", " d "\n\taddl " s2 ", " d "\n"
#define sub(s1, s2, d)      "\tmovl " s1 ", " d "\n\tsubl " s2 ", " d "\n"
#define mul(s1, s2, d)      "\tmovl " s1 ", " d "\n\timull " s2 ", " d "\n"
#define div(s1, s2, d)      "\tmovl " s1 ", %%eax\n\txorl %%edx, %%edx\n\tdivl " s2 "\n\tmovl %%eax, " d "\n"
#define mds(s1, s2, d)      "\tmovl " s1 ", %%eax\n\txorl %%edx, %%edx\n\tdivl " s2 "\n\tmovl %%edx, " d "\n"
#define and(s1, s2, d)      "\tmovl " s1 ", " d "\n\tandl " s2 ", " d "\n"
#define or(s1, s2, d)       "\tmovl " s1 ", " d "\n\torl " s2 ", " d "\n"
#define xor(s1, s2, d)      "\tmovl " s1 ", " d "\n\txorl " s2 ", " d "\n"
#define not(s, d)           "\tmovl " s ", " d "\n\tnotl " d "\n"
#define shl(s, amt, d)      "\tmovl " s ", " d "\n\tshll $" #amt ", " d "\n"
#define shr(s, amt, d)      "\tmovl " s ", " d "\n\tshrl $" #amt ", " d "\n"
#define sar(s, amt, d)      "\tmovl " s ", " d "\n\tsarl $" #amt ", " d "\n"

/* Control Flow */
#define lbl(name)           #name ":\n"
#define jmp(label)          "\tjmp " #label "\n"
#define call(label)         "\tcall " #label "\n"
#define ret()               "\tret\n"
#define cmp(r1, r2)         "\tcmpl " r1 ", " r2 "\n"
#define je(label)           "\tje " #label "\n"
#define jne(label)          "\tjne " #label "\n"
#define bz(reg, label)      "\ttestl " reg ", " reg "\n\tjz " #label "\n"
#define bnz(reg, label)     "\ttestl " reg ", " reg "\n\tjnz " #label "\n"

/* Stack */
#define stk_push(reg)       "\tpushl " reg "\n"
#define stk_pop(reg)        "\tpopl " reg "\n"
#define stk_push_pair(a, b) "\tpushl " a "\n\tpushl " b "\n"
#define stk_pop_pair(a, b)  "\tpopl " b "\n\tpopl " a "\n"

/* System */
#define syscall()           "\tint $0x80\n"
#define pause()             "\tpause\n"
#define halt()              "\thlt\n"
#define nop()               "\tnop\n"
#define fence()             "\tmfence\n"
#define atomic_add(p, v)    "\tlock addl " v ", " p "\n"
#define cmpxchg(p, e, n)    "\tlock cmpxchgl " n ", " p "\n"
#define cpu_fpu_setup(addr) "\tfldcw " mem_dref(addr) "\n"
