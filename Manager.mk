SRCDIR := src
HDRDIR := include
OBJDIR := bin
LIBDIR := lib

MAINSRC := usr/src/managerInterface.c

EXE := managerInterface.exe

PREPROCHDR := -DEMPLOYEE_ACCESS -DMULTI_ACCESS -DEMPLOYEE_LOGIN

SOURCES :=$(wildcard $(SRCDIR)/*.c)

OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

INCLUDES := $(wildcard $(HDRDIR)/*.h)

TARGET := libbankmanagementma.so

CC := gcc -Wall

CFLAGS := -ggdb -ljansson -lcrypt -luuid

LFLAGS :=-I$(HDRDIR) -fPIC

manager: $(LIBDIR)/$(TARGET)
	$(CC) $(MAINSRC) -o $(EXE) -I$(HDRDIR) -L./$(LIBDIR) -lbankmanagementma -ljansson
	./$(EXE)

$(LIBDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) -shared $^ -o $@


$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	$(CC) $(CFLAGS) $(LFLAGS) -c  $< -o $@ $(PREPROCHDR)


.PHONY: clean

clean:
	rm $(EXE)
	clear