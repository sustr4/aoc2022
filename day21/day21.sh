#!/bin/bash

cp input.txt outp.$$

while [ true ]; do

	mv outp.$$ inp.$$

	# New patterns
	cat inp.$$ | grep '^....: [0-9][0-9]*$' | sed 's/^\(....\): \([0-9]*\)/s\/ \1\/ \2\//' > sed.$$

	# Run patterns
	sed -i -f sed.$$ inp.$$

	# Operations
	cat inp.$$ | awk '{
		if($2 ~ /[0-9]/ && $4 ~ /[0-9]/) {
			switch($3) {
				case "+":
					print $1 " " $2 + $4;
					break;
				case "-":
					print $1 " " $2 - $4;
					break;
				case "*":
					print $1 " " $2 * $4;
					break;
				case "/":
					print $1 " " $2 / $4;
					break;
			}
		}
		else print $0;
	}' > outp.$$

	#cat outp.$$

	grep root outp.$$

done
