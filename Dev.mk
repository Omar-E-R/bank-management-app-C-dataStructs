SRCDIR := src
HDRDIR := include
OBJDIR := bin
LIBDIR := lib

MAINSRC := usr/src/developperInterface.c

EXE := developperInterface.exe

PREPROCHDR := -DADMIN_ACCESS

SOURCES :=$(wildcard $(SRCDIR)/*.c)

OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

INCLUDES := $(wildcard $(HDRDIR)/*.h)

TARGET := libbankmanagementad.so

CC := gcc -Wall

CFLAGS := -ggdb -ljansson -lcrypt -luuid

LFLAGS :=-I$(HDRDIR) -fPIC



dev: $(LIBDIR)/$(TARGET)
	$(CC) $(MAINSRC) -o $(EXE) -I$(HDRDIR) -L./$(LIBDIR) -lbankmanagementad -ljansson
	./$(EXE)

$(LIBDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) -shared $^ -o $@


$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	$(CC) $(CFLAGS) $(LFLAGS) -c  $< -o $@ $(PREPROCHDR)


.PHONY: clean

clean:
	rm ./usr/*.txt
	rm ./root/*.txt
	rm ./bin/*
	rm ./lib/libbankmanagement.so
	rm -R ./database/*
	rm ./adminInterface
	rm ./developperInterface
	rm ./clientInterface
	rm ./managerInterface
	clear
