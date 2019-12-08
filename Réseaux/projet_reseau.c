/** LEFEVRE Clément **\
	VERDURE Pierre 
	   IATIC 4 
\** Projet Réseau **/
		
/** LIBRAIRIES **/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

/** MACRO-CONSTANTES **/
#define NUM_OF_SPREADING_FACTOR 3
#define NUM_OF_CHANNEL 2
#define NUM_OF_RESOURCES NUM_OF_SPREADING_FACTOR * NUM_OF_CHANNEL
#define NUM_OF_ITERATIONS 20

/** STRUCTURES **/

// 1. Représentation d'une ressource 
typedef struct {
	int id; // Identifiant
	int channel; // Canal
	int spreadingFactor; // Facteur d'étalement
	float averageReward; // Gain moyen
	int nbPlays; // Nombre de lancement effectués
} resource;

// 2. Représentation d'un équipement
typedef struct {
	int id; // Identifiant
	int position; // Position
	resource* reachablesResources; // Ressources disponibles
	int nbReachablesResources; // Nombre de ressources
	int totalPlays; // Total de lancers
	int idResourceToPlay; // L'identifiant de la ressource à jouer
} equipment;


/** HEADERS **/
void init_resource(int id, int channel, int spreadingFactor, resource* r);
void init_equipment(int id, equipment* e);
void set_reachables_resources(resource* r, equipment* e, int nbEquipments);
void ucb1(resource* r, equipment* e, int nbEquipments);
void play_resource(equipment* e, bool isCollision);
void print_equipments(equipment* e, int nbEquipments);
void print_resource(resource* r);
int select_resource_that_maximise(equipment e, int currentlyUsedResources[]);
void release_resources(int currentlyUsedResources[]);
bool detect_collision(equipment e, int currentlyUsedResources[]);
float avg_reward(equipment* e, int nbEquipments);
int select_random_resource(equipment e);
void random_use(resource* r, equipment* e, int nbEquipments);
void reset_equipments(equipment* e, int nbEquipments);
float small_random_value();
void print_infos(equipment* e, int nbEquipments);


int nbCollisions = 0;
int nbNonCollisions = 0;

/** MAIN **/
int main(int argc, char* argv[]) {
	
	// Initialisation du rand 
	srand(time(NULL));

	// Vérification des arguments
	if(argc < 2) {
		printf("Erreur, veuillez saisir le nombre d'équipements pour la simulation.\n");
		exit(EXIT_FAILURE);
	}

	// Génération des ressources (couple Facteur d'étalement - Canal)
	resource* r = (resource*) malloc(NUM_OF_RESOURCES * sizeof(resource));
	int id = 0;
	for(int i = 0; i < NUM_OF_CHANNEL; i++) {
		for(int j = 0; j < NUM_OF_SPREADING_FACTOR; j++) {
			init_resource(id,i,j,&r[id]);
			id++;
		}
	}

	// Génération des équipements (nombre variant de 5 à 10 en paramètres)
	int nbEquipments = atoi(argv[1]);
	equipment* e = (equipment*) malloc(nbEquipments * sizeof(equipment));
	for(int i = 0; i < nbEquipments; i++) {
		init_equipment(i,&e[i]);
	}


	// Lancement d'UCB1
	ucb1(r,e,nbEquipments);

	// Reinitialise les informations des équipements
	reset_equipments(e,nbEquipments);

	// Lancement de l'utilisation aléatoire
	random_use(r,e,nbEquipments);
	

	// Libération des ressources
	for(int i = 0; i < nbEquipments; i++) {
		free(e[i].reachablesResources);
	}
	free(e);
	free(r);

	return 0;
}



/** FONCTIONS **/

// Initialise la ressource 
void init_resource(int id, int channel, int spreadingFactor, resource* r) {
	r->id = id;
	r->channel = channel;
	r->spreadingFactor = spreadingFactor;
	r->averageReward = 0;
	r->nbPlays = 0;
}

// Initialise l'équipement
void init_equipment(int id, equipment* e) {
	e->id = id;
	e->position = rand() % 3;
	e->reachablesResources = (resource*) malloc(1 * sizeof(resource)); // Tableau contenant les machines atteignables
	e->nbReachablesResources = 0;
	e->totalPlays = 0;
}

// Ajoute aux équipements les ressources accessibles (qu'ils peuvent utiliser en fonction de leur position)
void set_reachables_resources(resource* r, equipment* e, int nbEquipments) {
	for(int i = 0; i < nbEquipments; i++) {
		for(int j = 0; j < NUM_OF_RESOURCES; j++) {
			if(r[j].spreadingFactor >= e[i].position) { // Les équipements dont la position est en dessous de Rk ne peuvent utiliser que les facteurs d'étalement SFi avec i = k..2 
				e[i].reachablesResources = realloc(e[i].reachablesResources, (e[i].nbReachablesResources + 1) * sizeof(resource));
				e[i].reachablesResources[e[i].nbReachablesResources] = r[j];
				e[i].nbReachablesResources ++;
			}
		}
	}
}

// Algorithme UCB1
void ucb1(resource* r, equipment* e, int nbEquipments) {
	set_reachables_resources(r,e,nbEquipments);
	int currentlyUsedResources[NUM_OF_RESOURCES] = {0}; // Pour les collisions
		
	// Initialisation --> Chaque équipement joue chaque ressource qu'il peut atteindre
	for(int i = 0; i < nbEquipments; i++) {
		for(int j = 0; j < e[i].nbReachablesResources; j++) {
			e[i].idResourceToPlay = j;
			play_resource(&e[i],false);
		}
	}

	// Loop --> Jouer la machine qui maximise
	for(int i = 0; i < NUM_OF_ITERATIONS; i++) { // Itérations
		for(int j = 0; j < nbEquipments; j++) { // Sélection
			e[j].idResourceToPlay = select_resource_that_maximise(e[j],currentlyUsedResources);
			currentlyUsedResources[e[j].reachablesResources[e[j].idResourceToPlay].id] ++;
		}
		for(int j = 0; j < nbEquipments; j++) { // Action
			play_resource(&e[j],detect_collision(e[j],currentlyUsedResources));
		}
		release_resources(currentlyUsedResources);
	}

	printf("Gain moyen (UCB1) = %f\n",avg_reward(e,nbEquipments));
	print_infos(e,nbEquipments);
}

// Joue la ressource selectionnée
void play_resource(equipment* e, bool isCollision) {
	// Récompense en fonction du SF --> 1/7 si SF0, 1/8 si SF1, 1/9 si SF2
	float reward = 1 / (float) (e->reachablesResources[e->idResourceToPlay].spreadingFactor + 7);

	// Si collision --> Récompense = 0
	if(isCollision) 
		reward = 0;
	
	e->reachablesResources[e->idResourceToPlay].nbPlays ++;
	// Calcul du nouveau gain moyen
	e->reachablesResources[e->idResourceToPlay].averageReward = ((float)(e->reachablesResources[e->idResourceToPlay].nbPlays - 1) / (float) e->reachablesResources[e->idResourceToPlay].nbPlays) * e->reachablesResources[e->idResourceToPlay].averageReward + (1 / (float) e->reachablesResources[e->idResourceToPlay].nbPlays) * reward;
	e->totalPlays ++;
}

// Debug (affiche les informations des équipements)
void print_equipments(equipment* e, int nbEquipments) {
	for(int i = 0; i < nbEquipments; i++) {
		printf("**Equipement n°%d**\n",e[i].id);
		printf("Position : %d\n",e[i].position);
		printf("Nombre de ressources accessibles : %d\n",e[i].nbReachablesResources);
		printf("Nombre total d'actions effectuées : %d\n",e[i].totalPlays);
		printf("\nRessources associées\n");
		for(int j = 0; j < e[i].nbReachablesResources; j++) {
			printf("\nRessource n°%d\n",e[i].reachablesResources[j].id);
			printf("Facteur d'étalement : %d\n",e[i].reachablesResources[j].spreadingFactor);
			printf("Canal : %d\n",e[i].reachablesResources[j].channel);
			printf("Nombre de lancers : %d\n",e[i].reachablesResources[j].nbPlays);
			printf("Gain Moyen : %f\n",e[i].reachablesResources[j].averageReward);
		}
		printf("\n\n\n");
	}
}

// Debug (affiche les informations des différentes machines)
void print_resource(resource* r) {
	for(int i = 0; i < NUM_OF_RESOURCES; i++) {
		printf("\nRessource n°%d\n",r[i].id);
		printf("Facteur d'étalement : %d\n",r[i].spreadingFactor);
		printf("Canal : %d\n",r[i].channel);
	}
}

// Selectionne la ressource de l'indice qui maximise la formule d'ucb1
int select_resource_that_maximise(equipment e, int currentlyUsedResources[]) {
	float value = 0;
	float maxValue = 0;
	int maxId;
	int* idsSelectables = malloc(1 * sizeof(int));
	int nbSelectables = 1;

	for(int i = 0; i < e.nbReachablesResources; i++) {
		// Formule d'UCB1
		value = e.reachablesResources[i].averageReward + sqrt(2 * log(e.totalPlays) / (float) e.reachablesResources[i].nbPlays) + small_random_value(); 
		if (value > maxValue) {
			maxId = i;
			maxValue = value;
		}
	}

	/* Si plusieurs valeurs maximales --> On les place dans un tableau et on en choisi une arbitrairement 
	Même si cela peut paraître déroutant, s'il s'agit d'une réelle application, les équipements ne vont pas raisonner itérativement, mais choisir une 
	valeur parmi les maximales */
	for(int i = 0; i < e.nbReachablesResources; i++) {
		value = e.reachablesResources[i].averageReward + sqrt(2 * log(e.totalPlays) / (float) e.reachablesResources[i].nbPlays);
		if(value == maxValue) {
			idsSelectables = realloc(idsSelectables, (nbSelectables + 1) * sizeof(int));
			idsSelectables[nbSelectables] = i;
			nbSelectables ++;
		}
	}

	return idsSelectables[rand() % nbSelectables];
}

// Libère les toutes les ressources (après chaque itération)
void release_resources(int currentlyUsedResources[]) {
	for(int i = 0; i < NUM_OF_RESOURCES; i++)
		currentlyUsedResources[i] = 0;
}

// Détecte une collision
bool detect_collision(equipment e, int currentlyUsedResources[]) {
	if(currentlyUsedResources[e.reachablesResources[e.idResourceToPlay].id] > 1) {
		nbCollisions ++;
		return true;
	}
	nbNonCollisions++;
	return false;
}	

// Calcule le gain de tous les équipements
float avg_reward(equipment* e, int nbEquipments) {
	float avg[nbEquipments];
	float totalAVG = 0;
	for(int i = 0; i < nbEquipments; i++) {
		avg[i] = 0;
		for(int j = 0; j < e[i].nbReachablesResources; j++) {
			avg[i] += e[i].reachablesResources[j].averageReward * e[i].reachablesResources[j].nbPlays / e[i].totalPlays;
		}
		totalAVG += avg[i];
	}
	return totalAVG;
}

// Reinitialise les valeurs des équipements (entre UCB1 et Random_use)
void reset_equipments(equipment* e, int nbEquipments) {
	for(int i = 0; i < nbEquipments; i++) {
		e[i].totalPlays = 0;
		e[i].nbReachablesResources = 0;
		free(e[i].reachablesResources);
		e[i].reachablesResources = (resource*) malloc(1 * sizeof(resource));
	}
}

// Utilisation aléatoire des ressources
void random_use(resource* r, equipment* e, int nbEquipments) {
	nbCollisions = 0;
	nbNonCollisions = 0;

	set_reachables_resources(r,e,nbEquipments);
	int currentlyUsedResources[NUM_OF_RESOURCES] = {0}; // Pour les collisions

	for(int i = 0; i < NUM_OF_ITERATIONS; i++) { // Itérations
		for(int j = 0; j < nbEquipments; j++) { // Chaque équipement
			e[j].idResourceToPlay = select_random_resource(e[j]);
			currentlyUsedResources[e[j].reachablesResources[e[j].idResourceToPlay].id] ++;
		}
		for(int j = 0; j < nbEquipments; j++) {
			play_resource(&e[j],detect_collision(e[j],currentlyUsedResources));
		}
		release_resources(currentlyUsedResources);
	}
	
	printf("Gain moyen (Random Use) = %f\n",avg_reward(e,nbEquipments));
	print_infos(e,nbEquipments);
}

// Renvoie un aléaoire correspondant à un identifiant de ressource
int select_random_resource(equipment e) {
	return rand() % e.nbReachablesResources; 
}

// Renvoie une petite valeur pour différencier le gain
float small_random_value() { // Valeur entre 0.001 et 0.0001
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = 0.001 - 0.0001;
    float r = random * diff;
    return 0.0001 + r;
}

// Affiche des informations
void print_infos(equipment* e, int nbEquipments) {
	printf("Positions = [ ");
	for(int i = 0; i < nbEquipments; i++) {
		printf("%d ",e[i].position);
	}
	printf("]\n");
	printf("Nombre de collisions : %d\n",nbCollisions);
	printf("Nombre de transmissions OK : %d\n",nbNonCollisions);
}
