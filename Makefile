CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = group40_manager
SRCS = group40_manager.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)
