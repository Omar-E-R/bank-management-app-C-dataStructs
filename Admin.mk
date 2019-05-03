SRCDIR := src
HDRDIR := include
OBJDIR := bin
LIBDIR := lib

MAINSRC := usr/src/adminInterface.c

EXE := adminInterface.exe

PREPROCHDR := -DADMIN_ACCESS

SOURCES :=$(wildcard $(SRCDIR)/*.c)

OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

INCLUDES := $(wildcard $(HDRDIR)/*.h)

TARGET := libbankmanagementad.so

CC := gcc -Wall

CFLAGS := -ggdb -ljansson -lcrypt -luuid

LFLAGS :=-I$(HDRDIR) -fPIC



admin:$(LIBDIR)/$(TARGET)
	$(CC) $(MAINSRC) -o $(EXE) -I$(HDRDIR) -L./$(LIBDIR) -lbankmanagementad -ljansson
	./$(EXE)

$(LIBDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) -shared $^ -o $@


$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	$(CC) $(CFLAGS) $(LFLAGS) -c  $< -o $@  $(PREPROCHDR)



.PHONY: clean

clean:
	rm $(EXE)
	clear
