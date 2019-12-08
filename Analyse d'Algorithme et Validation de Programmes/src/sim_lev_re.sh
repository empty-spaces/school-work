#!/bin/bash

gcc -o distances distances.c

# Pour tester la performance de notre algorithme, nous allons calculer leur temps d'execution en
# fonction de la taille des deux mots saisis

mot1="a"
mot2="b"

echo "Taille de la chaine;Distance de Levenshtein (RE);" > lev_re.csv

for i in `seq 1 15`
do
	echo "i = $i"
	printf "$i;" >> lev_re.csv
	./distances $mot1 $mot2

	mot1+=a
	mot2+=b
done

