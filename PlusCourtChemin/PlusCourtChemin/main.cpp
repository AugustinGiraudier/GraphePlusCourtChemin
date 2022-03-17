#include <iostream>
#include <vector>
#include <algorithm>
#include "Graphe.h"

int main() {

	const std::string GRAPHES_ROOT = "../../InputGraphes/";
	const unsigned int SOMMET_DEPART = 225;
	const unsigned int SOMMET_ARRIVEE = 4015;
	const unsigned int MAX_AFFICHAGE = 25;


	//Graphe G = Graphe(GRAPHES_ROOT + "CommunesFrance_5000.tgoGraph");
	//Graphe G = Graphe(GRAPHES_ROOT + "Graphe_01.tgoGraph");
	Graphe G = Graphe(GRAPHES_ROOT + "CommunesFrance_5000coord.tgoGraph");

	//G.Afficher(MAX_AFFICHAGE);

	/*
	std::vector<double> vecRes = G.DikstraAll(SOMMET_DEPART);

	std::cout << std::endl << "Affichage resultats Dikstra All :" << std::endl;
	for (unsigned int i = 0; i < std::min((unsigned int)vecRes.size(), MAX_AFFICHAGE); i++) {
		std::cout << SOMMET_DEPART << " -> " << i << " : " << vecRes[i] << std::endl;
	}
	*/

	
	double dist = G.Dikstra(SOMMET_DEPART, SOMMET_ARRIVEE);
	std::cout << std::endl << "Affichage resultats Dikstra : " << std::endl;
	std::cout << "de " << SOMMET_DEPART << " a " << SOMMET_ARRIVEE << " : " << dist;
	
	return 0;
}
