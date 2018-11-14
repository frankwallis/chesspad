/*****************************************************************
* Name:    chess_moves.c
*
* Creator: Frank Wallis
* Purpose: Returns the available moves for a chess piece and square.
*          Caches previously calculated results in a lookup table.
*
* History: 06/10/2009	FW	Created.
******************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "stdtypes.h"
#include "chess_moves.h"

/* prototypes */
void initialise_board(int width, int height);
void free_board(void);
available_squares_rec *get_available_squares(coor *start_square, int piece);
int str_to_piece(char *str);

int get_lookup_index(coor *square, int piece);
void populate_current_available(coor *start_square, int piece);

void add_king_moves(coor *start_square);
void add_queen_moves(coor *start_square);
void add_bishop_moves(coor *start_square);
void add_rook_moves(coor *start_square);
void add_pawn_moves(coor *start_square);
void add_pawn_special_moves(coor *start_square);
void add_knight_moves(coor *start_square);

void add_diag_moves(coor *start_square, int max_hops);
void add_perp_moves(coor *start_square, int max_hops);
void add_vector_moves(coor *start_square, int dx, int dy, int max_hops);
void add_square(coor *square);

/* globals */

/* lookup table of available squares for each starting square and piece */
static available_squares_rec *g_lookup = NULL;
static size_t g_lookup_size = 0;

/* current board dimensions */
static int g_board_width;
static int g_board_height;

/* current lookup record being populated */
static available_squares_rec *g_current_available;

/*
	void initialise_board( int width, int height )
	sets the current board size and clears the lookup table
*/
void initialise_board(int width, int height)
{
	/* clear the current lookup */
	free_board();

	/* set the board dimensions */
	g_board_width = width;
	g_board_height = height;

	/* and allocate a new lookup table */
	g_lookup_size = g_board_width * g_board_height * NUM_PIECES;
	g_lookup = malloc(g_lookup_size * sizeof(available_squares_rec));
	memset(g_lookup, 0, g_lookup_size * sizeof(available_squares_rec));

	return;
}

/* 
	void free_board( void )
	clears the in-memory lookup
*/
void free_board(void)
{
	int i;

	if (g_lookup != NULL)
	{
		for (i = 0; i < g_lookup_size; i++)
		{
			if (g_lookup[i].squares != NULL)
				free(g_lookup[i].squares);
		}

		free(g_lookup);
		g_lookup = NULL;
	}
	return;
}

/*
	available_squares_rec get_available_squares( coor * start_square, int piece );
	returns a record containing a list of available moves for this piece from start_square 
	start_square must be a position on the board previously initialised
	piece must be a valid member of the chess_piece enumeration.
*/
available_squares_rec *get_available_squares(coor *start_square, int piece)
{
	int lookup_index = get_lookup_index(start_square, piece);
	g_current_available = g_lookup + lookup_index;

	if (g_current_available->squares == NULL)
	{
		g_current_available->squares = malloc(g_board_width * g_board_height * sizeof(coor));
		populate_current_available(start_square, piece);
	}

	return g_current_available;
}

/*
	int get_lookup_index( coor square, int piece )
	returns the index into the lookup to find the moves available to this
	piece from this square
*/
int get_lookup_index(coor *square, int piece)
{
	return (((g_board_height * square->x) + square->y) * NUM_PIECES) + piece;
}

/*
	void populate_current_available( coor * start_square, int piece );
	populates the lookup table with a list of squares which are available
	for this piece from this square
*/
typedef void(add_moves_fn)(coor *start_square);

void populate_current_available(coor *start_square, int piece)
{
	/* Run the add_moves_fn for this piece */
	add_moves_fn *init_moves_table[] = {add_king_moves, add_queen_moves,
										add_bishop_moves, add_knight_moves,
										add_rook_moves, add_pawn_moves,
										add_pawn_special_moves};
	init_moves_table[piece](start_square);

	return;
}

/*
	void add_king_moves(coor * start_square)
	populates g_current_available with the available moves 
	for the king from start_square
*/
void add_king_moves(coor *start_square)
{
	add_perp_moves(start_square, 1);
	add_diag_moves(start_square, 1);
	return;
}

/*
	void add_queen_moves(coor * start_square)
	populates g_current_available with the available moves 
	for the queen from start_square
*/
void add_queen_moves(coor *start_square)
{
	add_perp_moves(start_square, INFINITE);
	add_diag_moves(start_square, INFINITE);
	return;
}

/*
	void add_bishop_moves(coor * start_square)
	populates g_current_available with the available moves 
	for the bishop from start_square
*/
void add_bishop_moves(coor *start_square)
{
	add_diag_moves(start_square, INFINITE);
	return;
}

/*
	void add_rook_moves(coor * start_square)
	populates g_current_available with the available moves 
	for the rook from start_square
*/
void add_rook_moves(coor *start_square)
{
	add_perp_moves(start_square, INFINITE);
	return;
}

/*
	void add_pawn_moves(coor * start_square)
	populates g_current_available with the available moves 
	for the pawn from start_square
*/
void add_pawn_moves(coor *start_square)
{
	add_vector_moves(start_square, 0, 1, 1);
	return;
}

/*
	void add_pawn_special_moves(coor * start_square)
	populates g_current_available with the available moves 
	for a pawn (which has not yet moved) from start_square
*/
void add_pawn_special_moves(coor *start_square)
{
	add_vector_moves(start_square, 0, 1, 2);
	return;
}

/*
	void add_knight_moves(coor * start_square)
	populates g_current_available with the available moves
	for the knight from start_square
*/
void add_knight_moves(coor *start_square)
{
	add_vector_moves(start_square, 2, 1, 1);
	add_vector_moves(start_square, 2, -1, 1);
	add_vector_moves(start_square, -2, 1, 1);
	add_vector_moves(start_square, -2, -1, 1);
	add_vector_moves(start_square, 1, 2, 1);
	add_vector_moves(start_square, 1, -2, 1);
	add_vector_moves(start_square, -1, 2, 1);
	add_vector_moves(start_square, -1, -2, 1);
	return;
}
/*
	void add_diag_moves(coor * start_square, int max_hops)
	adds moves in all diagonal directions from the start_square
	up to a limit of max_hops in any direction
*/
void add_diag_moves(coor *start_square, int max_hops)
{
	add_vector_moves(start_square, 1, 1, max_hops);
	add_vector_moves(start_square, -1, 1, max_hops);
	add_vector_moves(start_square, 1, -1, max_hops);
	add_vector_moves(start_square, -1, -1, max_hops);
	return;
}

/*
	void add_perp_moves(coor * start_square, int max_hops)
	adds moves in all perpendicular directions from the start_square
	up to a limit of max_hops in any direction
*/
void add_perp_moves(coor *start_square, int max_hops)
{
	add_vector_moves(start_square, 1, 0, max_hops);
	add_vector_moves(start_square, 0, 1, max_hops);
	add_vector_moves(start_square, -1, 0, max_hops);
	add_vector_moves(start_square, 0, -1, max_hops);
	return;
}

/*
	void add_vector_moves(coor * start_square, int dx, int dy, int max_hops)
	adds moves with vector [dx, dy] up to a limit of max_hops times or
	until the edge of the board is reached.
*/
void add_vector_moves(coor *start_square, int dx, int dy, int max_hops)
{
	coor new_square;

	new_square.x = start_square->x;
	new_square.y = start_square->y;

	while (max_hops > 0)
	{
		new_square.x = new_square.x + dx;
		new_square.y = new_square.y + dy;

		/* Are we off the board? */
		if ((new_square.x < 0) || (new_square.x >= g_board_width))
			break;

		if ((new_square.y < 0) || (new_square.y >= g_board_height))
			break;

		/* It's a valid move for the piece so put it in the list */
		add_square(&new_square);

		/* try another hop */
		max_hops--;
	}
	return;
}

/*
	void add_square( coor * square )
	adds this square to the list of currently available ones in
	g_current_available
*/
void add_square(coor *square)
{
	g_current_available->squares[g_current_available->count].x = square->x;
	g_current_available->squares[g_current_available->count].y = square->y;
	g_current_available->count++;
	return;
}

/*
	int str_to_piece(char * str)
	converts the name of a chess piece to its enum value
	in chess_moves.h. If it is not recognised then returns -1.
*/
int str_to_piece(char *str)
{
	int i;

	/* convert to lowercase */
	for (i = 0; str[i] != '\0'; i++)
		str[i] = tolower(str[i]);

	/* and find the piece */
	if (strcmp(str, "king") == 0)
		return CP_KING;
	else if (strcmp(str, "queen") == 0)
		return CP_QUEEN;
	else if (strcmp(str, "bishop") == 0)
		return CP_BISHOP;
	else if ((strcmp(str, "knight") == 0) || (strcmp(str, "horse") == 0))
		return CP_KNIGHT;
	else if ((strcmp(str, "rook") == 0) || (strcmp(str, "castle") == 0))
		return CP_ROOK;
	else if (strcmp(str, "pawn") == 0)
		return CP_PAWN;
	else
		return -1;
}
