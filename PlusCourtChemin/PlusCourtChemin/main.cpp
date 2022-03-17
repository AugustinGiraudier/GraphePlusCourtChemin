#include <iostream>
#include <vector>
#include <algorithm>
#include "Graphe.h"

int main() {

	const unsigned int SOMMET_DEPART = 0;
	const unsigned int MAX_AFFICHAGE = 25;
	const std::string GRAPHES_ROOT = "../../InputGraphes/";


	Graphe G = Graphe(GRAPHES_ROOT + "CommunesFrance_5000.tgoGraph");
	//Graphe G = Graphe(GRAPHES_ROOT + "Graphe_01.tgoGraph");
	G.Afficher(MAX_AFFICHAGE);

	std::vector<double> vecRes = G.Dikstra(SOMMET_DEPART);

	std::cout << std::endl << "Affichage resultats Dikstra :" << std::endl;
	for (unsigned int i = 0; i < std::min((unsigned int)vecRes.size(), MAX_AFFICHAGE); i++) {
		std::cout << SOMMET_DEPART << " -> " << i << " : " << vecRes[i] << std::endl;
	}

	return 0;
}
