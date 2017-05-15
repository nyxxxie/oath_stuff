CLIENT_BIN=oath
CLIENT_OBJ=main.o sha1.o hmac_sha1.o

CFLAGS=-Wall -ansi -pedantic

all: $(CLIENT_BIN)

debug: CFLAGS += -g
debug: all

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

$(CLIENT_BIN): $(CLIENT_OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

.PHONY: clean
clean:
	rm -f *.o ${CLIENT_OBJ} ${CLIENT_BIN}
