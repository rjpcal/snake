CC      = g++ -Wall -Werror
CFLAGS	= -O4 -pg
LFLAGS  = -lm

SRC    = gabor.c ground.c params.c snake.c  main.c
INC    = gabor.h ground.h params.h snake.h  geom.h 

OBJ    = $(SRC:.c=.o)

default all: makesnake

clean:
	rm -f core *~ *.o *.a makesnake

makesnake: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LFLAGS)

tags:
	etags *.[ch]

test:
	rm -f regtest*.pnm
	time ./makesnake regtest
	for i in 0 1 2 3; do \
	  pnmarith -difference regtest_$$i.pnm ref/regtest_$$i.pnm \
	  > diff_$$i.pnm; done
	for f in regtest*.pnm; do echo $$f; cmp $$f ref/$$f; done

gabor.o: $(INC)
ground.o: $(INC)
params.o: $(INC)
snake.o: $(INC)
main.o: $(INC)
