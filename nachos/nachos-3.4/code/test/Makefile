# use normal make for this Makefile
#
# Makefile for building user programs to run on top of Nachos
#
# Several things to be aware of:
#
#    Nachos assumes that the location of the program startup routine (the
# 	location the kernel jumps to when the program initially starts up)
#       is at location 0.  This means: start.o must be the first .o passed 
# 	to ld, in order for the routine "Start" to be loaded at location 0
#

# if you are cross-compiling, you need to point to the right executables
# and change the flags to ld and the build procedure for as

GCCDIR = ../../../gnu-decstation-ultrix/decstation-ultrix/2.95.3/

LDFLAGS = -T script -N
ASFLAGS = -mips2
CPPFLAGS = $(INCDIR)

# if you aren't cross-compiling:
# GCCDIR =
# LDFLAGS = -N -T 0
# ASFLAGS =
# CPPFLAGS = -P $(INCDIR)

PATH = $(GCCDIR):/lib:/usr/bin:/bin

CC = $(GCCDIR)gcc -B../../../gnu-decstation-ultrix/
AS = $(GCCDIR)as
LD = $(GCCDIR)ld

CPP = gcc -E
INCDIR =-I../userprog -I../threads
CFLAGS = -G 0 -c $(INCDIR)

all: halt shell matmult sort createfile basic_io basic_io_string basic_io_char basic_io_int help bubble_sort ascii test_IO_file

start.o: start.s ../userprog/syscall.h
	$(CPP) $(CPPFLAGS) start.c > strt.s
	$(AS) $(ASFLAGS) -o start.o strt.s
	rm strt.s

halt.o: halt.c
	$(CC) $(CFLAGS) -c halt.c
halt: halt.o start.o
	$(LD) $(LDFLAGS) start.o halt.o -o halt.coff
	../bin/coff2noff halt.coff halt

shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c
shell: shell.o start.o
	$(LD) $(LDFLAGS) start.o shell.o -o shell.coff
	../bin/coff2noff shell.coff shell

sort.o: sort.c
	$(CC) $(CFLAGS) -c sort.c
sort: sort.o start.o
	$(LD) $(LDFLAGS) start.o sort.o -o sort.coff
	../bin/coff2noff sort.coff sort

matmult.o: matmult.c
	$(CC) $(CFLAGS) -c matmult.c
matmult: matmult.o start.o
	$(LD) $(LDFLAGS) start.o matmult.o -o matmult.coff
	../bin/coff2noff matmult.coff matmult
	
createfile.o: createfile.c
	$(CC) $(CFLAGS) -c createfile.c
createfile: createfile.o start.o
	$(LD) $(LDFLAGS) start.o createfile.o -o createfile.coff
	../bin/coff2noff createfile.coff createfile

basic_io.o: basic_io.c
	$(CC) $(CFLAGS) -c basic_io.c
basic_io: basic_io.o start.o
	$(LD) $(LDFLAGS) start.o basic_io.o -o basic_io.coff
	../bin/coff2noff basic_io.coff basic_io

basic_io_string.o: basic_io_string.c
	$(CC) $(CFLAGS) -c basic_io_string.c
basic_io_string: basic_io_string.o start.o
	$(LD) $(LDFLAGS) start.o basic_io_string.o -o basic_io_string.coff
	../bin/coff2noff basic_io_string.coff basic_io_string

basic_io_char.o: basic_io_char.c
	$(CC) $(CFLAGS) -c basic_io_char.c
basic_io_char: basic_io_char.o start.o
	$(LD) $(LDFLAGS) start.o basic_io_char.o -o basic_io_char.coff
	../bin/coff2noff basic_io_char.coff basic_io_char

basic_io_int.o: basic_io_int.c
	$(CC) $(CFLAGS) -c basic_io_int.c
basic_io_int: basic_io_int.o start.o
	$(LD) $(LDFLAGS) start.o basic_io_int.o -o basic_io_int.coff
	../bin/coff2noff basic_io_int.coff basic_io_int



help.o: help.c
	$(CC) $(CFLAGS) -c help.c
help: help.o start.o
	$(LD) $(LDFLAGS) start.o help.o -o help.coff
	../bin/coff2noff help.coff help

bubble_sort.o: bubble_sort.c
	$(CC) $(CFLAGS) -c bubble_sort.c
bubble_sort: bubble_sort.o start.o
	$(LD) $(LDFLAGS) start.o bubble_sort.o -o bubble_sort.coff
	../bin/coff2noff bubble_sort.coff bubble_sort

ascii.o: ascii.c
	$(CC) $(CFLAGS) -c ascii.c
ascii: ascii.o start.o
	$(LD) $(LDFLAGS) start.o ascii.o -o ascii.coff
	../bin/coff2noff ascii.coff ascii


test_IO_file.o: test_IO_file.c
	$(CC) $(CFLAGS) -c test_IO_file.c
test_IO_file: test_IO_file.o start.o
	$(LD) $(LDFLAGS) start.o test_IO_file.o -o test_IO_file.coff
	../bin/coff2noff test_IO_file.coff test_IO_file