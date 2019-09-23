.PHONY: build run clean

build:
	gcc -std=c99 -Wall src/gameoflife.c -o gameoflife

run:
	./gameoflife

clean:
	rm -rf gameoflife