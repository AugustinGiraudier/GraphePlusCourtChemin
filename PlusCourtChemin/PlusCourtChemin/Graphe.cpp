#include "Graphe.h"
#include <math.h>
#include <algorithm>
#include <queue>
#include <exception>
#include <thread>
#include <list>

Graphe::Graphe(std::string filePath) {

	std::ifstream is(filePath);

	if (!is.is_open()) {
		std::cerr << "impossible d'ouvrir le fichier...";
		exit(1);
	}

	std::string line;

	getline(is, line); // nom du graphe

	getline(is, line); // oriented
	if (getSplited(line, ' ', 1).find("true") != std::string::npos) // oriented
		this->oriente = true;
	else
		this->oriente = false;

	getline(is, line); // title param

	getline(is, line); // params
	this->nbSommets = stoi(getSplited(line, ' ', 0));
	this->nbValeurParSommet = stoi(getSplited(line, ' ', 1)); // valeurs par sommet;
	this->nbArcs = stoi(getSplited(line, ' ', 2));
	this->nbValeursParArc = stoi(getSplited(line, ' ', 3));

	// initialisation des arrayList :
	this->listeSommets = std::vector<Vertex>();
	listeAdjacense.reserve(this->nbSommets);
	this->listeAdjacense = std::vector<std::list<Edge>>();
	listeAdjacense.reserve(this->nbSommets);
	for (int i = 0; i < this->nbSommets; i++)
		this->listeAdjacense.emplace_back(std::list<Edge>());
	this->degres = std::unique_ptr<int>(new int[nbSommets] {0});

	getline(is, line); // Titre Vertices

	// pour chaque vertex :
	while (getline(is, line)) {
		if (getSplited(line, ' ', 0).find("Edges") != std::string::npos)
			break;

		std::vector<double> val = std::vector<double>(this->nbValeurParSommet);
		for (int i = 0; i < this->nbValeurParSommet; i++) {
			val[i] = std::stod(getSplited(line, ' ', 2+i));
		}

		this->listeSommets.emplace_back(Vertex(getSplited(line, ' ', 1), val));
	}

	// pour chaque edge :
	while (getline(is, line)) {

		std::vector<double> val = std::vector<double>();
		val.reserve(this->nbValeursParArc);
		for (int i = 0; i < this->nbValeursParArc; i++)
			val.emplace_back(std::stod((getSplited(line, ' ', 2 + i))));

		this->listeAdjacense[std::stoi(getSplited(line, ' ', 0))].push_back(Edge(std::stoi(getSplited(line, ' ', 1)), val));
		this->degres.get()[std::stoi(getSplited(line, ' ', 0))]++;
		this->degres.get()[std::stoi(getSplited(line, ' ', 1))]++;
		if (!this->oriente) {
			this->degres.get()[std::stoi(getSplited(line, ' ', 0))]++;
			this->degres.get()[std::stoi(getSplited(line, ' ', 1))]++;
			this->listeAdjacense[std::stoi(getSplited(line, ' ', 1))].push_back(Edge(std::stoi(getSplited(line, ' ', 0)), val));
		}
	}
}

std::string Graphe::getSplited(std::string& str, const char separator, int index) {
	int nbSepa = 0;
	int id = 0;
	while (nbSepa < index && id < str.size()) {
		if (str[id] == separator)
			nbSepa++;
		id++;
	}
	std::string out = "";
	for (char* pchar = &(str[id]); *pchar != separator && pchar != &(str[str.size()]); pchar++) {
		out += *pchar;
	}
	return out;
}

Edge* Graphe::getEdgeBetween(unsigned int v1, unsigned int v2)
{
	// verifier qu'il y a bien une arrete vers v2
	for (Edge& e : this->listeAdjacense[v1]) {
		if (e.sommetTerminal == v2)
			return &e;
	}
	// sinon on retourne null
	return nullptr;
}

void Graphe::Afficher(int nbMax)
{
	std::cout << "Affichage Graphe : " << std::endl;
	std::cout << "Oriente : " << (this->oriente ? "oui" : "non") << std::endl;
	std::cout << "Nb Sommets : " << this->nbSommets << std::endl;
	std::cout << "Nb Arcs : " << this->nbArcs << std::endl;
	std::cout << "Nb Valeur par Arc : " << this->nbValeursParArc << std::endl;
	std::cout << "\nSommets :" << std::endl;
	for (int iSommet = 0; iSommet < std::min(this->nbSommets, nbMax); iSommet++) {
		std::cout << this->listeSommets[iSommet].toString() << " (deg:" + std::to_string(this->degres.get()[iSommet]) + ")";
		if (this->nbValeurParSommet > 0) {
			std::cout << " (val:";
			for (int i = 0; i < this->nbValeurParSommet; i++) {
				std::cout << " " << this->listeSommets[iSommet].values[i];
			}
			std::cout << ")";
		}
		std::cout << std::endl;
	}
	std::cout << "\nArretes :" << std::endl;
	for (int i = 0; i < std::min(this->nbSommets, nbMax); i++) {
		std::string str = "";
		str += std::to_string(i) + " : ";
		for (Edge e : this->listeAdjacense[i])
			str += e.toString() + " ";
		std::cout << str << std::endl;
	}
}

std::vector<double> Graphe::DikstraAll(unsigned int v1)
{

	// vecteur de retour lambda :
	std::vector<double> lambda = std::vector<double>(this->listeSommets.size());

	std::vector<unsigned int> Z = std::vector<unsigned int>();
	Z.reserve(this->listeSommets.size() - 1);
	for (unsigned int index = 0; index < this->listeSommets.size(); index++)
		if (index != v1) 
			Z.emplace_back(index);
	lambda[v1] = 0;

	for (unsigned int vertID : Z) {

		// recherche d'arrete v1 --> v2
		Edge* e = getEdgeBetween(v1, vertID);
		
		// si l'arrete existe :
		if (e != nullptr)
			lambda[vertID] = e->valeurs[0];
		else
			lambda[vertID] = DOUBLE_MAX;
	}

	while (!Z.empty()) {
	
		// recuperation du minimum dans lambda avec vert dans Z :
		int x = 0;
		{
			int idInZ = -1;
			int xInZ = 0;
			double minVal = DOUBLE_MAX;
			for (unsigned int vertID : Z) {
				idInZ++;
				if (lambda[vertID] <= minVal) {
					minVal = lambda[vertID];
					x = vertID;
					xInZ = idInZ;
				}
			}
			Z.erase(std::next(Z.begin(), xInZ)); // suppression de x dans Z
		}

		// pour tous les successeurs de x :
		for (Edge& e : this->listeAdjacense[x]) {
			int iId = e.sommetTerminal;

			// s'il appartient a Z :
			for (unsigned int vertID : Z) {
				if (vertID == iId) {
					// si lambda de x + l(x,i) < lambda de i :
					if (lambda[x] + e.valeurs[0] < lambda[iId])
						lambda[iId] = lambda[x] + e.valeurs[0];
					break;
				}
			}
		}
	}

	return lambda;
}

double Graphe::Dikstra(unsigned int v1, unsigned int v2)
{

	// vecteur de retour lambda :
	std::vector<double> lambda = std::vector<double>(this->listeSommets.size());

	std::vector<unsigned int> Z = std::vector<unsigned int>();
	Z.reserve(this->listeSommets.size() - 1);
	for (unsigned int index = 0; index < this->listeSommets.size(); index++)
		if (index != v1)
			Z.emplace_back(index);
	lambda[v1] = 0;

	for (unsigned int vertID : Z) {

		// recherche d'arrete v1 --> v2
		Edge* e = getEdgeBetween(v1, vertID);

		// si l'arrete existe :
		if (e != nullptr)
			lambda[vertID] = e->valeurs[0];
		else
			lambda[vertID] = DOUBLE_MAX;
	}

	while (!Z.empty()) {

		// recuperation du minimum dans lambda avec vert dans Z :
		int x = 0;
		{
			int idInZ = -1;
			int xInZ = 0;
			double minVal = DOUBLE_MAX;
			for (unsigned int vertID : Z) {
				idInZ++;
				if (lambda[vertID] <= minVal) {
					minVal = lambda[vertID];
					x = vertID;
					xInZ = idInZ;
				}
			}
			if (Z[xInZ] == v2)
				return lambda[v2];
			Z.erase(std::next(Z.begin(), xInZ)); // suppression de x dans Z
		}

		// pour tous les successeurs de x :
		for (Edge& e : this->listeAdjacense[x]) {
			int iId = e.sommetTerminal;

			// s'il appartient a Z :
			for (unsigned int vertID : Z) {
				if (vertID == iId) {
					// si lambda de x + l(x,i) < lambda de i :
					if (lambda[x] + e.valeurs[0] < lambda[iId])
						lambda[iId] = lambda[x] + e.valeurs[0];
					break;
				}
			}
		}
	}

	return -1;
}

double Graphe::DikstraHeap(unsigned int v1, unsigned int v2)
{
	std::priority_queue< std::pair<double,int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> heap;

	std::vector<double> lambda(listeSommets.size(), DOUBLE_MAX);

	heap.push(std::make_pair(0.0f, v1));
	lambda[v1] = 0;

	while (!heap.empty())
	{
		int vertID = heap.top().second;
		if (vertID == v2)
			return lambda[vertID];

		heap.pop();

		for (Edge& e : this->listeAdjacense[vertID]) {

			int v = e.sommetTerminal;
			double dist =e.valeurs[0];

			if (lambda[v] > lambda[vertID] + dist)
			{
				lambda[v] = lambda[vertID] + dist;
				heap.push(std::make_pair(lambda[v], v));
			}
		}
	}
	
	return lambda[v2];
}

double Graphe::Dikstra2(unsigned int v1, unsigned int v2)
{
	std::vector<std::pair<double, int>> Z;

	std::vector<double> lambda(listeSommets.size(), DOUBLE_MAX);

	Z.push_back(std::make_pair(0.0f, v1));
	lambda[v1] = 0;

	while (!Z.empty())
	{

		int vertID = -1;
		int vertInZ = -1;
		double min = DOUBLE_MAX;

		// trouver le min
		int ZCpt = 0;
		for (auto& pairVec : Z) {
			if (pairVec.first < min) {
				min = pairVec.first;
				vertID = pairVec.second;
				vertInZ = ZCpt;
			}
			ZCpt++;
		}

		if (vertID == v2)
			return lambda[vertID];

		Z.erase(std::next(Z.begin(), vertInZ));

		for (Edge& e : this->listeAdjacense[vertID]) {

			int v = e.sommetTerminal;
			double dist = e.valeurs[0];

			if (lambda[v] > lambda[vertID] + dist)
			{
				lambda[v] = lambda[vertID] + dist;
				Z.push_back(std::make_pair(lambda[v], v));
			}
		}
	}

	return lambda[v2];
}


double Graphe::AStar(unsigned int v1, unsigned int v2){

	// calcul des heuristiques :
	std::vector<double> heuristique = std::vector<double>(this->listeSommets.size());

	for(int iSommet=0; iSommet<this->listeSommets.size(); iSommet++)
		heuristique[iSommet] = this->computeHeuristique(this->listeSommets[iSommet], this->listeSommets[v2]);

	// vecteur de retour lambda :
	std::vector<double> lambda = std::vector<double>(this->listeSommets.size());

	std::vector<unsigned int> Z = std::vector<unsigned int>();
	Z.reserve(this->listeSommets.size() - 1);
	for (unsigned int index = 0; index < this->listeSommets.size(); index++)
		if (index != v1)
			Z.emplace_back(index);
	lambda[v1] = 0;

	for (unsigned int vertID : Z) {

		// recherche d'arrete v1 --> v2
		Edge* e = getEdgeBetween(v1, vertID);

		// si l'arrete existe :
		if (e != nullptr)
			lambda[vertID] = e->valeurs[0];
		else
			lambda[vertID] = DOUBLE_MAX;
	}

	while (!Z.empty()) {

		// recuperation du minimum dans lambda avec vert dans Z :
		int x = 0;
		{
			int idInZ = -1;
			int xInZ = 0;
			double minVal = DOUBLE_MAX;
			for (unsigned int vertID : Z) {
				idInZ++;
				double val = lambda[vertID] + heuristique[vertID];
				if (val <= minVal) {
					minVal = val;
					x = vertID;
					xInZ = idInZ;
				}
			}
			if (Z[xInZ] == v2)
				return lambda[v2];
			Z.erase(std::next(Z.begin(), xInZ)); // suppression de x dans Z
		}

		// pour tous les successeurs de x :
		for (Edge& e : this->listeAdjacense[x]) {
			int iId = e.sommetTerminal;

			// s'il appartient a Z :
			for (unsigned int vertID : Z) {
				if (vertID == iId) {
					// si lambda de x + l(x,i) < lambda de i :
					if (lambda[x] + e.valeurs[0] < lambda[iId])
						lambda[iId] = lambda[x] + e.valeurs[0];
					break;
				}
			}
		}
	}

	return -1;
}

Vertex* Graphe::VRP1(unsigned int nbMinHab, std::string strCsvFileName)
{
	std::ifstream is(strCsvFileName);

	if (!is.is_open()) {
		std::cerr << "impossible d'ouvrir le fichier...";
		exit(1);
	}

	const char SEPARATOR = ';';
	std::string line;

	getline(is, line); // nom des colonnes

	std::vector<int> villesSelect{};

	// selection des villes de + de n habitants
	while (getline(is, line)) {
		std::string ville = getSplited(line, SEPARATOR, 0);
		unsigned int nbHabitants = stoi(getSplited(line, SEPARATOR, 2));
		if (nbHabitants >= nbMinHab)
			for (int i_vert = 0; i_vert < this->listeSommets.size();i_vert++)
				if (this->listeSommets[i_vert].nom == ville){
					villesSelect.push_back(i_vert);
					break;
				}
	}

	std::cout << "nombre de villes trouvees : " << villesSelect.size() << std::endl;

	struct threadReturn {
		std::thread* th = nullptr;
		int index = -1;
		double minAvg = DOUBLE_MAX;
	};

	const unsigned int NB_THREAD = std::thread::hardware_concurrency();

	std::cout << "Lancement sur " << NB_THREAD << " threads..." << std::endl;

	std::vector<threadReturn*> tabThread {};
	tabThread.reserve(NB_THREAD);

	int nbVille = listeSommets.size();
	int nbCityPerThread = (int)(listeSommets.size() / (NB_THREAD));
	int nbCityForLastThread = listeSommets.size() - (nbCityPerThread * NB_THREAD);

	int iVille = 0;

	// pour chaque thread :
	for (unsigned int iThread = 0; iThread < NB_THREAD; iThread++) {
		threadReturn* tr = new threadReturn{};
		tr->th = new std::thread(&Graphe::computeNCities, this, iVille, nbCityPerThread, villesSelect, std::ref(tr->minAvg), std::ref(tr->index));
		//computeNCities(iVille, nbCityPerThread, villesSelect, tr->minAvg, tr->index);
		tabThread.emplace_back(tr);
		iVille += nbCityPerThread;
	}
	// dernier thread :
	double minAverage = DOUBLE_MAX;
	int index = -1;
	computeNCities(iVille, nbCityForLastThread, villesSelect, minAverage, index);

	// recuperation des threads, traitement et clean memoire :
	for (threadReturn* tr : tabThread) {
		tr->th->join();
		std::cout << "thread joined..." << std::endl;
		if (tr->minAvg < minAverage) {
			minAverage = tr->minAvg;
			index = tr->index;
		}
		delete tr->th;
		delete tr;
	}

	if (index == -1) {
		std::cerr << "Erreur de lecture des donnees csv...";
		exit(1);
	}

	return &listeSommets[index];
}

void Graphe::computeNCities(int iVille, int nbCityPerThread, const std::vector<int>& villesSelect, double& minAverage, int& index) {
	int end = iVille + nbCityPerThread;
	for (; iVille < end; iVille++) {
		double currentSum = 0;

		for (int i_grandeVille = 0; i_grandeVille < villesSelect.size();i_grandeVille++) {
			currentSum += this->DikstraHeap(iVille, villesSelect[i_grandeVille]);
		}

		double avg = currentSum / villesSelect.size();
		if (avg < minAverage) {
			minAverage = avg;
			index = iVille;
		}
	}
}

double Graphe::computeHeuristique(Vertex& v1, Vertex& v2){

	double delta = v2.values[0] - v1.values[0];
	double radlat1 = v1.values[1] * PI /180;
	double radlat2 = v2.values[1] * PI /180;
	double sinus = sin(radlat1) * sin(radlat2);
	double cosinus = cos(radlat1) * cos(radlat2) * cos(delta * PI/180);
	double Sab = acos(sinus + cosinus);
	return (Sab * 6371);

}