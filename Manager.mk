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

manager: $(LIBDIR)/$(TARGET)
	gcc -Wall ./usr/src/managerInterface.c -o ./managerInterface -I./include -L./lib -lbankmanagement -ljansson

$(LIBDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) -shared $^ -o $@


$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	$(CC) $(CFLAGS) $(LFLAGS) -c  $< -o $@ -DEMPLOYEE_ACCESS -DMULTI_ACCESS -DEMPLOYEE_LOGIN


.PHONY: clean

clean:
	rm ./bin/*
	rm ./managerInterface
	clear