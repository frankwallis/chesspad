/*****************************************************************
* Name:    keypad.c
*
* Creator: Frank Wallis
* Purpose: Functions modelling a standard 3x4 telephone keypad.
*
* History: 06/10/2009	FW	Created.
******************************************************************/
#include "stdtypes.h"
#include "keypad.h"

/* prototypes */
coor key_to_square(char key);
char key_for_square(coor *square);
int contains_digit(coor *square);

/* global array of keypad characters */
char KeyPad[KEYPAD_WIDTH][KEYPAD_HEIGHT] = {{'*', '7', '4', '1'},
											{'0', '8', '5', '2'},
											{'#', '9', '6', '3'}};

/*
	coor key_to_square( char key )
	converts a keypad character to a square on the board
	if the key is not found then returns [ -1, -1 ]
*/
coor key_to_square(char key)
{
	int i, j;
	coor result = {-1, -1};

	for (i = 0; i < KEYPAD_WIDTH; i++)
	{
		for (j = 0; j < KEYPAD_HEIGHT; j++)
		{
			if (KeyPad[i][j] == key)
			{
				result.x = i;
				result.y = j;
				break;
			}
		}
	}
	return result;
}

/*
	int contains_digit( coor * square ) 
	'*' and '#' are not valid digits in a phone number
	this method has been optimised for the known keypad
	layout.
*/
int contains_digit(coor *square)
{
	/* [ 0, 0 ] and [ 0, 2 ] are the only invalid squares */
	return ((square->y != 0) || (square->x == 1));
}

/*
	char key_for_square( coor * square )
	returns the character found on this square on the keypad
	square must be valid location on keypad.
*/
char key_for_square(coor *square)
{
	return KeyPad[square->x][square->y];
}
