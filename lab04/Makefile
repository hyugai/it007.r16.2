CC = cc
CFLAGS = -std=c11 -O2 -Wall -Wextra -Wpedantic

.PHONY: all clean
all: sjf srt rr

sjf: sjf.c scheduler.h

srt: srt.c scheduler.h

rr: rr.c scheduler.h

clean:
	rm -f sjf srt rr
