#include <iostream>
#include <vector>
#include <algorithm>
#include "Graphe.h"
#include <chrono>

int main() {

	const std::string GRAPHES_ROOT = "../../InputGraphes/";
	const unsigned int SOMMET_DEPART = 1054;
	const unsigned int SOMMET_ARRIVEE = 379;
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

/*
	{
		auto t0 = std::chrono::high_resolution_clock::now();
		auto vec = G.testHeuristique();
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> fs = t1 - t0;
		std::cout << "temps d'execution : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
		for(int i=0; i<10; i++)
			std::cout << vec[i] << std::endl;
	}*/
	
	{
		auto t0 = std::chrono::high_resolution_clock::now();
		double dist = G.Dikstra(SOMMET_DEPART, SOMMET_ARRIVEE);
		auto t1 = std::chrono::high_resolution_clock::now();

		std::chrono::duration<float> fs = t1 - t0;

		std::cout << std::endl << "Affichage resultats Dikstra : " << std::endl;
		std::cout << "de " << SOMMET_DEPART << " a " << SOMMET_ARRIVEE << " : " << dist << std::endl;
		std::cout << "temps d'execution : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
	}
	{
		auto t0 = std::chrono::high_resolution_clock::now();
		double dist = G.AStar(SOMMET_DEPART, SOMMET_ARRIVEE);
		auto t1 = std::chrono::high_resolution_clock::now();

		std::chrono::duration<float> fs = t1 - t0;
		
		std::cout << std::endl << "Affichage resultats A* : " << std::endl;
		std::cout << "de " << SOMMET_DEPART << " a " << SOMMET_ARRIVEE << " : " << dist << std::endl;
		std::cout << "temps d'execution : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
	}
	
	return 0;
}
