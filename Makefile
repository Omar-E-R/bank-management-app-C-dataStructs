SRCDIR := src
HDRDIR := include
OBJDIR := bin
LIBDIR := lib


SOURCES :=$(wildcard $(SRCDIR)/*.c)

OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

INCLUDES := $(wildcard $(HDRDIR)/*.h)

TARGET := bankmanagement.so

CC := gcc

CFLAGS := -Wall -ggdb -ljansson -lcrypt -luuid

LFLAGS :=-I$(HDRDIR)



all: $(LIBDIR)/$(TARGET)
	./lib/bankmanagement.so

$(LIBDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@


$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	$(CC) $(CFLAGS) $(LFLAGS) -c  $< -o $@ -DWORKSPACE=\"$$PWD\"

.PHONY: clean

clean:
	rm ./bin/*
	clear
