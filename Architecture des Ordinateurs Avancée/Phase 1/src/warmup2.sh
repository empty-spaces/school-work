#!/bin/bash

# Calcule l'indicateur de dispertion (médiane - min) / min
# Pour le warmup

NB_REPETS=45
VAL=3.14
TAILLE_TABS=25000 # Pour travailler en L2

echo "Nombre de warmups;Ecart à la médiane" > data2.csv

for NB_WARMUPS in `seq 0 20000 1000000`;
do
	echo "$NB_WARMUPS warmups"
	printf "$NB_WARMUPS;" >> data2.csv
	taskset -c 0 ./baseline $TAILLE_TABS $NB_WARMUPS $NB_REPETS $VAL | sed 's/[^0-9\.]*//g' | sort -n | awk '
	BEGIN {
		c = 0;
		sum = 0;
	}
	$1 ~ /^[0-9]*(\.[0-9]*)?$/ {
		a[c++] = $1;
		sum += $1;
	}
	END {
		ave = sum / c;
		if( (c % 2) == 1 ) {
			median = a[ int(c/2) ];
		} else {
			median = ( a[c/2] + a[c/2-1] ) / 2;
		}
		OFS="\t";
		print ( median - a[0] ) / a[0];
	}
	' >> data2.csv
done

