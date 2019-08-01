CFLAGS+=-std=c11 -g -Og

ALL=sim

all: $(ALL)

sim: sim.o 
sim.o: sim.c 

.PHONY: clean all dist
clean:
	$(RM) $(ALL) *.o

dist:
	tar zcvf Makefile sim.c loop.*
