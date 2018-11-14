/*****************************************************************
* Name:    chess_moves.h
*
* Creator: Frank Wallis
* Purpose: Header file for chess_moves.c
*
* History: 06/10/2009	FW	Created.
******************************************************************/
#ifndef CHESS_MOVES_H
#define CHESS_MOVES_H

#define INFINITE 0x0FFFFFFF

typedef enum
{
	CP_KING,
	CP_QUEEN,
	CP_BISHOP,
	CP_KNIGHT,
	CP_ROOK,
	CP_PAWN,
	CP_PAWN_SPECIAL
} chess_piece;

#define NUM_PIECES (CP_PAWN_SPECIAL + 1)

typedef struct
{
	coor *squares;
	int count;
} available_squares_rec;

extern void initialise_board(int width, int height);
extern void free_board();
extern available_squares_rec *get_available_squares(coor *start_square, int piece);
extern int str_to_piece(char *str);

#endif