#!/bin/bash

if [ -z "$1" ]
then
	echo "Usage : ./measure <namefile>"
	exit
fi

FILE="$1".csv
NB_WARMUPS=10000
NB_REPETS=45
VAL=3.14
TAILLE_TABS=25000

echo "Valeur" > $FILE
taskset -c 0 ./baseline $TAILLE_TABS $NB_WARMUPS $NB_REPETS $VAL | sed 's/[^0-9\.]*//g' >> $FILE



