# Makefile for test

PROJECT		= ConfLite
DESCRIPTION	= A sqlite solution to system/user configure
VERSION		= 0.0.1

INCLUDES	+= ../include ../build
SRCS		+= .
SOURCES		= $(shell find $(SRCS) -iregex ".*\.c\(c\|pp\|++\|xx\)")
LIBS		+= conflite
LDPATH		+= ../build
LDFLAGS		+= $(LDPATH:%=-L%)
CFLAGS		+= `pkg-config --cflags sqlite3`
LDFLAGS		+= `pkg-config --libs sqlite3`
#MACROS		+= _GLIBCXX_USE_NANOSLEEP

include ../makefiles/predef.mk

all: $(patsubst %.cpp, %, $(SOURCES))
#ts-server ts-client ts-proxy

%: %.cpp
	@echo
	@echo "===Generating $(BUILD)/test/$@===>"
	$(MKDIR) $(dir $(BUILD)/test/$@)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(INCLUDES:%=-I%) $(MACROS:%=-D%) $(LIBS:%=-l%) -o $(BUILD)/test/$@ $^
	
