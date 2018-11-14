#!/bin/sh
#
# Name:    growth.sh
#
# Creator: Frank Wallis
# Purpose: Outputs combinations vs number length for piece and square
#
# History: 08/10/2009	FW	Created.
#
if [ $# -lt 2 ]
then
	echo "Usage: $0 <piece> <start_key>"
else
	echo "Showing combinations vs number length for $1 starting on $2"
	
	for (( i = 1; i <= 15; i++ ))
	do	
		echo -n "Length  $i: " 
		./chesspad $1 $2 $i 1
	done
fi
