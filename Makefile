SRCDIR := src
HDRDIR := include
OBJDIR := bin
LIBDIR := lib
# TMPDIR := $(OBJDIR)


SOURCES :=$(wildcard $(SRCDIR)/*.c)

OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

INCLUDES := $(wildcard $(HDRDIR)/*.h)

TARGET := bankmanagment.so

CC := gcc

CFLAGS := -Wall -ggdb  -ljansson -lcrypt -luuid

LFLAGS :=-I$(HDRDIR)



all: $(LIBDIR)/$(TARGET)
#	rm -f $(TMPDIR)/*


$(LIBDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@


$(OBJECTS): $(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCLUDES)
	$(CC) $(CFLAGS) $(LFLAGS) -c  $< -o $@ -DWORKSPACE=\"$$PWD\"


#link and compile all the TEMPORARY SRC FILES, that normally are responsible for generating .dat files for plot


# PLOTSRC := $(wildcard $(TMPDIR)/*.c)



# PLOT := $(PLOTSRC:$(TMPDIR)/%.c=$(TMPDIR)/%)

# plot: $(PLOT)
# 	$^
# 	rm -f $(PLOT)
# 	rm -f $(TMPDIR)/*

# $(PLOT):$(TMPDIR)/%: $(PLOTSRC)
# 	$(CC) $(CFLAGS) -I$(HDRDIR) -L$(LIBDIR) -ltrajectoire -Wl,-rpath=$(LIBDIR) $^ -o $@

# .PHONY: clean

# clean:
# 	rm -f $(LIBDIR)/$(TARGET)
# 	rm -f ./data/*.*
