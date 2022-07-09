#
# Makefile
#
# Copyright 2017 Ammon Smith
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

V         ?= 0

ECHO_CC_0  = @echo '[CC]  $@'; $(CC)
ECHO_CC_1  = $(CC)
ECHO_CC    = $(ECHO_CC_$(V))

ECHO_CC2_0 = @echo '[CC]  $@'; $(CC) -fpic
ECHO_CC2_1 = $(CC) -fpic
ECHO_CC2   = $(ECHO_CC2_$(V))

ECHO_LD_0  = @echo '[LD]  $@'; $(CC)
ECHO_LD_1  = $(CC)
ECHO_LD    = $(ECHO_LD_$(V))

ECHO_AR_0  = @echo '[AR]  $@'; $(AR)
ECHO_AR_1  = $(AR)
ECHO_AR    = $(ECHO_AR_$(V))

ECHO_DEP_0 = @echo '[DEP] $@'; $(CC)
ECHO_DEP_1 = $(CC)
ECHO_DEP   = $(ECHO_DEP_$(V))

ECHO_RUN_0 = @echo '[RUN] $<'; echo; $(shell realpath $<)
ECHO_RUN_1 = $(shell realpath $<)
ECHO_RUN   = $(ECHO_RUN_$(V))

EXE_SRCS  := main.c
EXE_OBJS  := $(EXE_SRCS:.c=.o)
EXE_DEPS  := $(EXE_SRCS:.c=.d)

TEST_SRCS := test.c
TEST_OBJS := $(TEST_SRCS:.c=.o)
TEST_DEPS := $(TEST_SRCS:.c=.d)

SOURCES   := $(filter-out $(EXE_SRCS),$(filter-out $(TEST_SRCS),$(wildcard *.c)))
OBJECTS   := $(SOURCES:.c=.o)
DEPENDS   := $(SOURCES:.c=.d)

PIC_OBJS  := $(SOURCES:.c=.pic.o)

FLAGS     := -pipe
INCLUDES  := -I.
LINKING   := -lc
WARNINGS  := -ansi -pedantic -Wall -Wextra -Wshadow -Wmissing-prototypes

ifeq ($(shell uname),Darwin)
SO_EXT    := dylib
else
SO_EXT    := so
endif

STATIC    := libroman.a
SHARED    := libroman.$(SO_EXT)
EXE       := roman
TEST      := test_roman

.PHONY: all test debug clean

all: CFLAGS += -O2
all: $(EXE) $(SHARED) $(STATIC)

test: CFLAGS += -g
test: $(TEST)
	$(ECHO_RUN)

debug: CFLAGS += -g
debug: $(EXE) $(SHARED) $(STATIC)

clean:
	rm -f $(EXE) *.$(SO_EXT) *.a *.o *.d

%.o: %.c
	$(ECHO_CC) $(FLAGS) $(INCLUDES) $(WARNINGS) $(CFLAGS) -c -o $@ $<

%.d: %.c
	$(ECHO_DEP) $(FLAGS) $(INCLUDES) -MM $< > $@

%.pic.o: %.c
	$(ECHO_CC2) $(FLAGS) $(INCLUDES) $(WARNINGS) $(CFLAGS) -c -o $@ $<

$(STATIC): $(OBJECTS)
	$(ECHO_AR) rcs $@ $^

$(SHARED): $(PIC_OBJS)
	$(ECHO_LD) $(FLAGS) $(LINKING) $(CFLAGS) -shared -o $@ $^

$(EXE): $(OBJECTS) $(EXE_OBJS)
	$(ECHO_LD) $(FLAGS) $(LINKING) $(CFLAGS) -o $@ $^

$(TEST): $(OBJECTS) $(TEST_OBJS)
	$(ECHO_LD) $(FLAGS) $(LINKING) $(CFLAGS) -o $@ $^

-include $(DEPENDS) $(EXE_DEPS) $(TEST_DEPS)

