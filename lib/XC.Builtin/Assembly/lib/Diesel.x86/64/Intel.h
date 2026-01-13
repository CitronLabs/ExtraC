/* Registers */
#define gpr_a "rax"
#define gpr_b "rbx"
#define gpr_c "rcx"
#define gpr_d "rdx"

#define r0  "rax"
#define r1  "rdi"
#define r2  "rsi"
#define r3  "rdx"
#define r4  "rcx"
#define r5  "r8"
#define r6  "r9"
#define r7  "r10"
#define r8  "rbx"
#define r9  "r12"
#define r10 "r13"
#define r11 "r14"
#define r12 "r15"
#define r13 "r11"
#define rbp "rbp"
#define rsp "rsp"
#define rip "rip"

/* Memory & Addressing */
#define mem_dref(base)      "[" base "]"
#define mem_off(base, off)  "[" base " + " #off "]"
#define mem_idx(b, i, s)    "[" b " + " i " * " #s "]"
#define ref(name)           "OFFSET " #name
#define lit(val) 	    #val

/* Data Movement */
#define mov(src, dst)       "\tmov " dst ", " src "\n"
#define mov_imm(imm, dst)   "\tmov " dst ", " #imm "\n"
#define load(addr, dst)     "\tmov " dst ", " addr "\n"
#define store(src, addr)    "\tmov " addr ", " src "\n"
#define lea(addr, dst)      "\tlea " dst ", " addr "\n"

/* Arithmetic (3-Operand Destructive) */
#define add(s1, s2, d)      "\tmov " d ", " s1 "\n\tadd " d ", " s2 "\n"
#define sub(s1, s2, d)      "\tmov " d ", " s1 "\n\tsub " d ", " s2 "\n"
#define mul(s1, s2, d)      "\tmov " d ", " s1 "\n\timul " d ", " s2 "\n"
#define div(s1, s2, d)      "\tmov rax, " s1 "\n\txor rdx, rdx\n\tdiv " s2 "\n\tmov " d ", rax\n"
#define mds(s1, s2, d)      "\tmov rax, " s1 "\n\txor rdx, rdx\n\tdiv " s2 "\n\tmov " d ", rdx\n"
#define and(s1, s2, d)      "\tmov " d ", " s1 "\n\tand " d ", " s2 "\n"
#define or(s1, s2, d)       "\tmov " d ", " s1 "\n\tor " d ", " s2 "\n"
#define xor(s1, s2, d)      "\tmov " d ", " s1 "\n\txor " d ", " s2 "\n"
#define not(s, d)           "\tmov " d ", " s "\n\tnot " d "\n"
#define shl(s, amt, d)      "\tmov " d ", " s "\n\tshl " d ", " #amt "\n"
#define shr(s, amt, d)      "\tmov " d ", " s "\n\tshr " d ", " #amt "\n"
#define sar(s, amt, d)      "\tmov " d ", " s "\n\tsar " d ", " #amt "\n"

/* Control Flow */
#define lbl(name)           #name ":\n"
#define jmp(label)          "\tjmp " #label "\n"
#define call(label)         "\tcall " #label "\n"
#define ret()               "\tret\n"
#define cmp(r1, r2)         "\tcmp " r1 ", " r2 "\n"
#define je(label)           "\tje " #label "\n"
#define jne(label)          "\tjne " #label "\n"
#define jl(label)           "\tjl " #label "\n"
#define jg(label)           "\tjg " #label "\n"
#define bz(reg, label)      "\ttest " reg ", " reg "\n\tjz " #label "\n"
#define bnz(reg, label)     "\ttest " reg ", " reg "\n\tjnz " #label "\n"

/* Stack */
#define stk_push(reg)       "\tpush " reg "\n"
#define stk_pop(reg)        "\tpop " reg "\n"
#define stk_push_pair(a, b) "\tpush " a "\n\tpush " b "\n"
#define stk_pop_pair(a, b)  "\tpop " b "\n\tpop " a "\n"

/* System */
#define syscall()           "\tsyscall\n"
#define pause()             "\tpause\n"
#define halt()              "\thlt\n"
#define nop()               "\tnop\n"
#define fence()             "\tmfence\n"
#define atomic_add(p, v)    "\tlock add " p ", " v "\n"
#define cmpxchg(p, e, n)    "\tlock cmpxchg " p ", " n "\n"
#define cpu_fpu_setup(addr) "\tfldcw " mem_dref(addr) "\n"
