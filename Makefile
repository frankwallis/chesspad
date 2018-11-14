####################################################################
# Name:    Makefile
#
# Creator: Frank Wallis
# Purpose: Builds chesspad and knightspad
#
# History: 06/10/2009	FW	Created.
####################################################################
CFLAGS=-O3 -Wall -L/usr/lib/include
CC=gcc
LD=gcc

all			:	chesspad knightspad
	
chesspad	:	chesspad.o chess_moves.o keypad.o
				$(LD) chesspad.o chess_moves.o keypad.o -o chesspad
		 
chesspad.o	:	chesspad.c chesspad.h
				$(CC) $(CFLAGS) -c chesspad.c

knightspad	:	knightspad.o chess_moves.o keypad.o
		  		$(LD) knightspad.o chess_moves.o keypad.o -o knightspad

knightspad.o:	knightspad.c
				$(CC) $(CFLAGS) -c knightspad.c
	
keypad.o	: 	keypad.c keypad.h
				$(CC) $(CFLAGS) -c keypad.c
			
chess_moves.o:	chess_moves.c chess_moves.h
			  	$(CC) $(CFLAGS) -c chess_moves.c
			   
clean:
	   rm -f *.o

backup:
	    tar -cf chesspad.tar *.[ch] *.sh Makefile