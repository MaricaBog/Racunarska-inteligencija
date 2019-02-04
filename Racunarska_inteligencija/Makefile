CC = gcc
CFLAGS = -Wall -Wextra
TARGET = ga

all: $(TARGET)

$(TARGET) : $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c -lm

clean:
	$(RM) $(TARGET)
