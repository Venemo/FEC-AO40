CC = gcc
CFLAGS = -Wall -Wextra -O3 -std=c99
RM = rm
AR = ar


all: usage

ao40_decode_message.o: ao40_decode_message.c ao40_decode_message.h
	$(CC) $(CFLAGS) -c ao40_decode_message.c

ao40_decode_rs.o: ao40_decode_rs.c ao40_decode_rs.h
	$(CC) $(CFLAGS) -c ao40_decode_rs.c

ao40_spiral-vit_scalar.o: ao40_spiral-vit_scalar.c
	$(CC) $(CFLAGS) -c ao40_spiral-vit_scalar.c

dec_usage.o: dec_usage.c ao40_decode_message.h
	$(CC) $(CFLAGS) -c dec_usage.c

usage: ao40_decode_message.o ao40_decode_rs.o ao40_spiral-vit_scalar.o dec_usage.o
	$(CC) $(CFLAGS) ao40_decode_message.o ao40_decode_rs.o ao40_spiral-vit_scalar.o dec_usage.o -o usage

clean: clean-debug
	-$(RM) *.o
	-$(RM) usage

clean-debug:
	-$(RM) debug_*

