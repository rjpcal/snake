SHELL	= /bin/sh

CC      = gcc
CFLAGS	= -I./ -O4 -g
#CFLAGS = -I./ -O -DKEEPTRACK
LFLAGS  = -lgl  -lfm -lX11 -lm -lGL 
LFLAGS2 = -lm 

SRC    = main.c applic.c window.c gamma.c snake.c ground.c geom.c params.c file.c timing.c gabor.c
INC    = incl.h params.h keydef.h

OTH    = Makefile *.sta

OBJ    = $(SRC:.c=.o)

MAIN   = makesnake

ALL    = $(MAIN)

default all:	$(ALL)

clean:
	rm -f core *~ *.o *.a $(ALL)

tar: $(SRC) $(INC) 
	tar cvf $(MAIN).tar $(SRC1) $(INC) $(OTH)

untar:
	tar xvf $(MAIN).tar

$(MAIN): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LFLAGS2)

tags:
	etags *.[ch]

test:
	rm regtest*.ras
	./makesnake regtest
	for f in regtest*.ras; do echo $$f; diff $$f orig/$$f; done

main.o: $(INC)
window.o: $(INC)
applic.o: $(INC)
geom.o: $(INC)
snake.o: $(INC)
ground.o: $(INC)
gabor.o: $(INC)
timing.o: $(INC)
params.o: $(INC)
file.o: $(INC)
