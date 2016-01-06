# Kernel modules
obj-m = charnull.o

# Specify flags for the module compilation
EXTRA_CFLAGS=-g -O

KVERSION = $(shell uname -r)
all:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(KVERSION)/build M=$(PWD) clean
