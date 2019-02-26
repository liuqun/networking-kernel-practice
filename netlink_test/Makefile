KDIR := /lib/modules/$(shell uname -r)/build
LIB_PATH = /usr/lib
obj-m += nlexample.o
obj-m += nlexample2.o
obj-m += genlexample.o
TOOLSDIR := ./tools

.PHONY: all
all: modules tools

.PHONY: modules
modules:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

.PHONY: tools
tools:
	$(MAKE) -C $(TOOLSDIR)

.PHONY: clean
clean:
	rm -rf *.o *.ko *.mod.* *.cmd .module* modules* Module* .*.cmd .tmp*
	$(MAKE) -C $(TOOLSDIR) clean
