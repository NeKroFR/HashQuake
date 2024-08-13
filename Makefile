CC = gcc
LDFLAGS = -lssl -lcrypto
TARGET = hash_collider
SRCS = main.c
all:
	$(CC) -o $(TARGET) $(SRCS) $(LDFLAGS)
clean:
	rm -f $(TARGET)
.PHONY: all clean
