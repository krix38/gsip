SRCDIR = src
HDRDIR = headers
DEPSDIR = deps
BINNAME = gsip

DEPSDIRS := $(sort $(dir $(wildcard ./$(DEPSDIR)/*/)))
SOURCES := $(wildcard $(foreach dir,$(DEPSDIRS),$(dir)$(SRCDIR)/*) $(foreach dir,$(DEPSDIRS),$(dir)$(HDRDIR)/*)) ./$(SRCDIR)/*

CODEFILES := $(filter %.c,$(SOURCES))
HEADERS := $(filter %.h,$(SOURCES))

INCLUDES := $(foreach dir,$(DEPSDIRS), -I $(dir)$(HDRDIR)/)
OBJECTS := $(subst $(SRCDIR)/,,$(CODEFILES:%.c=%.o)) ./$(BINNAME).o

CFLAGS := -Wall

.SECONDEXPANSION:
%.o:    $$(addsuffix /%.c,$$(SRCDIR)) $$(addsuffix /%.h,$$(HDRDIR))
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)


$(BINNAME):	$(OBJECTS)
	$(CC) $(OBJECTS) -o $(BINNAME)

all:	$(BINNAME)

$(BINNAME).o:	src/$(BINNAME).c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

.PHONY: clean
clean:
	$(foreach dir,$(DEPSDIRS), rm -f $(dir)*.o && ) true
	rm -f $(BINNAME).o
	rm -f $(BINNAME)
