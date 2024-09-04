CC = gcc
LDFLAGS = -lssl -lcrypto
TARGET = hash_collider
SRCS = main.c gen_dict.c

all:
	$(CC) -g -fsanitize=address -o $(TARGET) $(SRCS) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
