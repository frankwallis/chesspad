/****************************************************************************
* Name:    knightspad.c
*
* Creator: Frank Wallis
* Purpose: This program outputs all the possible knight's tours on a telephone
*          keypad for a given starting key.
*
* History: 06/10/2009	FW	Created.
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stdtypes.h"
#include "keypad.h"
#include "chess_moves.h"

/* prototypes */

void output_digits(coor *current_square, int digit);
void next_move(coor *current_square, int current_digit);
int is_valid_move(coor *square, int current_digit);
void output_tour(void);
int process_args(int argc, char *argv[], coor *start_key);
void display_usage(char *program_name);

/* globals */

char g_output[16] = {};
int g_output_counter = 0;

int main(int argc, char *argv[])
{
	coor start_square;

	if (!process_args(argc, argv, &start_square))
	{
		display_usage(argv[0]);
		return 1;
	}

	/* initialise the chess_moves library */
	initialise_board(KEYPAD_WIDTH, KEYPAD_HEIGHT);

	/* iterate through all the possible phone numbers */
	output_digits(&start_square, 0);

	if (g_output_counter == 1)
		printf("No knight's tours available\n");
	else
		printf("Found %d knight's tours\n", g_output_counter);

	/* release the chess_moves library */
	free_board();

	/* We're done */
	return 0;
}

/*
	void output_digits(int current_piece, coor * current_square, int current_digit)
	follows all the available routes that the knight can take from current_square,
	and outputs the resulting phone numbers
*/
void output_digits(coor *current_square, int current_digit)
{
	/* set this key in the output string and move on to the next digit */
	g_output[current_digit] = key_for_square(current_square);
	current_digit++;

	/* if we have reached the required length then output the string 
	   otherwise continue moving around the keypad */
	if (current_digit == (KEYPAD_WIDTH * KEYPAD_HEIGHT))
		output_tour();
	else
		next_move(current_square, current_digit);

	return;
}

/*
	void next_move(coor * current_square, int current_digit)
	outputs all the remaining combinations for the current starting digits
*/
void next_move(coor *current_square, int current_digit)
{
	int i;
	available_squares_rec *available;

	/* Get a record containing the squares available to this piece from here */
	available = get_available_squares(current_square, CP_KNIGHT);

	/* And follow each of the valid ones */
	for (i = 0; i < available->count; i++)
		if (is_valid_move(available->squares + i, current_digit))
			output_digits(available->squares + i, current_digit);

	return;
}

/*
	int is_valid_move(coor * square, int current_digit)
	we cannot visit the same square twice
*/
int is_valid_move(coor *square, int current_digit)
{
	int i;
	char key;

	key = key_for_square(square);

	for (i = 0; i < current_digit; i++)
		if (g_output[i] == key)
			return FALSE;

	return TRUE;
}

/*
	void output_tour()
	outputs the completed tour
*/
void output_tour()
{
	printf("%s\n", g_output);
	g_output_counter += 1;
}

/*
	void display_usage(char * program_name)
	show accepted command line args
*/
void display_usage(char *program_name)
{
	printf("Usage %s <start_digit>\n", program_name);
}

/*
	int process_args(int argc, char* argv[], coor * start_square)
	get the starting square and the starting piece from the input parameters
	return true only if the inputs are valid
*/
int process_args(int argc, char *argv[], coor *start_square)
{
	/* We need a start square at least */
	if (argc < 2)
	{
		printf("Insufficient arguments\n");
		return FALSE;
	}

	/* Get the starting key */
	*start_square = key_to_square(argv[1][0]);

	if ((start_square->x < 0) || (start_square->y < 0))
	{
		printf("Invalid starting key");
		return FALSE;
	}

	return TRUE;
}