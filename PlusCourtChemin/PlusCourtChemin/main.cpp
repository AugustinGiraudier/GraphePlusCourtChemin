#include <iostream>
#include <vector>
#include <algorithm>
#include "Graphe.h"
#include <chrono>
#include <thread>

// commentez les tests que vous ne voulez pas lancer :

//#define TST_VRP_1_1
//#define TST_VRP_1_2

//#define TST_VRP_2_1
//#define TST_VRP_2_2

#define TST_DIJKSTRA
//#define TST_DIJKSTRA_ALL
#define TST_DIJKSTRA_HEAP

#define TST_A_STAR


int main() {

	const std::string GRAPHES_ROOT = "../../InputGraphes/";
	const std::string VILLE_DEPART = "nantes";
	const std::string VILLE_ARRIVEE = "marseille";
	const unsigned int MAX_AFFICHAGE = 25;
	const unsigned int NB_HABITANTS_GRANDE_VILLE_VRP_1 = 200000;
	const unsigned int NB_HABITANTS_GRANDE_VILLE_VRP_2 = 100000;

	//Graphe G = Graphe(GRAPHES_ROOT + "CommunesFrance_5000.tgoGraph");
	//Graphe G = Graphe(GRAPHES_ROOT + "Graphe_01.tgoGraph");
	//Graphe G = Graphe(GRAPHES_ROOT + "CommunesFrance_5000coord.tgoGraph");
	Graphe G = Graphe(GRAPHES_ROOT + "CommunesFrance_10000coord.tgoGraph");

	const unsigned int SOMMET_DEPART = G.getIndexWithName(VILLE_DEPART);
	const unsigned int SOMMET_ARRIVEE = G.getIndexWithName(VILLE_ARRIVEE);


	/*-------- VRP 1 --------*/
#ifdef TST_VRP_1_1
	{
		std::cout << "=================================================" << std::endl;
		std::cout << "[+] Lancement VRP 1 Brute Force" << std::endl;
		auto t0 = std::chrono::high_resolution_clock::now();
		std::string nom = G.VRP1(NB_HABITANTS_GRANDE_VILLE_VRP_1, GRAPHES_ROOT + "CommunesFrance.csv")->nom;
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> fs = t1 - t0;
		std::cout << "[i] ville trouvee : " << nom << std::endl;
		std::cout << "[i] temps d'execution VRP 1 Brute Force : " << std::chrono::duration_cast<std::chrono::seconds>(fs).count() << " s" << std::endl;
	}
#endif
	/*-------- VRP 1 V2 --------*/
#ifdef TST_VRP_1_2
	{
		std::cout << "=================================================" << std::endl;
		std::cout << "[+] Lancement VRP 1 Intelligent" << std::endl;
		auto t0 = std::chrono::high_resolution_clock::now();
		std::string nom = G.VRP1v2(NB_HABITANTS_GRANDE_VILLE_VRP_1, GRAPHES_ROOT + "CommunesFrance.csv")->nom;
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> fs = t1 - t0;
		std::cout << "[i] ville trouvee : " << nom << std::endl;
		std::cout << "[i] temps d'execution VRP 1 Intelligent : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
	}
#endif	
	/*-------- VRP 2 --------*/
#ifdef TST_VRP_2_1
	{
		std::cout << "=================================================" << std::endl;
		std::cout << "[+] Lancement VRP 2 Proximite" << std::endl;
		auto t0 = std::chrono::high_resolution_clock::now();
		std::vector<int> vec = G.VRP2(NB_HABITANTS_GRANDE_VILLE_VRP_2, GRAPHES_ROOT + "communesfrance.csv");
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> fs = t1 - t0;
		std::cout << "[i] villes trouvees : " << std::endl;
		for (int index : vec)
			std::cout << "[-]     " << G.getCityNameWithIndex(index) << std::endl;
		std::cout << "[i] temps d'execution VRP 2 Proximite : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
		//std::cout << G.getMyMappsScript(vec) << "\n\n";
	}	
#endif
	/*-------- VRP 2 v2 --------*/
#ifdef TST_VRP_2_2
	{
		std::cout << "=================================================" << std::endl;
		std::cout << "[+] Lancement VRP 2 Boucle" << std::endl;
		auto t0 = std::chrono::high_resolution_clock::now();
		std::vector<int> vec = G.VRP2v2(NB_HABITANTS_GRANDE_VILLE_VRP_2, GRAPHES_ROOT + "CommunesFrance.csv");
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> fs = t1 - t0;
		std::cout << "[i] villes trouvees : " << std::endl;
		for (int index : vec)
			std::cout << "[-]     " << G.getCityNameWithIndex(index) << std::endl;
		std::cout << "[i] temps d'execution VRP2 Boucle : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
		//std::cout << G.getMyMappsScript(vec);
	}
#endif
	/*-------- Dikstra --------*/
#ifdef TST_DIJKSTRA
	{
		std::cout << "=================================================" << std::endl;
		std::cout << "[+] Lancement Dikstra" << std::endl;
		std::cout << "[i] Lancement sur " << VILLE_DEPART << " - " << VILLE_ARRIVEE << std::endl;
		auto t0 = std::chrono::high_resolution_clock::now();
		double dist = G.Dijkstra(SOMMET_DEPART, SOMMET_ARRIVEE);
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> fs = t1 - t0;
		std::cout << "[i] distance trouvee : " << dist << " km" << std::endl;
		std::cout << "[i] temps d'execution : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
	}
#endif
	/*-------- Dikstra All --------*/
#ifdef TST_DIJKSTRA_ALL
	{
		std::cout << "=================================================" << std::endl;
		std::cout << "[+] Lancement Dikstra ALL" << std::endl;
		std::cout << "[i] Lancement depuis : " << VILLE_DEPART << std::endl;
		auto t0 = std::chrono::high_resolution_clock::now();
		std::vector<double> vecRes = G.DijkstraAll(SOMMET_DEPART);
		auto t1 = std::chrono::high_resolution_clock::now();

		std::chrono::duration<float> fs = t1 - t0;

		//std::cout << std::endl << "Affichage resultats Dikstra All :" << std::endl;
		//for (unsigned int i = 0; i < std::min((unsigned int)vecRes.size(), MAX_AFFICHAGE); i++) {
		//	std::cout << VILLE_DEPART << " -> " << i << " : " << vecRes[i] << std::endl;
		//}
		std::cout << "[i] temps d'execution : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
	}
#endif
	/*-------- Dikstra Heap --------*/
#ifdef TST_DIJKSTRA_HEAP
	{
		std::cout << "=================================================" << std::endl;
		std::cout << "[+] Lancement Dikstra avec Priority Queue" << std::endl;
		std::cout << "[i] Lancement sur " << VILLE_DEPART << " - " << VILLE_ARRIVEE << std::endl;
		auto t0 = std::chrono::high_resolution_clock::now();
		double dist = G.DijkstraHeap(SOMMET_DEPART, SOMMET_ARRIVEE);
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> fs = t1 - t0;
		std::cout << "[i] distance trouvee : " << dist << " km" << std::endl;
		std::cout << "[i] temps d'execution : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
	}
#endif
	/*-------- A* --------*/
#ifdef TST_A_STAR
	{
		std::cout << "=================================================" << std::endl;
		std::cout << "[+] Lancement A Etoile" << std::endl;
		std::cout << "[i] Lancement sur " << VILLE_DEPART << " - " << VILLE_ARRIVEE << std::endl;
		auto t0 = std::chrono::high_resolution_clock::now();
		double dist = G.AStar(SOMMET_DEPART, SOMMET_ARRIVEE);
		auto t1 = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> fs = t1 - t0;
		std::cout << "[i] distance trouvee : " << dist << " km" << std::endl;
		std::cout << "[i] temps d'execution : " << std::chrono::duration_cast<std::chrono::milliseconds>(fs).count() << " ms" << std::endl;
	}
#endif

	//getchar();

	return 0;
}
