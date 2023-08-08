CC = gcc
OFLAGS = -Og
CFLAGS = $(OFLAGS) -Wall -g

CINC =
F64 =-m64

%.s: %.c
	$(CC) $(OFLAGS) $(CINC) -S $(F64) $<

%.d: %.c
	$(CC) $(CFLAGS) $(CINC) $(F64) $< -o $*
	objdump -d $* > $@
	# rm -f $*

files:	swap.s

clean:
	rm -f *~ *.s *.d *.s *.64d
