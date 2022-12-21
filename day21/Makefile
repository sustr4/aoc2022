all: thisday

thisday: day21.c
	gcc -Wall -Wno-comment -Wno-stringop-truncation -ggdb -O3 -o day21 day21.c

run: day21.sh
	./day21.sh

clean:
	-rm inp.*
	-rm outp*
	-rm sed.*
	-rm day21
