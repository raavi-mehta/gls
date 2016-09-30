CC=gcc

# -lssl and -lcrypto are used for computing the MD5 checksum
# -g is used to allow usage of gdb
# -Wall is used to detect all warnings
CFLAGS=-lssl -lcrypto -g -Wall

all: gls

gls: gls.c
	$(CC) $(CFLAGS) gls.c -o gls

clean:
	rm -f gls