/*****************************************************************
* Name:    keypad.h
*
* Creator: Frank Wallis
* Purpose: Header file for keypad.h
*
* History: 06/10/2009	FW	Created.
******************************************************************/
#ifndef KEYPAD_H
#define KEYPAD_H

#define KEYPAD_WIDTH 3
#define KEYPAD_HEIGHT 4

extern char KeyPad[KEYPAD_WIDTH][KEYPAD_HEIGHT];

extern coor key_to_square(char key);
extern char key_for_square(coor *square);
extern int contains_digit(coor *square);

#endif