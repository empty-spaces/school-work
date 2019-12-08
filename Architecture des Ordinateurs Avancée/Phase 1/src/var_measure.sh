#!/bin/bash

if [ -z "$1" ]
then
	echo "Usage : ./measure <namefile>"
	exit
fi

FILE="$1".csv
NB_WARMUPS=500000
NB_REPETS=45
VAL=3.14
BORNE1=15000
BORNE2=30000
STEP=1000

echo "Taille tableaux" > $FILE
for TAILLE_TABS in `seq $BORNE1 $STEP $BORNE2`;
do
	echo "Taille : $TAILLE_TABS"
	printf "$TAILLE_TABS;" >> $FILE
	taskset -c 0 ./baseline $TAILLE_TABS $NB_WARMUPS $NB_REPETS $VAL | sed 's/[^0-9\.]*//g' | tr '\n' ';' >> $FILE
	echo >> $FILE
done
