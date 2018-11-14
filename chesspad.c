/****************************************************************************
* Name:    chesspad.c
*
* Creator: Frank Wallis
* Purpose: This program outputs all the possible phone numbers which a chess
*           piece moving around a telephone keypad could produce.
*
* History: 06/10/2009	FW	Created.
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "stdtypes.h"
#include "keypad.h"
#include "chess_moves.h"
#include "chesspad.h"

/* prototypes */

void output_moves(int piece, coor *current_square, int digit);
void next_move(int current_piece, coor *current_square, int current_digit);
int reevaluate_piece(int current_piece, coor *current_square, int current_digit);
void write_phoneno(void);
void write_summary(time_t start_time);
int process_args(int argc, char *argv[], int *piece, coor *start_key);
void display_usage(char *program_name);

/* globals */

int g_phoneno_length = PHONENO_LENGTH_DEF;
char g_output[PHONENO_LENGTH_MAX + 1] = {};
double g_output_counter = 0;
int g_output_summary = 0;

int main(int argc, char *argv[])
{
	int start_piece;
	coor start_square;
	time_t start_time;

	/* get piece and start key */
	if (!process_args(argc, argv, &start_piece, &start_square))
	{
		display_usage(argv[0]);
		return 1;
	}

	/* initialise the chess_moves library */
	initialise_board(KEYPAD_WIDTH, KEYPAD_HEIGHT);

	/* iterate through all the possible phone numbers */
	start_time = time(NULL);
	output_moves(start_piece, &start_square, 0);

	if (g_output_summary)
		write_summary(start_time);

	/* release the chess_moves library */
	free_board();

	/* We're done */
	return 0;
}

/*
	void write_summary( time_t start_time )
	Output the number of phone numbers found and the time it took.
*/
void write_summary(time_t start_time)
{
	char duration_str[64];
	int duration = time(NULL) - start_time;

	/* work out how many seconds it took */
	if (duration < 1)
		strcpy(duration_str, "< 1 second");
	else if (duration == 1)
		strcpy(duration_str, "1 second");
	else
		sprintf(duration_str, "%d seconds", duration);

	/* write the summary */
	if (g_output_counter == 1)
		printf("Found one phone number in %s\n", duration_str);
	else
		printf("Found %.0lf phone numbers in %s\n", g_output_counter, duration_str);

	return;
}

/*
	void output_moves(int current_piece, coor * current_square, int current_digit)
	follows all the available routes that current_piece can take from current_square,
	and outputs the resulting phone numbers
*/
void output_moves(int current_piece, coor *current_square, int current_digit)
{
	/* set this key in the output string and move on to the next digit */
	g_output[current_digit] = KeyPad[current_square->x][current_square->y];

	/* if we have reached the required length then output the string 
	   otherwise continue moving around the keypad */
	if (current_digit == g_phoneno_length - 1)
		write_phoneno();
	else
		next_move(current_piece, current_square, current_digit);

	return;
}

/*
	void next_move( int current_piece, coor * current_square, int current_digit )
	outputs all the remaining combinations for the current starting digits
*/
void next_move(int current_piece, coor *current_square, int current_digit)
{
	int i;
	available_squares_rec *available;

	current_digit++;

	/* If we started with a pawn they can change into other pieces.. */
	current_piece = reevaluate_piece(current_piece, current_square, current_digit);

	/* Staying in the same place is a valid move */
	output_moves(current_piece, current_square, current_digit);

	/* Get a record containing the squares available to this piece from here */
	available = get_available_squares(current_square, current_piece);

	/* And follow each of them */
	for (i = 0; i < available->count; i++)
		if (contains_digit(available->squares + i))
			output_moves(current_piece, available->squares + i, current_digit);

	return;
}

/*
	int reevaluate_piece( int current_piece, coor current_square, int current_digit )
	Unless the piece is a pawn this will return current_piece
	When called with a pawn, will return either CP_PAWN, CP_PAWN_SPECIAL or CP_QUEEN, 
	according tothe following rules:
	- When a pawn reaches the top row it become a queen.
	- For the case where the piece is a pawn, starting on one of the bottom 2 rows,
	  the Pawn may move either one or two spaces forward the first time it moves
*/
int reevaluate_piece(int current_piece, coor *current_square, int current_digit)
{
	if (current_piece == CP_PAWN)
	{
		/* If a pawn starts on the first 2 rows it can move 2 spaces on it's
		   first move so upgrade it to CP_PAWN_SPECIAL */
		if ((current_digit == 1) && (current_square->y < 2))
			return CP_PAWN_SPECIAL;

		/* if pawn reaches final row upgrade it to queen */
		if (current_square->y == KEYPAD_HEIGHT - 1)
			return CP_QUEEN;
	}
	else if (current_piece == CP_PAWN_SPECIAL)
	{
		/* the pawn can only moves 2 spaces for it's first move at most */
		if (current_digit >= 2)
			return CP_PAWN;
	}
	return current_piece;
}

/*
	void write_phoneno()
	outputs the current phone number to stdout
*/
void write_phoneno(void)
{
	if (!g_output_summary)
		printf("%s\n", g_output);

	g_output_counter += 1;
}

/*
	void display_usage(char * program_name)
	show accepted command line args
*/
void display_usage(char *program_name)
{
	printf("Usage %s <chess_piece> <start_key> [ <phone_no_length> <summarise> ]\n", program_name);
}

/*
	int process_args(int argc, char* argv[], int * piece, coor * start_square)
	get the starting square and the starting piece from the input parameters
	return true only if the inputs are valid
*/
int process_args(int argc, char *argv[], int *piece, coor *start_square)
{
	/* We need piece and start square at least */
	if (argc < 3)
	{
		printf("Insufficient arguments\n");
		return FALSE;
	}

	/* Get the starting piece */
	*piece = str_to_piece(argv[1]);

	if (*piece == -1)
	{
		printf("Invalid starting piece\n");
		return FALSE;
	}

	/* Get the starting key */
	*start_square = key_to_square(argv[2][0]);

	if ((start_square->x < 0) || (start_square->y < 0))
	{
		printf("Invalid starting key\n");
		return FALSE;
	}

	if (!contains_digit(start_square))
	{
		printf("Invalid starting key\n");
		return FALSE;
	}

	/* for testing let us change the phone number length */
	if (argc > 3)
	{
		g_phoneno_length = atoi(argv[3]);
		if ((g_phoneno_length < 1) || (g_phoneno_length > PHONENO_LENGTH_MAX))
		{
			printf("Phone numbers must be between 1 and %d digits long\n", PHONENO_LENGTH_MAX);
			return FALSE;
		}
	}

	/* and just output a summary of the numbers found, not the numbers themselves */
	if (argc > 4)
		g_output_summary = atoi(argv[4]);

	return TRUE;
}