SRC_DIR = ./src
TARGET = HashQuake

all: build

build:
	go build -o $(TARGET) $(SRC_DIR)/*.go

run: build
	./$(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all build clean run

