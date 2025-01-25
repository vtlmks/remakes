#!/bin/bash

CFLAGS="-O2 -march=native -Wno-unused-result"

# Below this is programs that we put in the $PATH
gcc ${CFLAGS} -o ctime ctime.c -I../include
gcc ${CFLAGS} -o bmp2ugg bmp2ugg.c -I../include
gcc ${CFLAGS} -o fontconv fontconv.c -I../include -lSDL2
gcc ${CFLAGS} -o shader2h shader2h.c -I../include

strip  bmp2ugg ctime fontconv shader2h
mkdir -p ../bin
mv bmp2ugg ctime fontconv shader2h ../bin

