#!/bin/bash

# /!\ AVANT TOUTE UTILISATION, COMMENTER LES DEUX PREMIERES LIGNES DU MAKEFILE /!\

# Mettre ses valeurs
NB_WARMUPS=500000
NB_WARMUPS_MAQAO=5000000 
NB_REPETS=45
VAL=3.14
TAILLE_TABS=25000
BORNE_INF_TABS=15000
BORNE_SUP_TABS=30000
STEP_TABS=1000

if [ ! -d "measures" ]
then
	mkdir measures
	mkdir measures/$DIRNAME
	mkdir measures/$DIRNAME/MAQAO
fi

# COMPILATION
CC=gcc
CFLAGS="-O2 -fopenmp -g -Wall"
make clean
make all CC="$CC" CFLAGS="$CFLAGS" OPT=OMP # On prend la version parallélisée

# Exécution pour un nombre de threads variant entre 1 et 5
for OMP_NUM_THREADS in `seq 1 1 5`;
do
	echo "$OMP_NUM_THREADS THREADS"
	export OMP_NUM_THREADS
	
	DIRNAME=OMP_$OMP_NUM_THREADS # Nom du répertoire
	
	if [ ! -d "measures/$DIRNAME" ]
	then
		mkdir measures/$DIRNAME
		mkdir measures/$DIRNAME/MAQAO
	fi

	# MAQAO DIR
	if [ -d "measures/$DIRNAME/MAQAO" ]
	then
		sudo rm -Rf measures/$DIRNAME/MAQAO
	fi
	
	# MAQAO
	echo "MAQAO"
	sudo maqao oneview --create-report=one binary=baseline xp=measures/"$DIRNAME"/MAQAO run_command="<binary> $TAILLE_TABS $NB_WARMUPS_MAQAO $NB_REPETS $VAL"
	echo "DONE"

	# VAR MEASURE
	echo "VAR MEASURE"
	FILE=measures/"$DIRNAME"/"$DIRNAME".csv
	echo "Taille tableaux" > $FILE
	for TAILLE_TABS in `seq $BORNE_INF_TABS $STEP_TABS $BORNE_SUP_TABS`;
	do
		echo "Taille : $TAILLE_TABS"
		printf "$TAILLE_TABS;" >> $FILE
		./baseline $TAILLE_TABS $NB_WARMUPS $NB_REPETS $VAL | sed 's/[^0-9\.]*//g' | tr '\n' ';' >> $FILE
		echo >> $FILE
	done
	echo "DONE"

done 

echo "ALL DONE"







