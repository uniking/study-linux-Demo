#include "common.h"

unsigned int WriteProcessRegister(int pid, unsigned int reg)
{
        struct user_regs_struct regs;
        unsigned int val;
        ptrace(PTRACE_GETREGS, pid, NULL, &regs);
        switch(reg)
	  {
                case r_eax:
                        regs.rax = val;
                        break;
                case r_ecx:
                        regs.rcx = val;
                        break;
                case r_edx:
                        regs.rdx = val;
                        break;
                case r_ebx:
                        regs.rbx = val;
                        break;
                case r_esp:
                        regs.rsp = val;
                case r_ebp:
                        regs.rbp = val;
                        break;
                case r_esi:
                        regs.rsi = val;
                        break;
                case r_edi:
                        regs.rdi = val;
                        break;
                case r_eip:
                        regs.rip = val;
                        break;
                case r_eflags:
                        regs.eflags = val;
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
      
	ptrace(PTRACE_SETREGS, pid, NULL, &regs);
	return val;
}

int WriteProcessRegisterAll(int pid, struct user_regs_struct* regs)
{
	int iRtn = 0;
	iRtn = ptrace(PTRACE_SETREGS, pid, NULL, regs);
	return iRtn;
}
