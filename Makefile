CC      = g++ -Wall -Werror
CFLAGS	= -O4 -pg
LFLAGS  = -lm

SRC    = gabor.c ground.c params.c snake.c window.c  main.c
INC    = gabor.h ground.h params.h snake.h window.h  geom.h 

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

gabor.o: $(INC)
ground.o: $(INC)
params.o: $(INC)
snake.o: $(INC)
window.o: $(INC)
main.o: $(INC)
