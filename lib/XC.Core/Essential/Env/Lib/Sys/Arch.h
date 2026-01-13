#pragma once
#include "../types.h"

typedef __XC_REGS_TYPE sysRegisters;

typedef union XC_Sys_Arch_Flags{ 
	struct{
	    word 
	 	carry 	 : 1,
	     	zero  	 : 1,
	     	overflow : 1,
	     	sign 	 : 1;
	} flags;
	word allFlags;

}XC_Sys_Arch_Flags;


Interface(__XC_SYS_ARCH,
	void* fn(getInstructionPtr)();
	
	void  fn(jmp)(void*);

	void* fn(saveRegs)(sysRegisters*);

	void  fn(loadRegs)(sysRegisters*);

        void* fn(getStackPtr)();
        void  fn(setStackPtr)(void* ptr);
        void* fn(getFramePtr)();
        void  fn(setFramePtr)(void* ptr);

        // Add-with-Carry: *sum_out = op1 + op2 + carry_in
        word fn(addCarry)(word* sum_out, word operand1, word operand2, word carry_in);

        // Subtract-with-Borrow: *diff_out = op1 - op2 - borrow_in
        word fn(subCarry)(word* diff_out, word operand1, word operand2, word borrow_in);

        XC_Sys_Arch_Flags 
	     fn(getFlags)();

        void fn(setFlags)(XC_Sys_Arch_Flags flag_word);


)
