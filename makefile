
# makefile to build library and testdriver
.SUFFIXES:.cc
.cc.o:
	$(CC) $(INCLUDES) $(CFLAGS) -c $< -o $@

# assign your compiler name to CC if not g++
CC = g++
# where your header files are (. is same directory as the makefile)
INCLUDES=-I.
# turn on debugging symbols
CFLAGS=-g
# where your object files are (. is the same directory as the makefile)
LDFLAGS=-L.

# when "make" is typed at command line with no args "default" will get executed and do "all"
default: 	all

#
# assign source and headers for our "utility" lib to variables below
#
# replace "SBB" (my initials) with yours or use the same filenames
LIB_UTIL_SRC = SBB_util.cc
LIB_UTIL_SRC_HDR = SBB_util.h
LIB_DATE_SRC = SBB_date.cc
LIB_DATE_SRC_HDR = SBB_date.h
LIB_IO_SRC = SBB_io.cc
LIB_IO_SRC_HDR = SBB_io.h
LIB_CALC_SRC = Calculator.cc
LIB_CALC_SRC_HDR = Calculator.h
#
LIB_UTIL_OBJ = $(LIB_UTIL_SRC:.cc=.o)
$(LIB_UTIL_OBJ) : $(LIB_UTIL_SRC_HDR)
LIB_UTIL_TARGET = libutil.a

LIB_DATE_OBJ = $(LIB_DATE_SRC:.cc=.o)
$(LIB_DATE_OBJ) : $(LIB_DATE_SRC_HDR)

LIB_IO_OBJ = $(LIB_IO_SRC:.cc=.o)
$(LIB_IO_OBJ) : $(LIB_IO_SRC_HDR)

LIB_CALC_OBJ = $(LIB_CALC_SRC:.cc=.o) $(LIB_DATE_OBJ)
$(LIB_CALC_OBJ) : $(LIB_CALC_SRC_HDR)

# 
# the file where main() is
MAIN_SRC = mmain.cc
MAINOBJ = $(MAIN_SRC:.cc=.o) 
$(MAINOBJ) : $(LIB_UTIL_TARGET) $(LIB_CALC_OBJ)
MAINTARGET=price_calc

ARCHIVE_OPTIONS = rucsv

$(LIB_UTIL_TARGET) : $(LIB_UTIL_OBJ) $(LIB_DATE_OBJ) $(LIB_IO_OBJ)
	ar $(ARCHIVE_OPTIONS) $(LIB_UTIL_TARGET) $(LIB_UTIL_OBJ) $(LIB_DATE_OBJ) $(LIB_IO_OBJ)

$(MAINTARGET) : $(MAINOBJ) $(LIB_UTIL_TARGET)
	$(CC) $(LDFLAGS) $(MAINOBJ) $(LIB_UTIL_TARGET) $(LIB_CALC_OBJ) -o $(MAINTARGET)

all : util test
	rm -f  *.a

test: $(MAINTARGET)

util: $(LIB_UTIL_TARGET)

clean:
	rm -f *.o 
	rm -f $(MAINTARGET)

clobber:
	-rm -f *.o *.a $(MAINTARGET)

backup: clobber
	tar cvf ./nyu.tar *.*

