#!/bin/sh
#
# Name:    test.sh
#
# Creator: Frank Wallis
# Purpose: Regression tests chesspad program against a set of previous outputs
#          Run with the 'create' parameter to create the master files to 
#          compare against. 
#          All files are created in ./test directory
# 
# History: 08/10/2009	FW	Created.
#
if [ $# -gt 0 ] && [ $1 = 'create' ];
then
	extension=".master";
else
	extension=".test";
fi

if [ ! -d "test" ]
then
	mkdir test;
fi

# Write the testing files
for piece in king queen bishop knight rook pawn
do
	for (( key=0; key < 10; key++ ))
	do
		eval filename="./test/${piece}_${key}${extension}";
		./chesspad $piece $key 5 > $filename;
	done
done

if [ $# -gt 0 ] && [ $1 = 'create' ];
then
	echo "Output master files"
else
	# Compare with the master files
	for piece in king queen bishop knight rook pawn
	do
		echo "Testing $piece's moves: ";
		eval failed=0;
		
		for (( key = 0; key < 10; key++ ))
		do
			if cmp -s ./test/${piece}_${key}.test ./test/${piece}_${key}.master
			then
				echo -n "";
			else
				eval failed=1;
				echo "$key failed";
			fi
		done	
		
		if [ $failed = 1 ];
		then
			echo "$piece failed"
		else
			echo "$piece passed"
		fi
	done
fi
