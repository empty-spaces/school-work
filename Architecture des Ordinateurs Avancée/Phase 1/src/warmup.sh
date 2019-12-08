# Calcule l'écart-type, indicateur de dispertion pour la détermination du warmup

#!/bin/bash

TAILLE_TABS=25000 # Pour travailler en L2
NB_REPETS=45 # Nombre de répétitions du noyau
VAL=3.14 # x


echo "Nombre de warmups;Ecart Type" > data.csv

for NB_WARMUPS in `seq 0 20000 1000000`;
do
	taskset -c 0 ./baseline $TAILLE_TABS $NB_WARMUPS $NB_REPETS $VAL | sed 's/[^0-9\.]*//g' > temp
	./ecart.sh temp $NB_WARMUPS
done

rm temp
