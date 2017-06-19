#

CROSS =
CFLAGS := -g -Wall -O2
LIBS :=
INCLUDES := -Isrc/damnlib/inc \
		  -Isrc/damnlib \
		  -Isrc

BUILDDIR := build
SRCDIR := src

SRCS_LIB := $(shell find $(SRCDIR)/damnlib -type f -name *.c)

OBJS_LIB := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SRCS_LIB:.c=.o))

MAINS = $(BUILDDIR)/damnlib.a

.PHONY: all clean remake

all: $(MAINS)
	@mkdir -p $(BUILDDIR)
	@echo Done compiling $(MAINS)

$(BUILDDIR)/damnlib.a: $(OBJS_LIB)
	$(CROSS)ar cr $@ $^ 


$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CROSS)gcc $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) -rf $(BUILDDIR)

remake: clean all
	@echo Finished remake
