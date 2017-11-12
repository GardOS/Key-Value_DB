#
# Generic makefile for compiling and linking more than one source file
#
TARGET = key-value_DB
OBJS = $(TARGET).o # List object files for the excutable here

LIB = nodeHandler
LIBOBJS = $(LIB).o # List object files for the library here

DEPS = nodeHandler.h makefile
CFLAGS = -O2 -Wall -std=c11 -g

%.o: %.c $(DEPS)
	gcc -c -o $@ $< $(CFLAGS)

$(TARGET): $(OBJS) $(LIB).a
	gcc -o $@ $^ $(CFLAGS)

$(LIB).a: $(LIBOBJS)
	ar rcs $@ $^

.phony: clean
clean:
	rm -f *.a *.o $(TARGET)

