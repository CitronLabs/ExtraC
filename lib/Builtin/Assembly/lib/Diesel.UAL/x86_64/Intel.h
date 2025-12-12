
#define DSEAL_SYNTAX_CONFIG ".intel_syntax noprefix\n\t"

// =========================================================================
// I. REGISTER ABSTRACTION - Intel (No '%' prefix)
// =========================================================================

// For 32-bit (i386) or lower half of 64-bit registers (x86_64)
#if defined(__i386__) || defined(__x86_64__)
#define GPR_A 		"eax"
#define GPR_B 		"ebx"
#define GPR_C 		"ecx"
#define GPR_D 		"edx"
#define GPR_E 		"esi"
#define GPR_F 		"edi"
#define GPR_G 		"ebp"
#define GPR_H 		"esp"

// Dedicated Pointers/Special Registers
#define SP_REG 		"esp"
#define BP_REG 		"ebp"
#define IP_REG 		"eip"
#define FLAG_REG 	"eflags"
#endif

// =========================================================================
// II. CLOBBER LIST ABSTRACTION - Intel (Identical to AT&T Clobber)
// =========================================================================
// The Clobber list always uses the compiler's internal register naming scheme.
#define GPR_A_CLOBBER 		"eax"
#define GPR_B_CLOBBER 		"ebx"
// ... (All other clobbers are identical to the AT&T implementation) ...

// =========================================================================
// III. MEMORY ACCESS ABSTRACTION - Intel
// =========================================================================
// Format: [base + index*scale + disp] or simply [reg]

// Register-Indirect Dereference: [reg]
#define MEM_DEREF(reg) "[" reg "]"

// Register-Indirect with Constant Offset: [reg + offset]
#define MEM_OFFSET(reg, offset) "[" reg " + " #offset "]"

// Register-Indirect with Index and Scale
#define MEM_INDEXED(base, index, scale, disp) "[" base " + " index "*" #scale " + " #disp "]"

// =========================================================================
// IV. INSTRUCTION MACROS - Intel (Operand Order: Destination, Source)
// =========================================================================

// --- Data Movement ---
// MOV: Destination = Source (mov dest, src)
#define MOV(dest, src) 		"mov " #dest ", " #src "\n\t"
#define MOV_B(dest, src) 	"mov " #dest ", " #src "\n\t" // Size implied by operand or suffix in AIL
#define MOV_W(dest, src) 	"mov " #dest ", " #src "\n\t"
#define MOV_L(dest, src) 	"mov " #dest ", " #src "\n\t"
#define MOV_Q(dest, src) 	"mov " #dest ", " #src "\n\t"

// LOAD (memory to register)
#define LOAD(dest, addr) 	"mov " #dest ", " addr "\n\t"

// STORE (register to memory)
#define STORE(addr, src) 	"mov " addr ", " #src "\n\t"

// --- Arithmetic and Logic Operations ---
#define ADD(dest, src) 		"add " #dest ", " #src "\n\t" // dest = dest + src
#define SUB(dest, src) 		"sub " #dest ", " #src "\n\t" // dest = dest - src
#define AND(dest, src) 		"and " #dest ", " #src "\n\t"
#define OR(dest, src) 		"or " #dest ", " #src "\n\t"
#define XOR(dest, src) 		"xor " #dest ", " #src "\n\t"
#define NOT(dest) 		"not " #dest "\n\t"

// MUL/DIV (Implicitly operate on EAX/EDX)
#define MUL(src) 		"mul " #src "\n\t"
#define DIV(src) 		"div " #src "\n\t"

// Shifts
#define SHL(dest, count) 	"shl " #dest ", " #count "\n\t"
#define SHR(dest, count) 	"shr " #dest ", " #count "\n\t"

// --- Stack Operations ---
#define PUSH(src) 		"push " #src "\n\t"
#define POP(dest) 		"pop " #dest "\n\t"

// --- Control Flow ---
#define JMP(label) 		"jmp " #label "\n\t"
#define CALL_FUNC(addr) 	"call " #addr "\n\t"
#define RET 			"ret\n\t"

// --- Flag/Conditional Operations ---
#define CMP(op1, op2) 		"cmp " #op1 ", " #op2 "\n\t" // cmp dest, src (Sets flags based on dest - src)
#define JZ(label) 		"jz " #label "\n\t"
#define JNE(label) 		"jne " #label "\n\t"
#define JG(label) 		"jg " #label "\n\t"
#define JL(label) 		"jl " #label "\n\t"
#define JGE(label)		"jge " #label "\n\t"
#define JLE(label) 		"jle " #label "\n\t"

// Set byte based on flag condition
#define SETZ(dest_byte_reg) 	"setz " #dest_byte_reg "\n\t"
#define SETNE(dest_byte_reg) 	"setne " #dest_byte_reg "\n\t"

// --- Calling Convention Abstraction ---
#define FUNC_PROLOGUE 		"push ebp\n\tmov ebp, esp\n\t"
#define FUNC_EPILOGUE 		"leave\n\tret\n\t"
