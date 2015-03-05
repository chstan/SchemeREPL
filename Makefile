CC = clang

CFLAGS = -Wall -Wextra -Werror -fstack-protector -Wno-typedef-redefinition -pedantic -O0 -g

INCLUDES = -I/Users/chstansbury/include
LDFLAGS =

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = SchemeREPL

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

.PHONY: clean

clean:
	@rm -f $(TARGET) $(OBJECTS) $(DEPENDENCIES) core
