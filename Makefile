########################################################################
#
# Copyright (C) 2011-2014  Adrian Solumsmo
# 
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
########################################################################

# Program variables..
LIBRARY = gtk-custom-table
LINKNIX = lib$(LIBRARY).so
LINKWIN = lib$(LIBRARY).dll
VERSION = 1.0.0
DELFILE = $(LINKNIX) $(LINKWIN) $(OBJECTS) $(DDFILES)
CFLAGS  = -c -Wall -Wno-unused-local-typedefs -MMD -MP -Isrc -Ilib -Iinclude
LDFLAGS = -shared -Wl,--as-needed
CC      = gcc
FINDDIR = src lib
EXAMDIR = examples
TESTDIR = tests

# Program source and object files..
CCFILES = $(shell find $(FINDDIR) -name "*.c")
OBJECTS = $(patsubst %.c,%.o,$(CCFILES))
DDFILES = $(patsubst %.o,%.d,$(OBJECTS))

# Variables for example programs..
EXAMSRC = $(shell find $(EXAMDIR) -name "*.c")
EXAMOBJ = $(patsubst %.c,%.o,$(EXAMSRC))
EXAMEXE = $(patsubst %.c,%.exe,$(EXAMSRC))

# Variables for test programs..
TESTSRC = $(shell find $(TESTDIR) -name "*.c")
TESTOBJ = $(patsubst %.c,%.o,$(TESTSRC))
TESTEXE = $(patsubst %.c,%.exe,$(TESTSRC))

########################################################################
# Standard Linux build..
#
# make [ debug | clean | make-all | debug-all | clean-all ]
#
########################################################################

.PHONY : all debug make-all debug-all linux extras clean-all

# Default for make..
all: CFLAGS += -O2
all: linux

# Make debug..
debug: CFLAGS += -g -DDEBUG
debug: linux

# Make all..
make-all: all extras
debug-all: debug extras

# Faux target..
linux: GTK3 = `pkg-config --cflags --libs gtk+-3.0`
linux: PACKAGES = $(GTK3) -lgthread-2.0
linux: CFLAGS += -fPIC $(PACKAGES)
linux: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(LINKNIX) $(OBJECTS) $(PACKAGES)

# Make examples and tests..
extras: GTK3 = `pkg-config --cflags --libs gtk+-3.0`
extras: CFLAGS = -Wall -Iinclude -Wl,-rpath,$(shell pwd)
extras: extras-make

# Clean all..
clean-all: extras-clean

########################################################################
# Standard Windows build: MSYS, GTK+3
#
# @usage: mingw32-make [ mingw32-[ make | debug | clean ] [-all] ]
#
########################################################################

.PHONY : mingw32-make mingw32-debug mingw32-clean windows
.PHONY : mingw32-make-all mingw32-debug-all mingw32-extras

# MinGW release..
mingw32-make: WINDOWS = -mwindows
mingw32-make: CFLAGS += -O2 
mingw32-make: windows

# MinGW debug..
mingw32-debug: WINDOWS = 
mingw32-debug: CFLAGS += -g -DDEBUG
mingw32-debug: windows

# MinGW make..
windows: LDFLAGS += -Wl,-no-undefined,--enable-runtime-pseudo-reloc
windows: GTK3 = $(shell pkg-config.exe --libs --cflags gtk+-win32-3.0)
windows: PACKAGES = $(GTK3)
windows: CFLAGS += $(PACKAGES)
windows: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(LINKWIN) $(OBJECTS) $(PACKAGES) $(WINDOWS)

# MinGW all..
mingw32-make-all: mingw32-make mingw32-extras
mingw32-debug-all: mingw32-debug mingw32-extras

# MinGW clean..
mingw32-clean: clean

# MinGW clean all..
mingw32-clean-all: extras-clean

# MinGW extras..
mingw32-extras: EXT = .exe
mingw32-extras: GTK3 = $(shell pkg-config.exe --libs --cflags gtk+-win32-3.0)
mingw32-extras: CFLAGS = -Wall -Iinclude
mingw32-extras: extras-make

########################################################################
# Shared targets..
#
########################################################################

.PHONY : clean purge extras-make extras-clean

clean:
	-@rm $(DELFILE) 2>/dev/null || echo "it's clean"

purge:
	-@find ./ -regex ".*\(\.swp\|\.swo\|~\|\.fuse.*\)" -delete

# Target to compile examples and tests..
extras-make: PACKAGES = $(GTK3) -L. -l$(LIBRARY)
extras-make: $(EXAMOBJ) $(TESTOBJ)
	-@cp $(LINKWIN) $(EXAMDIR) 2>/dev/null || true
	-@cp $(LINKWIN) $(TESTDIR) 2>/dev/null || true

# Clean examples dir..
extras-clean: clean
	-@rm $(EXAMOBJ) $(EXAMEXE) $(EXAMDIR)/$(LINKWIN) 2>/dev/null || true
	-@rm $(TESTOBJ) $(TESTEXE) $(TESTDIR)/$(LINKWIN) 2>/dev/null || true

########################################################################
# Project targets..
#
########################################################################

tests/%.o: tests/%.c
	$(CC) $(CFLAGS) $< -o tests/$*$(EXT) $(PACKAGES)

examples/%.o: examples/%.c
	$(CC) $(CFLAGS) $< -o examples/$*$(EXT) $(PACKAGES)

src/%.o: src/%.c
	$(CC) $(CFLAGS) $< -o $@

-include $(DDFILES)

