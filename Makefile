CC      = g++ -Wall -Werror
CFLAGS	= -O4 -pg
LFLAGS  = -lm

SRC    = gabor.c geom.c ground.c params.c snake.c window.c timing.c main.c
INC    = gabor.h geom.h ground.h params.h snake.h timing.h window.h

OBJ    = $(SRC:.c=.o)

default all: makesnake

clean:
	rm -f core *~ *.o *.a makesnake

makesnake: $(OBJ)
	$(CC) -pg -o $@ $(OBJ) $(LFLAGS)

tags:
	etags *.[ch]

test:
	rm -f regtest*.pnm
	time ./makesnake regtest
	for f in regtest*.pnm; do echo $$f; cmp $$f orig_cpp_6/$$f; done
	for i in 0 1 2 3; do echo $$i; \
	  pnmarith -difference regtest_$$i.pnm orig_cpp_6/regtest_$$i.pnm \
	  > diff_$$i.pnm; done

main.o: $(INC)
window.o: $(INC)
geom.o: $(INC)
snake.o: $(INC)
ground.o: $(INC)
gabor.o: $(INC)
timing.o: $(INC)
params.o: $(INC)
