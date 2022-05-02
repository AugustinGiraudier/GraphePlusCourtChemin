#include <iostream>
#include <vector>
#include <algorithm>
#include "Graphe.h"
#include <chrono>
#include <thread>

int main() {

	const std::string GRAPHES_ROOT = "../../InputGraphes/";
	const unsigned int SOMMET_DEPART = 2031;
	const unsigned int SOMMET_ARRIVEE = 491;
	const unsigned int MAX_AFFICHAGE = 25;
	const unsigned int NB_HABITANTS_GRANDE_VILLE_VRP_1 = 200000;
	const unsigned int NB_HABITANTS_GRANDE_VILLE_VRP_2 = 100000;

	//Graphe G = Graphe(GRAPHES_ROOT + "CommunesFrance_5000.tgoGraph");
	//Graphe G = Graphe(GRAPHES_ROOT + "Graphe_01.tgoGraph");
	Graphe G = Graphe(GRAPHES_ROOT + "CommunesFrance_5000coord.tgoGraph");
	//Graphe G = Graphe(GRAPHES_ROOT + "CommunesFrance_10000coord.tgoGraph");

	/*-------- VRP 1 --------*/

	//{
	//	auto t0 = std::chrono::high_resolution_clock::now();
	//	std::string nom = G.VRP1(NB_HABITANTS_GRANDE_VILLE_VRP_1, GRAPHES_ROOT + "CommunesFrance.csv")->nom;
	//	auto t1 = std::chrono::high_resolution_clock::now();
	//	std::chrono::duration<float> fs = t1 - t0;
	//	std::cout << "temps d'execution VRP1 : " << std::chrono::duration_cast<std::chrono::seconds>(fs).count() << " s" << std::endl;
	//	std::cout << "ville trouvee : " << nom << std::endl;
	//}

	/*-------- VRP 1 V2 --------*/

	//{
	//	auto t0 = std::chrono::high_resolution_clock::now();
	//	std::string nom = G.VRP1v2(NB_HABITANTS_GRANDE_VILLE_VRP_1, GRAPHES_ROOT + "CommunesFrance.csv")->nom;
	//	auto t1 = std::chrono::high_resolution_clock::now();
	//	std::chrono::duration<float> fs = t1 - t0;
	//	std::cout << "temps d'execution VRP1 v2 : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
	//	std::cout << "ville trouvee : " << nom << std::endl;
	//}
	
	/*-------- VRP 2 --------*/

	{
		auto t0 = std::chrono::high_resolution_clock::now();
		std::vector<int> vec = G.VRP2(NB_HABITANTS_GRANDE_VILLE_VRP_2, GRAPHES_ROOT + "communesfrance.csv");
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> fs = t1 - t0;
		std::cout << "temps d'execution vrp2 : " << std::chrono::duration_cast<std::chrono::seconds>(fs).count() << " s" << std::endl;
		std::cout << "villes trouvees : " << std::endl << std::endl;
		std::cout << G.getMyMappsScript(vec) << "\n\n";
	}	

	/*-------- VRP 2 v2 --------*/

	{
		auto t0 = std::chrono::high_resolution_clock::now();
		std::vector<int> vec = G.VRP2v2(NB_HABITANTS_GRANDE_VILLE_VRP_2, GRAPHES_ROOT + "CommunesFrance.csv");
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> fs = t1 - t0;
		std::cout << std::endl << "temps d'execution VRP2 v2 : " << std::chrono::duration_cast<std::chrono::seconds>(fs).count() << " s" << std::endl;
		std::cout << "villes trouvees : " << std::endl << std::endl;
		std::cout << G.getMyMappsScript(vec);
	}

	/*-------- Dikstra All --------*/

	//{
	//	auto t0 = std::chrono::high_resolution_clock::now();
	//	std::vector<double> vecRes = G.DikstraAll(SOMMET_DEPART);
	//	auto t1 = std::chrono::high_resolution_clock::now();

	//	std::chrono::duration<float> fs = t1 - t0;

	//	std::cout << std::endl << "Affichage resultats Dikstra All :" << std::endl;
	//	for (unsigned int i = 0; i < std::min((unsigned int)vecRes.size(), MAX_AFFICHAGE); i++) {
	//		std::cout << SOMMET_DEPART << " -> " << i << " : " << vecRes[i] << std::endl;
	//	}
	//	std::cout << "temps d'execution : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
	//}

	/*-------- Dikstra --------*/
	
	//{
	//	auto t0 = std::chrono::high_resolution_clock::now();
	//	double dist = G.Dikstra(SOMMET_DEPART, SOMMET_ARRIVEE);
	//	auto t1 = std::chrono::high_resolution_clock::now();

	//	std::chrono::duration<float> fs = t1 - t0;

	//	std::cout << std::endl << "Affichage resultats Dikstra : " << std::endl;
	//	std::cout << "de " << SOMMET_DEPART << " a " << SOMMET_ARRIVEE << " : " << dist << std::endl;
	//	std::cout << "temps d'execution : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
	//}
	
	/*-------- Dikstra 2 --------*/

	//{
	//	auto t0 = std::chrono::high_resolution_clock::now();
	//	double dist = G.Dikstra2(SOMMET_DEPART, SOMMET_ARRIVEE);
	//	auto t1 = std::chrono::high_resolution_clock::now();

	//	std::chrono::duration<float> fs = t1 - t0;

	//	std::cout << std::endl << "Affichage resultats Dikstra2 : " << std::endl;
	//	std::cout << "de " << SOMMET_DEPART << " a " << SOMMET_ARRIVEE << " : " << dist << std::endl;
	//	std::cout << "temps d'execution : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
	//}
	
	/*-------- Dikstra Heap --------*/

	//{
	//	auto t0 = std::chrono::high_resolution_clock::now();
	//	double dist = G.DikstraHeap(SOMMET_DEPART, SOMMET_ARRIVEE);
	//	auto t1 = std::chrono::high_resolution_clock::now();

	//	std::chrono::duration<float> fs = t1 - t0;

	//	std::cout << std::endl << "Affichage resultats Dikstra Heap : " << std::endl;
	//	std::cout << "de " << SOMMET_DEPART << " a " << SOMMET_ARRIVEE << " : " << dist << std::endl;
	//	std::cout << "temps d'execution : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
	//}
	
	/*-------- A* --------*/

	//{
	//	auto t0 = std::chrono::high_resolution_clock::now();
	//	double dist = G.AStar(SOMMET_DEPART, SOMMET_ARRIVEE);
	//	auto t1 = std::chrono::high_resolution_clock::now();

	//	std::chrono::duration<float> fs = t1 - t0;
	//	
	//	std::cout << std::endl << "Affichage resultats A* : " << std::endl;
	//	std::cout << "de " << SOMMET_DEPART << " a " << SOMMET_ARRIVEE << " : " << dist << std::endl;
	//	std::cout << "temps d'execution : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
	//}
	
	return 0;
}
