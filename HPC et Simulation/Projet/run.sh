#!/bin/sh

cd src 

## ALPHA = 0.85
for graph in ../graphs/* ; do
	echo "=================================================="
    echo "Executing simulation for graph $graph"
    python3 main.py -f $graph -a 0.85 -v 0.05 -c 0.25
    echo "=================================================="
done
