########################################################################
#
# Copyright (C) 2014  Adrian Solumsmo
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
LINKDLL = gtk_custom_table.dll
LINKLIB = gtk_custom_table_dll.a
VERSION = 1.0.0
DELFILE = $(LINKDLL) $(LINKLIB) *.o $(OBJECTS) $(DDFILES)
CFLAGS  = -c -Wall -Wno-unused-local-typedefs -MMD -MP -Isrc -Ilib
FINDDIR = src lib
LDFLAGS = -shared -Wl,--as-needed\
          -Wl,-no-undefined,--enable-runtime-pseudo-reloc,--out-implib,$(LINKLIB)
CC      = gcc

# Program source and object files..
CCFILES = $(shell find $(FINDDIR) -name "*.c")
OBJECTS = $(patsubst %.c,%.o,$(CCFILES))
DDFILES = $(patsubst %.o,%.d,$(OBJECTS))

########################################################################
# Standard Linux build..
#
# make | make [ debug | clean ]
#
########################################################################

.PHONY : all debug linux

# Default for make..
all: CFLAGS += -O2 -DINSTALL
all: linux

# Make debug..
debug: CFLAGS += -g -DDEBUG
debug: linux

# Faux target..
linux: OS = LINUX
linux: GTK3 = `pkg-config --cflags --libs gtk+-3.0`
linux: PACKAGES = $(GTK3) -lgthread-2.0
linux: CFLAGS += $(PACKAGES)
linux: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(LINKDLL) $(OBJECTS) $(PACKAGES)

########################################################################
# Standard Windows build: MSYS, GTK+3
#
# @usage: mingw32-make [ mingw32-[ make | debug | clean ] ]
#
########################################################################

.PHONY : mingw32-make mingw32-debug mingw32-clean windows

# MinGW release..
mingw32-make: WINDOWS = -mwindows
mingw32-make: CFLAGS += -O2 
mingw32-make: windows

# MinGW debug..
mingw32-debug: WINDOWS = 
mingw32-debug: CFLAGS += -g -DDEBUG
mingw32-debug: windows

# MinGW make..
windows: OS = WINDOWS
windows: GTK3 = $(shell pkg-config.exe --libs --cflags gtk+-win32-3.0)
windows: PACKAGES = $(GTK3)
windows: CFLAGS += $(PACKAGES)
windows: $(OBJECTS)
	$(CC) $(LDFLAGS) -o $(LINKDLL) $(OBJECTS) $(PACKAGES) $(WINDOWS)

# MinGW clean..
mingw32-clean: clean

########################################################################
# Shared targets..
#
########################################################################

.PHONY : dist purge

clean:
	-@rm $(DELFILE) 2>/dev/null || echo "it's clean"

purge:
	-@find ./ -regex ".*\(\.swp\|\.swo\|~\|\.fuse.*\)" -delete

########################################################################
# Project targets..
#
########################################################################

.PHONY : totem
totem: GTK3 = $(shell pkg-config.exe --libs --cflags gtk+-win32-3.0)
totem: LINK = $(GTK3) $(LINKDLL)
totem:
	$(CC) -c -Isrc main.c $(LINK)
	$(CC) -o main.exe main.o $(LINK)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

-include $(DDFILES)
