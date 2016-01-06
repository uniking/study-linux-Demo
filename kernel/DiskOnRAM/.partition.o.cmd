cmd_/home/sxis/workspace/kernel/DiskOnRAM/partition.o := gcc -Wp,-MD,/home/sxis/workspace/kernel/DiskOnRAM/.partition.o.d  -nostdinc -isystem /usr/lib/gcc/x86_64-redhat-linux/4.8.3/include -I/usr/src/kernels/3.10.0-229.14.1.el7.x86_64/arch/x86/include -Iarch/x86/include/generated  -Iinclude -I/usr/src/kernels/3.10.0-229.14.1.el7.x86_64/arch/x86/include/uapi -Iarch/x86/include/generated/uapi -I/usr/src/kernels/3.10.0-229.14.1.el7.x86_64/include/uapi -Iinclude/generated/uapi -include /usr/src/kernels/3.10.0-229.14.1.el7.x86_64/include/linux/kconfig.h -D__KERNEL__ -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -m64 -mno-sse -mpreferred-stack-boundary=3 -mtune=generic -mno-red-zone -mcmodel=kernel -funit-at-a-time -maccumulate-outgoing-args -Wframe-larger-than=2048 -DCONFIG_AS_CFI=1 -DCONFIG_AS_CFI_SIGNAL_FRAME=1 -DCONFIG_AS_CFI_SECTIONS=1 -DCONFIG_AS_FXSAVEQ=1 -DCONFIG_AS_AVX=1 -DCONFIG_AS_AVX2=1 -pipe -Wno-sign-compare -fno-asynchronous-unwind-tables -mno-sse -mno-mmx -mno-sse2 -mno-3dnow -mno-avx -Wframe-larger-than=2048 -fstack-protector-strong -Wno-unused-but-set-variable -fno-omit-frame-pointer -fno-optimize-sibling-calls -g -pg -mfentry -DCC_USING_FENTRY -fno-inline-functions-called-once -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DCC_HAVE_ASM_GOTO  -DMODULE  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(partition)"  -D"KBUILD_MODNAME=KBUILD_STR(dor)" -c -o /home/sxis/workspace/kernel/DiskOnRAM/.tmp_partition.o /home/sxis/workspace/kernel/DiskOnRAM/partition.c

source_/home/sxis/workspace/kernel/DiskOnRAM/partition.o := /home/sxis/workspace/kernel/DiskOnRAM/partition.c

deps_/home/sxis/workspace/kernel/DiskOnRAM/partition.o := \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  include/uapi/linux/types.h \
  /usr/src/kernels/3.10.0-229.14.1.el7.x86_64/arch/x86/include/uapi/asm/types.h \
  /usr/src/kernels/3.10.0-229.14.1.el7.x86_64/include/uapi/asm-generic/types.h \
  include/asm-generic/int-ll64.h \
  include/uapi/asm-generic/int-ll64.h \
  /usr/src/kernels/3.10.0-229.14.1.el7.x86_64/arch/x86/include/uapi/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/uapi/asm-generic/bitsperlong.h \
  /usr/src/kernels/3.10.0-229.14.1.el7.x86_64/include/uapi/linux/posix_types.h \
  include/linux/stddef.h \
  include/uapi/linux/stddef.h \
  /usr/src/kernels/3.10.0-229.14.1.el7.x86_64/arch/x86/include/asm/posix_types.h \
    $(wildcard include/config/x86/32.h) \
  /usr/src/kernels/3.10.0-229.14.1.el7.x86_64/arch/x86/include/uapi/asm/posix_types_64.h \
  /usr/src/kernels/3.10.0-229.14.1.el7.x86_64/include/uapi/asm-generic/posix_types.h \
  /usr/lib/gcc/x86_64-redhat-linux/4.8.3/include/stdarg.h \
  include/uapi/linux/string.h \
  /usr/src/kernels/3.10.0-229.14.1.el7.x86_64/arch/x86/include/asm/string.h \
  /usr/src/kernels/3.10.0-229.14.1.el7.x86_64/arch/x86/include/asm/string_64.h \
    $(wildcard include/config/kmemcheck.h) \
  /home/sxis/workspace/kernel/DiskOnRAM/partition.h \

/home/sxis/workspace/kernel/DiskOnRAM/partition.o: $(deps_/home/sxis/workspace/kernel/DiskOnRAM/partition.o)

$(deps_/home/sxis/workspace/kernel/DiskOnRAM/partition.o):
