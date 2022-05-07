#include "Graphe.h"

Vertex* Graphe::VRP1(unsigned int nbMinHab, std::string strCsvFileName)
{
	std::vector<int> villesSelect = getCitiesWithMorePeopleThan(nbMinHab, strCsvFileName);
	std::cout << "[i] nombre de villes trouvees : " << villesSelect.size() << std::endl;

	struct threadReturn {
		std::thread* th = nullptr;
		int index = -1;
		double distMin = DOUBLE_MAX;
	};

	const unsigned int NB_THREAD = std::thread::hardware_concurrency();

	std::cout << "[i] lancement sur " << NB_THREAD << " threads..." << std::endl;

	std::vector<threadReturn*> tabThread{};
	tabThread.reserve(NB_THREAD);

	int nbVille = listeSommets.size();
	int nbCityPerThread = (int)(listeSommets.size() / (NB_THREAD));
	int nbCityForLastThread = listeSommets.size() - (nbCityPerThread * NB_THREAD);

	int iVille = 0;

	// pour chaque thread :
	for (unsigned int iThread = 0; iThread < NB_THREAD; iThread++) {
		threadReturn* tr = new threadReturn{};
		tr->th = new std::thread(&Graphe::VRP1computeNCities, this, iVille, nbCityPerThread, villesSelect, std::ref(tr->distMin), std::ref(tr->index));
		//computeNCities(iVille, nbCityPerThread, villesSelect, tr->minAvg, tr->index);
		tabThread.emplace_back(tr);
		iVille += nbCityPerThread;
	}
	// dernier thread :
	double min = DOUBLE_MAX;
	int index = -1;
	VRP1computeNCities(iVille, nbCityForLastThread, villesSelect, min, index);

	// recuperation des threads, traitement et clean memoire :
	for (threadReturn* tr : tabThread) {
		tr->th->join();
		if (tr->distMin < min) {
			min = tr->distMin;
			index = tr->index;
		}
		delete tr->th;
		delete tr;
	}

	if (index == -1) {
		std::cerr << "[X] Erreur de lecture des donnees csv...";
		exit(1);
	}

	std::cout << "[i] distance moyenne des trajets : " << min / villesSelect.size() << " km" << std::endl;

	return &listeSommets[index];
}

Vertex* Graphe::VRP1v2(unsigned int nbMinHab, std::string strCsvFileName)
{
	std::vector<int> villesSelect = getCitiesWithMorePeopleThan(nbMinHab, strCsvFileName);
	std::cout << "[i] nombre de villes trouvees : " << villesSelect.size() << std::endl;

	std::vector<double> vecRes(this->listeSommets.size());

	for (int iVille = 0; iVille < villesSelect.size(); iVille++) {
		std::vector<double> res = this->DijkstraAll(villesSelect[iVille]);
		for (int i = 0; i < res.size(); i++)
			vecRes[i] += res[i];
	}

	int iMin = -1;
	double min = DOUBLE_MAX;

	for (int i = 0; i < vecRes.size(); i++)
		if (vecRes[i] < min) {
			min = vecRes[i];
			iMin = i;
		}

	std::cout << "[i] distance moyenne des trajets : " << min/villesSelect.size() << " km" << std::endl;

	return &this->listeSommets[iMin];
}

void Graphe::VRP1computeNCities(int iVille, int nbCityPerThread, const std::vector<int>& villesSelect, double& min, int& index) {
	int end = iVille + nbCityPerThread;
	for (; iVille < end; iVille++) {
		double currentSum = 0;

		for (int i_grandeVille = 0; i_grandeVille < villesSelect.size();i_grandeVille++) {
			currentSum += this->DijkstraHeap(iVille, villesSelect[i_grandeVille]);
		}

		if (currentSum < min) {
			min = currentSum;
			index = iVille;
		}
	}
}