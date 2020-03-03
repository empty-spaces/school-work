> Clément LEFEVRE, Nicolas BLOT
> Encadré par Mme EMAD
> IATIC 5 2019/2020
# Projet HPC & Simulation - Étude de la propagation d'une épidémie par l'approche PageRank

## 1. Pré-requis
1.  [Installer Python 3]([https://www.python.org/downloads/](https://www.python.org/downloads/))
2. Installation des dépendances du projet :   
 `make install`


## 2.  Exécution
#### Exécution pour un graphe donné 
- Exécutez la commande suivante :    
`python3 src/main.py -f "path/to/graphfile"`

- Il est également possible de spécifier les paramètres au programme (alpha, epsilon, taux de contamination, de vaccination, etc.). Exécutez la commande suivante pour afficher l'aide : 
`make help`

#### Exécution pour plusieurs graphes
1. Placez les graphes désirés dans le dossier `graphs` du projet
2. Exécutez la commande suivante :    
`make run`

___
> Clément LEFEVRE, Nicolas BLOT
> Encadré par Mme EMAD
> IATIC 5 2019/2020