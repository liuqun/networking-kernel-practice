CC ?= gcc
CFLAGS := -g -O0
CPPFLAGS := -I..
LDLIBS := -lmnl

programs += change
programs += event

genl-programs += genl-change
genl-programs += genl-event
genl-programs += genl-resolve
genl-programs += genl-get

.PHONY: all
all: $(programs) $(genl-programs)

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) $(programs)
	$(RM) $(genl-programs)
