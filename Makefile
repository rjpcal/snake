CC      = g++ -Wall -Werror
CFLAGS	= -O4 -pg
LFLAGS  = -lm

SRC    = main.c window.c gamma.c snake.c ground.c geom.c params.c timing.c gabor.c
INC    = defs.h gabor.h gamma.h \
	geom.h ground.h main.h params.h snake.h timing.h window.h

OBJ    = $(SRC:.c=.o)

default all: makesnake

clean:
	rm -f core *~ *.o *.a makesnake

makesnake: $(OBJ)
	$(CC) -pg -o $@ $(OBJ) $(LFLAGS)

tags:
	etags *.[ch]

test:
	rm -f regtest*.ras
	time ./makesnake regtest
	for f in regtest*.ras; do echo $$f; diff $$f orig_cpp_2/$$f; done

main.o: $(INC)
window.o: $(INC)
geom.o: $(INC)
snake.o: $(INC)
ground.o: $(INC)
gabor.o: $(INC)
timing.o: $(INC)
params.o: $(INC)
