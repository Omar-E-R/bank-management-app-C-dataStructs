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
	rm -f ./usr/*.txt
	rm -f ./root/*.txt
	rm -f ./bin/*
	rm -f ./lib/libbankmanagement*
	rm -f -R ./database/*
	rm -f ./adminInterface.exe
	rm -f ./developperInterface.exe
	rm -f ./clientInterface.exe
	rm -f ./managerInterface.exe
	clear
