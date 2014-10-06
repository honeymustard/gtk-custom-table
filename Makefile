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
LIBRARY = gtk_custom_table
VERSION = 1.0.0
DELFILE = $(LIBRARY).dll $(LIBRARY).so $(OBJECTS) $(DDFILES)
CFLAGS  = -c -Wall -Wno-unused-local-typedefs -MMD -MP -Isrc -Ilib -Iinclude
FINDDIR = src lib
LDFLAGS = -shared -Wl,--as-needed,-no-undefined,--enable-runtime-pseudo-reloc
CC      = gcc

# Program source and object files..
CCFILES = $(shell find $(FINDDIR) -name "*.c")
OBJECTS = $(patsubst %.c,%.o,$(CCFILES))
DDFILES = $(patsubst %.o,%.d,$(OBJECTS))

# Variables for example programs..
EXAMDIR = examples
EXCFILE = $(shell find $(EXAMDIR) -name "*.c")
EXOFILE = $(patsubst %.c,%,$(EXCFILE))

########################################################################
# Standard Linux build..
#
# make | make [ debug | clean | examples ]
#
########################################################################

.PHONY : all debug linux examples

# Default for make..
all: CFLAGS += -O2
all: linux

# Make debug..
debug: CFLAGS += -g -DDEBUG
debug: linux

# Faux target..
linux: LINKLIB = $(LIBRARY).so
linux: GTK3 = `pkg-config --cflags --libs gtk+-3.0`
linux: PACKAGES = $(GTK3) -lgthread-2.0
linux: CFLAGS += $(PACKAGES)
linux: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(LINKLIB) $(OBJECTS) $(PACKAGES)

# Examples..
examples: GTK3 = `pkg-config --cflags --libs gtk+-3.0`
examples: compile-ex

########################################################################
# Standard Windows build: MSYS, GTK+3
#
# @usage: mingw32-make [ mingw32-[ make | debug | clean | examples ] ]
#
########################################################################

.PHONY : mingw32-make mingw32-debug mingw32-clean mingw32-examples windows

# MinGW release..
mingw32-make: WINDOWS = -mwindows
mingw32-make: CFLAGS += -O2 
mingw32-make: windows

# MinGW debug..
mingw32-debug: WINDOWS = 
mingw32-debug: CFLAGS += -g -DDEBUG
mingw32-debug: windows

# MinGW make..
windows: LINKLIB = $(LIBRARY).dll
windows: GTK3 = $(shell pkg-config.exe --libs --cflags gtk+-win32-3.0)
windows: PACKAGES = $(GTK3)
windows: CFLAGS += $(PACKAGES)
windows: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(LINKLIB) $(OBJECTS) $(PACKAGES) $(WINDOWS)

# MinGW clean..
mingw32-clean: clean

# MinGW examples..
mingw32-examples: EXT = .exe
mingw32-examples: GTK3 = $(shell pkg-config.exe --libs --cflags gtk+-win32-3.0)
mingw32-examples: compile-ex

########################################################################
# Shared targets..
#
########################################################################

.PHONY : dist purge compile-ex

clean:
	-@rm $(DELFILE) 2>/dev/null || echo "it's clean"

purge:
	-@find ./ -regex ".*\(\.swp\|\.swo\|~\|\.fuse.*\)" -delete

# Target to compile examples..
compile-ex: PACKAGES = $(GTK3) -L. -lgtk_custom_table
compile-ex: CFLAGS = -Wall -Iinclude
compile-ex: $(EXOFILE)
compile-ex: 
	-@cp *.dll *.so $(EXAMDIR) 2>/dev/null || true

########################################################################
# Project targets..
#
########################################################################

%: %.c
	$(CC) $(CFLAGS) $< -o $@$(EXT) $(PACKAGES)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

-include $(DDFILES)

