SRCDIR := src
HDRDIR := include
OBJDIR := bin
LIBDIR := lib


SOURCES :=$(wildcard $(SRCDIR)/*.c)

OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

INCLUDES := $(wildcard $(HDRDIR)/*.h)

TARGET := libbankmanagement.so

CC := gcc

CFLAGS := -Wall -ggdb -ljansson -lcrypt -luuid

LFLAGS :=-I$(HDRDIR) -fPIC



admin: $(LIBDIR)/$(TARGET)
	gcc -Wall ./usr/src/adminInterface.c -o ./adminInterface -I./include -L./lib -lbankmanagement -ljansson
	./adminInterface

$(LIBDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) -shared $^ -o $@


$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	$(CC) $(CFLAGS) $(LFLAGS) -c  $< -o $@ -DADMIN_ACCESS



.PHONY: clean

clean:
	rm ./bin/*
	rm ./adminInterface
	clear
