SRCDIR := src
HDRDIR := include
OBJDIR := bin
LIBDIR := lib

MAINSRC := usr/src/clientInterface.c

EXE := clientInterface.exe

PREPROCHDR := -DCLIENT_LOGIN

SOURCES :=$(wildcard $(SRCDIR)/*.c)

OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

INCLUDES := $(wildcard $(HDRDIR)/*.h)

TARGET := libbankmanagementcl.so

CC := gcc -Wall

CFLAGS := -ggdb -ljansson -lcrypt -luuid

LFLAGS :=-I$(HDRDIR) -fPIC


client: $(LIBDIR)/$(TARGET)
	$(CC) $(MAINSRC) -o $(EXE) -I$(HDRDIR) -L./$(LIBDIR) -lbankmanagementcl -ljansson
	./$(EXE)

$(LIBDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) -shared $^ -o $@


$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	$(CC) $(CFLAGS) $(LFLAGS) -c  $< -o $@ $(PREPROCHDR)


.PHONY: clean

clean:
	rm -f $(EXE)
	clear
