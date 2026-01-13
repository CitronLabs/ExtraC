/* Registers */
#define gpr_a "eax"
#define gpr_b "ebx"
#define gpr_c "ecx"
#define gpr_d "edx"

#define r0  "eax"
#define r1  "ecx"
#define r2  "edx"
#define r3  "ebx"
#define rbp "ebp"
#define rsp "esp"
#define rip "eip"

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

/* Arithmetic */
#define add(s1, s2, d)      "\tmov " d ", " s1 "\n\tadd " d ", " s2 "\n"
#define sub(s1, s2, d)      "\tmov " d ", " s1 "\n\tsub " d ", " s2 "\n"
#define mul(s1, s2, d)      "\tmov " d ", " s1 "\n\timul " d ", " s2 "\n"
#define div(s1, s2, d)      "\tmov eax, " s1 "\n\txor edx, edx\n\tdiv " s2 "\n\tmov " d ", eax\n"
#define mds(s1, s2, d)      "\tmov eax, " s1 "\n\txor edx, edx\n\tdiv " s2 "\n\tmov " d ", edx\n"
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
#define bz(reg, label)      "\ttest " reg ", " reg "\n\tjz " #label "\n"
#define bnz(reg, label)     "\ttest " reg ", " reg "\n\tjnz " #label "\n"

/* Stack */
#define stk_push(reg)       "\tpush " reg "\n"
#define stk_pop(reg)        "\tpop " reg "\n"
#define stk_push_pair(a, b) "\tpush " a "\n\tpush " b "\n"
#define stk_pop_pair(a, b)  "\tpop " b "\n\tpop " a "\n"

/* System */
#define syscall()           "\tint 0x80\n"
#define pause()             "\tpause\n"
#define halt()              "\thlt\n"
#define nop()               "\tnop\n"
#define fence()             "\tmfence\n"
#define atomic_add(p, v)    "\tlock add " p ", " v "\n"
#define cmpxchg(p, e, n)    "\tlock cmpxchg " p ", " n "\n"
#define cpu_fpu_setup(addr) "\tfldcw " mem_dref(addr) "\n"
