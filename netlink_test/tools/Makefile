CC ?= gcc
CFLAGS := -g -O0
CPPFLAGS := -I..
LDLIBS := -lmnl

programs += change
programs += event

.PHONY: all
all: $(programs)

.PHONY: clean
clean:
	$(RM) *.o
	$(RM) $(programs)
