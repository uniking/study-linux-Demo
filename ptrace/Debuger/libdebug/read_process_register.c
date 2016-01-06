#include "common.h"

unsigned int ReadProcessRegister(int pid, unsigned int reg)
{
        struct user_regs_struct regs;
        unsigned int val;
        ptrace(PTRACE_GETREGS, pid, NULL, &regs);
        switch(reg)
	  {
                case r_eax:
                        val = regs.rax;
                        break;
                case r_ecx:
                        val = regs.rcx;
                        break;
                case r_edx:
                        val = regs.rdx;
                        break;
                case r_ebx:
                        val = regs.rbx;
                        break;
                case r_esp:
                        val = regs.rsp;
                case r_ebp:
                        val = regs.rbp;
                        break;
                case r_esi:
                        val = regs.rsi;
                        break;
                case r_edi:
                        val = regs.rdi;
                        break;
                case r_eip:
                        val = regs.rip;
                        break;
                case r_eflags:
                        val = regs.eflags;
                        break;
                case dr0:
                case dr1:
                case dr2:
                case dr3:
                case dr4:
                case dr5:
                case dr6:
                case dr7:
                        break;
                default:
                        fprintf(stderr, "Unknown register\n");
                        exit(-1);
        }
      
	return val;
}

int ReadProcessRegisterAll(int pid, struct user_regs_struct* regs)
{
	int iRtn = 0;
	iRtn = ptrace(PTRACE_GETREGS, pid, NULL, regs);

	return iRtn;      
}
