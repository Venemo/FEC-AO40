CC = gcc
CFLAGS = -Wall -Wextra -O3 -std=c99 -DAO40SHORT_DEBUG
RM = rm
AR = ar

all: usage

ao40short_decode_message.o: ao40short_decode_message.c ao40short_decode_message.h
	$(CC) $(CFLAGS) -c ao40short_decode_message.c

ao40short_decode_rs.o: ao40short_decode_rs.c ao40short_decode_rs.h
	$(CC) $(CFLAGS) -c ao40short_decode_rs.c

ao40short_spiral-vit_scalar_1280.o: ao40short_spiral-vit_scalar_1280.c ao40short_spiral-vit_scalar_1280.h
	$(CC) $(CFLAGS) -c ao40short_spiral-vit_scalar_1280.c

dec_short_usage.o: dec_short_usage.c ao40short_decode_message.h
	$(CC) $(CFLAGS) -c dec_short_usage.c

usage: dec_short_usage.o ao40short_spiral-vit_scalar_1280.o ao40short_decode_rs.o ao40short_decode_message.o
	$(CC) $(CFLAGS) dec_short_usage.o ao40short_spiral-vit_scalar_1280.o ao40short_decode_rs.o ao40short_decode_message.o -o dec_usage

clean: clean-debug
	-$(RM) *.o
	-$(RM) usage

